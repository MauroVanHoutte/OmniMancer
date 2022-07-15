 // Fill out your copyright notice in the Description page of Project Settings.


#include "WizardCharacter.h"
#include <DrawDebugHelpers.h>
#include "Spells/BaseProjectile.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/CapsuleComponent.h>
#include <AIController.h>
#include "Powerups/PowerUpEffect.h"
#include "FloatingTextActor.h"
#include <Components/BillboardComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include "Spells/FlameColumn.h"
#include "Spells/IceZone.h"
#include "Spells/IceWall.h"
#include "Spells/Tornado.h"
#include "Spells/ChainLightning.h"
#include "Spells/Shockwave.h"
#include "Spells/BaseProjectile.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TriggerEffects.h"

// Sets default values
AWizardCharacter::AWizardCharacter()
	: ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Springarm");
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeLocationAndRotation(FVector(-10.f, 0, 300.f), FRotator(FMath::RadiansToDegrees(FMath::Asin(0.1)) + 270.f, 0, 0));
	SpringArm->bEnableCameraLag = false;
	SpringArm->CameraLagSpeed = 10.f;
	SpringArm->TargetArmLength = 1200.f;

	Camera = CreateDefaultSubobject<UCameraComponent>("TopDownCamera");
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	WizardMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	UStaticMesh* cubeMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;
	WizardMesh->SetStaticMesh(cubeMesh);
	WizardMesh->SetupAttachment(RootComponent);

	CurrentElements.Init(WizardElement::Fire, MaxElements);

	FirstElementBillboard = CreateDefaultSubobject<UBillboardComponent>("First Element");
	FirstElementBillboard->bHiddenInGame = false;
	FirstElementBillboard->SetupAttachment(RootComponent);
	FirstElementBillboard->SetRelativeLocation(FVector(0, -50, 100));
	FirstElementBillboard->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.25f));
	SecondElementBillboard = CreateDefaultSubobject<UBillboardComponent>("Second Element");
	SecondElementBillboard->bHiddenInGame = false;
	SecondElementBillboard->SetupAttachment(RootComponent);
	SecondElementBillboard->SetRelativeLocation(FVector(0, 50, 100));
	SecondElementBillboard->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.25f));
}

AWizardCharacter::AWizardCharacter(FVTableHelper& Helper)
	: AWizardCharacter()
{

}


// Called when the game starts or when spawned
void AWizardCharacter::BeginPlay()
{
	Super::BeginPlay();

	switch (MainElement)
	{
	case WizardElement::Fire:
		FirstElementBillboard->SetSprite(FireElementTexture);
		SecondElementBillboard->SetSprite(FireElementTexture);
		break;
	case WizardElement::Frost:
		FirstElementBillboard->SetSprite(FrostElementTexture);
		SecondElementBillboard->SetSprite(FrostElementTexture);
		break;
	case WizardElement::Wind:
		FirstElementBillboard->SetSprite(WindElementTexture);
		SecondElementBillboard->SetSprite(WindElementTexture);
		break;
	default:
		break;
	}

	for (auto& pair : Cooldowns)
	{
		FTimerHandle handle;
		CooldownTimers.Add(pair.Key, handle); //setup cooldown timer handles
	}

	APlayerController* controller = GetController<APlayerController>();
	if (controller != nullptr)
		controller->SetShowMouseCursor(true);

	SetupMainElementPassive();
}

void AWizardCharacter::TakeSpellDamage(ABaseSpell* spell)
{
	Health -= spell->GetDamage() * (1 + DamageTakenMultiplier / 100.f);
	SpawnDamageText(spell->GetDamage() * (1 + DamageTakenMultiplier / 100.f));
	OnTakeHit(spell->GetInstigator());
	CheckDeath();
}

void AWizardCharacter::TakeTickDamage(float damage)
{
	Health -= damage * (1 + DamageTakenMultiplier / 100.f);
	SpawnDamageText(damage * (1 + DamageTakenMultiplier / 100.f));
	CheckDeath();
}

void AWizardCharacter::OnTakeHit(AActor* cause)
{
	auto caster = Cast<ABaseCharacter>(cause);
	if (caster->IsValidLowLevel())
		caster->AddStatusEffects(ReflectEffects);
}

AWizardCharacter::~AWizardCharacter() = default;

// Called every frame
void AWizardCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdatePowerups(DeltaTime);

	APlayerController* controller = GetController<APlayerController>();
	if (controller == nullptr)
		return;

	controller->SetShowMouseCursor(true);

	FVector mousePosWorld, mouseDirWorld;
	controller->DeprojectMousePositionToWorld(mousePosWorld, mouseDirWorld);
	mousePosWorld = FMath::RayPlaneIntersection(mousePosWorld, mouseDirWorld, FPlane(GetActorLocation(), FVector(0.f, 0.f, 1.f))); //screen position raycast to plane at actor height

	WizardMesh->SetRelativeRotation((mousePosWorld - GetActorLocation()).Rotation()); //character looks towards cursor
}

void AWizardCharacter::TakeSpellDamageFloat(float damage, AActor* cause)
{
	Health -= damage * (1 + DamageTakenMultiplier / 100.f);
	SpawnDamageText(damage * (1 + DamageTakenMultiplier / 100.f));
	OnTakeHit(cause);
	CheckDeath();
}

// Called to bind functionality to input
void AWizardCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveUp", this, &AWizardCharacter::MoveUp);
	PlayerInputComponent->BindAxis("MoveRight", this, &AWizardCharacter::MoveRight);
	PlayerInputComponent->BindAction("Space", IE_Pressed, this, &AWizardCharacter::Dash);
	PlayerInputComponent->BindAxis("LMB", this, &AWizardCharacter::Fire);
	PlayerInputComponent->BindAction("RMB", IE_Pressed, this, &AWizardCharacter::CastSpell);
	PlayerInputComponent->BindAction<FAddElementDelegate>("FireElement", IE_Pressed, this, &AWizardCharacter::AddElement, WizardElement::Fire);
	PlayerInputComponent->BindAction<FAddElementDelegate>("FrostElement", IE_Pressed, this, &AWizardCharacter::AddElement, WizardElement::Frost);
	PlayerInputComponent->BindAction<FAddElementDelegate>("WindElement", IE_Pressed, this, &AWizardCharacter::AddElement, WizardElement::Wind);
}

void AWizardCharacter::OnBaseProjectileHitEnemy(AActor* enemy)
{
	for (auto& trigger : OnHitTriggers)
	{
		trigger->OnTrigger(this, enemy);
	}
}

const TArray<WizardElement>& AWizardCharacter::GetCurrentElements()
{
	return CurrentElements;
}

void AWizardCharacter::LevelUpElement(WizardElement element)
{
	switch (element)
	{
	case WizardElement::Fire:
		FireLevel++;
		break;
	case WizardElement::Frost:
		FrostLevel++;
		break;
	case WizardElement::Wind:
		WindLevel++;
		break;
	default:
		break;
	}
}

int AWizardCharacter::GetCurrentElementLevel(WizardElement element)
{
	switch (element)
	{
	case WizardElement::Fire:
		return FireLevel;
	case WizardElement::Frost:
		return FrostLevel;
	case WizardElement::Wind:
		return WindLevel;
	}
	return 0;
}

TMap<int, float>& AWizardCharacter::GetCooldowns()
{
	return Cooldowns;
}

TMap<int, FTimerHandle>& AWizardCharacter::GetCooldownTimers()
{
	return CooldownTimers;
}

void AWizardCharacter::AddPowerUpEffect( UPowerUpEffect* effect)
{
	effect->Apply(this);
	PowerUpEffects.Add(effect);
}

void AWizardCharacter::SetExplosionVariables(float damage, float radius, bool explode)
{
	ExplosionDamage = damage;
	ExplosionRadius = radius;
	ExplosiveBaseAttack = explode;
}

void AWizardCharacter::GetExplosionVariables(float& damageOut, float& radiusOut)
{
	damageOut = ExplosionDamage;
	radiusOut = ExplosionRadius;
}

void AWizardCharacter::SetSpread(int spread)
{
	Spread = spread;
}

int AWizardCharacter::GetSpread()
{
	return Spread;
}

void AWizardCharacter::SetBasicAttackCooldown(float cooldown)
{
	BasicAttackCooldown = cooldown;
}

float AWizardCharacter::GetBasicAttackCooldown()
{
	return BasicAttackCooldown;
}

void AWizardCharacter::SetSpeed(float speed)
{
	CharacterMovement->MaxWalkSpeed = speed;
}

float AWizardCharacter::GetSpeed()
{
	return CharacterMovement->MaxWalkSpeed;
}

void AWizardCharacter::SetBounces(int bounces)
{
	Bounces = bounces;
}

int AWizardCharacter::GetBounces()
{
	return Bounces;
}

TArray<FStatusEffect>& AWizardCharacter::GetBaseAttackEffectsRef()
{
	return BaseAttackEffects;
}

void AWizardCharacter::LowerCooldowns(float amount)
{
	for (auto cooldownPair : CooldownTimers)
	{
		auto& timerManager = GetWorld()->GetTimerManager();
		if (GetWorld()->GetTimerManager().IsTimerActive(cooldownPair.Value))
		{
			float remaining = timerManager.GetTimerRemaining(cooldownPair.Value);
			if (remaining < amount)
			{
				timerManager.PauseTimer(cooldownPair.Value);
			}
			else
			{
				timerManager.SetTimer(cooldownPair.Value, remaining - amount, false);
			}
		}
	}
}

void AWizardCharacter::MoveUp(float value)
{
	AddMovementInput(FVector(1.f, 0.f, 0.f), value);
}

void AWizardCharacter::MoveRight(float value)
{
	AddMovementInput(FVector(0.f, 1.f, 0.f), value);
}

void AWizardCharacter::AddElement(WizardElement element)
{
	switch (element) //check if allowed to add element
	{
	case WizardElement::Fire:
		if (FireLevel < 1)
			return;
		break;
	case WizardElement::Frost:
		if (FrostLevel < 1)
			return;
		break;
	case WizardElement::Wind:
		if (WindLevel < 1)
			return;
		break;
	default:
		break;
	}

	switch (CurrentElements[1]) // passive effect of to be removed element removed
	{
	case WizardElement::Fire:
		BaseDamageMultiplier -= DamagePerFire;
		break;
	case WizardElement::Frost:
		DamageTakenMultiplier += DamageReductionPerFrost;
		break;
	case WizardElement::Wind:
		GetCharacterMovement()->MaxWalkSpeed /= (1+SpeedPerWind/100.f);
		break;
	default:
		break;
	}

	CurrentElements.Pop();
	CurrentElements.Insert(element, 0); // last element discarde new element inserted
	
	switch (CurrentElements[0]) // updating passive effects
	{
	case WizardElement::Fire:
		FirstElementBillboard->SetSprite(FireElementTexture);
		BaseDamageMultiplier += DamagePerFire;
		break;
	case WizardElement::Frost:
		FirstElementBillboard->SetSprite(FrostElementTexture);
		DamageTakenMultiplier -= DamageReductionPerFrost;
		break;
	case WizardElement::Wind:
		FirstElementBillboard->SetSprite(WindElementTexture);
		GetCharacterMovement()->MaxWalkSpeed *= (1 + SpeedPerWind / 100.f);
		break;
	}

	switch (CurrentElements[1]) // updating visuals
	{
	case WizardElement::Fire:
		SecondElementBillboard->SetSprite(FireElementTexture);
		break;
	case WizardElement::Frost:
		SecondElementBillboard->SetSprite(FrostElementTexture);
		break;
	case WizardElement::Wind:
		SecondElementBillboard->SetSprite(WindElementTexture);
		break;
	}

	OnAddElement();
}

void AWizardCharacter::Fire(float input)
{
	if (input < 1.f || GetWorld()->GetTimerManager().IsTimerActive(BasicAttackTimer))
		return;

	GetWorld()->GetTimerManager().SetTimer(BasicAttackTimer, BasicAttackCooldown, false);

	APlayerController* controller = GetController<APlayerController>();

	FVector mousePosWorld, mouseDirWorld;
	controller->DeprojectMousePositionToWorld(mousePosWorld, mouseDirWorld);
	mousePosWorld = FMath::RayPlaneIntersection(mousePosWorld, mouseDirWorld, FPlane(GetActorLocation(), FVector(0.f, 0.f, 1.f))); // screen position raycast to plane at actor height

	FVector direction{ (mousePosWorld - GetActorLocation()).GetSafeNormal() }; // aiming direction

	FActorSpawnParameters spawnParams{};
	spawnParams.Owner = this;
	spawnParams.Instigator = GetInstigator();

	auto actor = GetWorld()->SpawnActor<AActor>(BaseProjectile.Get(), spawnParams);
	auto projectile = Cast<ABaseProjectile>(actor);

	InitProjectile(projectile, direction);

	for (int i = 0; i < Spread; i++)
	{
		float angleOffset = ((i + 1) * 45.f / Spread);
		auto actorCW = GetWorld()->SpawnActor<AActor>(BaseProjectile.Get(), spawnParams);
		auto actorCCW = GetWorld()->SpawnActor<AActor>(BaseProjectile.Get(), spawnParams);
		auto projectileCW = Cast<ABaseProjectile>(actorCW);
		auto projectileCCW = Cast<ABaseProjectile>(actorCCW);

		auto rotation = direction.Rotation();
		auto rotationCW = rotation;
		rotationCW.Yaw += angleOffset;
		auto rotationCCW = rotation;
		rotationCCW.Yaw -= angleOffset;

		InitProjectile(projectileCW, rotationCW.Vector());
		InitProjectile(projectileCCW, rotationCCW.Vector());
	}
}

void AWizardCharacter::InitProjectile(ABaseProjectile* projectile, const FVector& direction)
{
	if (projectile)
	{
		projectile->SetDamageMultiplier(BaseDamageMultiplier);
		projectile->SetActorScale3D(FVector(0.75f, 0.75f, 0.75f));
		projectile->SetActorLocationAndRotation(GetActorLocation(), direction.Rotation());
		projectile->SetBounces(Bounces);
		projectile->FireInDirection(direction);

		for (const auto& effect : BaseAttackEffects)
		{
			projectile->AddStatusEffect(effect);
		}

		projectile->SetInstigator(GetInstigator());
		projectile->SetOwner(this);

		if (ExplosiveBaseAttack)
		{
			projectile->SetExplosion(ExplosionRadius, ExplosionDamage);
		}
	}
}

void AWizardCharacter::CastSpell()
{
	FVector worldPos, worldDir;
	GetController<APlayerController>()->DeprojectMousePositionToWorld(worldPos, worldDir);
	FHitResult hit{};
	FCollisionObjectQueryParams collisionParams{};
	collisionParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	GetWorld()->LineTraceSingleByObjectType(hit, worldPos, worldPos + worldDir * 100000, collisionParams); //get mouse position on static level
	
	FVector mouseWorldPosAtActorHeight = FMath::RayPlaneIntersection(worldPos, worldDir, FPlane(GetActorLocation(), FVector(0, 0, 1)));
	FVector projectileDirection = (mouseWorldPosAtActorHeight - GetActorLocation()).GetSafeNormal(); // direction for projectile spells

	int spellID{};

	for ( auto element : CurrentElements)
	{
		spellID += int(element);
	}
	// Fire -> 0, Frost -> 1, Wind -> 3
	// 0 + 0 = 0  ---> FlameColumn
	// 0 + 1 = 1  ---> IceZone
	// 1 + 1 = 2  ---> IceWall
	// 0 + 3 = 3  ---> Tornado
	// 1 + 3 = 4  ---> ChainLightning
	// 3 + 3 = 6  ---> Shockwave

	if (GetWorld()->GetTimerManager().IsTimerActive(CooldownTimers[spellID]))
	{
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(CooldownTimers[spellID], Cooldowns[spellID], false);

	auto spellType = Spells.Find(spellID);
	if (spellType == nullptr)
		return;

	 auto spell = GetWorld()->SpawnActor<ABaseSpell>(*spellType);
	 spell->InitSpell(GetActorLocation(), hit.Location, projectileDirection, this, GetInstigator(), FireLevel, FrostLevel, WindLevel);

	 for (auto& trigger : OnCastTriggers)
	 {
		 trigger->OnTrigger(this, nullptr);
	 }
}

void AWizardCharacter::Dash()
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(m_DashCooldownTimer))
	{
		Cast<UCharacterMovementComponent>(GetMovementComponent())->AddImpulse(GetMovementComponent()->Velocity.GetSafeNormal() * m_DashForce, true);
		GetWorld()->GetTimerManager().SetTimer(m_DashCooldownTimer, m_DashCooldown, false);
	}
}

void AWizardCharacter::UpdatePowerups(float deltaTime)
{
	for (int32 i = 0; i < PowerUpEffects.Num(); i++)
	{
		PowerUpEffects[i]->Timer += deltaTime;
		if (PowerUpEffects[i]->Timer > PowerUpEffects[i]->Duration)
		{
			PowerUpEffects[i]->Remove(this); // remove powerup effects from player
			PowerUpEffects[i]->ConditionalBeginDestroy();
			PowerUpEffects.RemoveAt(i);
			i--;
		}
	}
}

void AWizardCharacter::SetupMainElementPassive()
{
	switch (MainElement)
	{
	case WizardElement::Fire:
		ReflectEffects.Add(FStatusEffect{Type::Damage, 1, 1, 5, this});
		break;
	case WizardElement::Frost:
	{
		auto slowTrigger = new TriggerEffects::AoeSlowTrigger();
		slowTrigger->SetVars(600, 20, 3);
		OnCastTriggers.Add(TUniquePtr<TriggerEffects::BaseTriggerEffect>(slowTrigger));
	}
		break;
	case WizardElement::Wind:
	{
		auto speedBuffTrigger = new TriggerEffects::SpeedBuffTrigger();
		speedBuffTrigger->SetVars(20, 3);
		OnCastTriggers.Add(TUniquePtr<TriggerEffects::BaseTriggerEffect>(speedBuffTrigger));
	}
		break;
	default:
		break;
	}

	auto lowerCooldownTrigger = new TriggerEffects::LowerCooldownTrigger();
	lowerCooldownTrigger->SetVars(30, 1);
	OnHitTriggers.Add(TUniquePtr<TriggerEffects::BaseTriggerEffect>(lowerCooldownTrigger));
}

