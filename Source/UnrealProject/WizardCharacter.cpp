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

	if (FlameColumnClass == nullptr)
		UE_LOG(LogTemp, Warning, TEXT("FlameColumn not set on Wizard"));
	if (IceZoneClass == nullptr)
		UE_LOG(LogTemp, Warning, TEXT("IceZone not set on Wizard"));
	if (IceWallClass == nullptr)
		UE_LOG(LogTemp, Warning, TEXT("IceWall not set on Wizard"));
	if (TornadoClass == nullptr)
		UE_LOG(LogTemp, Warning, TEXT("Tornado not set on Wizard"));
	if (ChainLighningClass == nullptr)
		UE_LOG(LogTemp, Warning, TEXT("Chainlightning not set on Wizard"));
	if (ShockwaveClass == nullptr)
		UE_LOG(LogTemp, Warning, TEXT("Shockwave not set on Wizard"));

}

// Called when the game starts or when spawned
void AWizardCharacter::BeginPlay()
{
	Super::BeginPlay();
	FirstElementBillboard->SetSprite(FireElementTexture);
	SecondElementBillboard->SetSprite(FireElementTexture);

	for (auto& pair : Cooldowns)
	{
		FTimerHandle handle;
		CooldownTimers.Add(pair.Key, handle);
	}

	APlayerController* controller = GetController<APlayerController>();
	if (controller != nullptr)
		controller->SetShowMouseCursor(true);
}

void AWizardCharacter::TakeSpellDamage(float damage)
{
	Health -= damage * (1 + DamageTakenMultiplier/100.f);
	SpawnDamageText(damage * (1 + DamageTakenMultiplier / 100.f));
	CheckDeath();
}

void AWizardCharacter::TakeTickDamage(float damage)
{
	Health -= damage * (1 + DamageTakenMultiplier / 100.f);
	SpawnDamageText(damage * (1 + DamageTakenMultiplier / 100.f));
	CheckDeath();
}

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

const TArray<WizardElement>& AWizardCharacter::GetCurrentElements()
{
	return CurrentElements;
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

	switch (spellID)
	{
	case 0:
		CastFlameColumn(hit.Location);
		break;
	case 1:
		CastIceZone(hit.Location);
		break;
	case 2:
		CastIceWall(hit.Location);
		break;
	case 3:
		CastTornado(projectileDirection);
		break;
	case 4:
		CastChainLightning(projectileDirection);
		break;
	case 6:
		CastShockwave();
		break;
	default:
		break;
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

void AWizardCharacter::CastFlameColumn(const FVector& worldPos)
{
	auto spell = GetWorld()->SpawnActor(FlameColumnClass);
	spell->SetOwner(this);
	spell->SetInstigator(GetInstigator());
	spell->SetActorLocation(worldPos);
}

void AWizardCharacter::CastIceZone(const FVector& worldPos)
{
	auto spell = GetWorld()->SpawnActor(IceZoneClass);
	spell->SetOwner(this);
	spell->SetInstigator(GetInstigator());
	spell->SetActorLocation(worldPos);
	auto icezone = Cast<AIceZone>(spell);
	icezone->SetBurnParams(true, 1.f, 0.2f, 2.f);
	icezone->SetSlowParams(true, 50, 2.f);
	icezone->SetRadius(7.f);
	icezone->SetLifetime(2.f);
}

void AWizardCharacter::CastIceWall(const FVector& worldPos)
{
	auto spell = GetWorld()->SpawnActor(IceWallClass);
	spell->SetOwner(this);
	spell->SetInstigator(GetInstigator());
	spell->SetActorLocation(worldPos);
	spell->SetActorRotation( (worldPos - GetActorLocation()).Rotation() );
}

void AWizardCharacter::CastTornado(const FVector& direction)
{
	auto spell = GetWorld()->SpawnActor(TornadoClass);
	spell->SetOwner(this);
	spell->SetInstigator(GetInstigator());
	spell->SetActorLocation(GetActorLocation());
	Cast<ATornado>(spell)->FireInDirection(direction);
}

void AWizardCharacter::CastChainLightning(const FVector& direction)
{
	auto spell = GetWorld()->SpawnActor(ChainLighningClass);
	spell->SetOwner(this);
	spell->SetInstigator(GetInstigator());
	spell->SetActorLocation(GetActorLocation());
	Cast<AChainLightning>(spell)->FireInDirection(direction);
}

void AWizardCharacter::CastShockwave()
{
	auto spell = GetWorld()->SpawnActor(ShockwaveClass);
	spell->SetOwner(this);
	spell->SetInstigator(GetInstigator());
	spell->SetActorLocation(GetActorLocation());
}

