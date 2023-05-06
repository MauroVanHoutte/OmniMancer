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
#include "Kismet/GameplayStatics.h"
#include "Upgrades/Triggers/TriggerEffects.h"
#include "OmnimancerGameInstance.h"
#include <Blueprint/UserWidget.h>
#include "UI/PlayerHUD.h"

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

AWizardCharacter::~AWizardCharacter() = default;


// Called when the game starts or when spawned
void AWizardCharacter::BeginPlay()
{
	Super::BeginPlay();

	switch (MainElement)
	{
	case WizardElement::Fire:
		FirstElementBillboard->SetSprite(FireElementTexture);
		SecondElementBillboard->SetSprite(FireElementTexture);
		BaseDamageMultiplier *= 1+DamagePerFire;
		BaseDamageMultiplier *= 1+DamagePerFire;
		break;
	case WizardElement::Frost:
		FirstElementBillboard->SetSprite(FrostElementTexture);
		SecondElementBillboard->SetSprite(FrostElementTexture);
		DamageTakenMultiplier *= 1-DamageReductionPerFrost;
		DamageTakenMultiplier *= 1-DamageReductionPerFrost;
		break;
	case WizardElement::Wind:
		FirstElementBillboard->SetSprite(WindElementTexture);
		SecondElementBillboard->SetSprite(WindElementTexture);
		GetCharacterMovement()->MaxWalkSpeed *= 1 + SpeedPerWind;
		GetCharacterMovement()->MaxWalkSpeed *= 1 + SpeedPerWind;
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
	{
		controller->SetShowMouseCursor(true);
		PlayerHud = CreateWidget<UPlayerHUD>(controller, PlayerHudType);
		PlayerHud->AddToPlayerScreen();
		PlayerHud->Setup();

		controller->SetShowMouseCursor(true);

	}

	SetupUpgrades();
}

void AWizardCharacter::TakeSpellDamage(ABaseSpell* spell)
{
	Health -= spell->GetDamage() * DamageTakenMultiplier;
	SpawnDamageText(spell->GetDamage() * DamageTakenMultiplier);
	OnTakeHit(spell->GetInstigator());
	CheckDeath();
}

void AWizardCharacter::TakeTickDamage(float damage)
{
	Health -= damage * DamageTakenMultiplier;
	SpawnDamageText(damage * DamageTakenMultiplier);
	CheckDeath();
}

void AWizardCharacter::OnTakeHit(AActor* cause)
{
	auto caster = Cast<ABaseCharacter>(cause);
	if (caster->IsValidLowLevel())
		caster->AddStatusEffects(ReflectEffects);

	for (auto& trigger : TriggerEffects)
	{
		if (trigger->Condition == TriggerCondition::OnTakeHit)
			trigger->OnTrigger(this, nullptr, caster);
	}
}

void AWizardCharacter::OnDeath()
{
	for (UCharacterUpgrade* upgrade : CharacterUpgrades)
	{
		upgrade->Remove(this);
	}
}


// Called every frame
void AWizardCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdatePowerups(DeltaTime);

	APlayerController* controller = GetController<APlayerController>();
	FVector mousePosWorld, mouseDirWorld;
	controller->DeprojectMousePositionToWorld(mousePosWorld, mouseDirWorld);
	mousePosWorld = FMath::RayPlaneIntersection(mousePosWorld, mouseDirWorld, FPlane(GetActorLocation(), FVector(0.f, 0.f, 1.f))); //screen position raycast to plane at actor height

	auto desiredRotation = (mousePosWorld - GetActorLocation()).Rotation();
	desiredRotation.Yaw -= 90.f;

	GetMesh()->SetRelativeRotation(desiredRotation); //character looks towards cursor

	//player can zoom camera in and out to see the level layout
	if (!FMath::IsNearlyEqual(SpringArm->TargetArmLength, TargetSpringArmLength))
		SpringArm->TargetArmLength = FMath::FInterpConstantTo(SpringArm->TargetArmLength, TargetSpringArmLength, DeltaTime, CameraZoomSpeed);
}

void AWizardCharacter::TakeSpellDamageFloat(float damage, AActor* cause)
{
	Health -= damage * DamageTakenMultiplier;
	SpawnDamageText(damage * DamageTakenMultiplier);
	OnTakeHit(cause);
	CheckDeath();
}

// Called to bind functionality to input
void AWizardCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent = PlayerInputComponent;

	PlayerInputComponent->BindAxis("MoveUp", this, &AWizardCharacter::MoveUp);
	PlayerInputComponent->BindAxis("MoveRight", this, &AWizardCharacter::MoveRight);
	PlayerInputComponent->BindAction("Space", IE_Pressed, this, &AWizardCharacter::Dash);
	PlayerInputComponent->BindAxis("LMB", this, &AWizardCharacter::Fire); //IE_Repeat doesnt work with mouse buttons
	PlayerInputComponent->BindAction("RMB", IE_Pressed, this, &AWizardCharacter::CastSpell);
	PlayerInputComponent->BindAction<FAddElementDelegate>("FireElement", IE_Pressed, this, &AWizardCharacter::AddElement, WizardElement::Fire);
	PlayerInputComponent->BindAction<FAddElementDelegate>("FrostElement", IE_Pressed, this, &AWizardCharacter::AddElement, WizardElement::Frost);
	PlayerInputComponent->BindAction<FAddElementDelegate>("WindElement", IE_Pressed, this, &AWizardCharacter::AddElement, WizardElement::Wind);
	auto& action = PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &AWizardCharacter::Pause);
	action.bExecuteWhenPaused = true;
	PlayerInputComponent->BindAction("OpenMap", IE_Pressed, this, &AWizardCharacter::ShowMap);
	PlayerInputComponent->BindAction("OpenMap", IE_Released, this, &AWizardCharacter::HideMap);
}

void AWizardCharacter::OnSpellHitEnemy( ABaseSpell* spell, ABaseCharacter* enemy)
{
	for (auto& trigger : TriggerEffects)
	{
		if (trigger->Condition == TriggerCondition::OnHit)
		trigger->OnTrigger(this, spell, enemy);
	}
}

const TArray<WizardElement>& AWizardCharacter::GetCurrentElements() const
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

int AWizardCharacter::GetCurrentElementLevel(WizardElement element) const
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

const TMap<int, float>& AWizardCharacter::GetCooldowns() const
{
	return Cooldowns;
}

const TMap<int, FTimerHandle>& AWizardCharacter::GetCooldownTimers() const
{
	return CooldownTimers;
}

void AWizardCharacter::AddPowerUpEffect( UPowerUpEffect* effect)
{
	effect->Apply(this);
	PowerUpEffects.Add(effect);
}

void AWizardCharacter::AddTriggerEffect(UBaseTriggerEffect* effect)
{
	TriggerEffects.Add(effect);
}

void AWizardCharacter::RemoveTriggerEffect(UBaseTriggerEffect* effect)
{
	for (size_t i = 0; i < TriggerEffects.Num(); i++)
	{
		if (TriggerEffects[i] == effect)
		{
			TriggerEffects.RemoveAt(i);
			--i;
		}
	}
}

void AWizardCharacter::ApplyUpgrade(const FString& tag)
{
	(* CharacterUpgrades.FindByPredicate([tag](const UCharacterUpgrade* upgrade)
		{
			return upgrade->Tag.Compare(tag) == 0;
		}))->Apply(this);
}

void AWizardCharacter::RemoveUpgrade(const FString& tag)
{
	(* CharacterUpgrades.FindByPredicate([tag](const UCharacterUpgrade* upgrade)
		{
			return upgrade->Tag.Compare(tag) == 0;
		}))->Remove(this);
}

void AWizardCharacter::SetExplosionVariables(float damage, float radius, bool explode)
{
	ExplosionDamage = damage;
	ExplosionRadius = radius;
	ExplosiveBaseAttack = explode;
}

void AWizardCharacter::GetExplosionVariables(float& damageOut, float& radiusOut) const
{
	damageOut = ExplosionDamage;
	radiusOut = ExplosionRadius;
}

void AWizardCharacter::SetSpread(int spread)
{
	Spread = spread;
}

int AWizardCharacter::GetSpread() const
{
	return Spread;
}

void AWizardCharacter::SetBasicAttackCooldown(float cooldown)
{
	BasicAttackCooldown = cooldown;
}

float AWizardCharacter::GetBasicAttackCooldown() const
{
	return BasicAttackCooldown;
}

void AWizardCharacter::SetSpeed(float speed)
{
	CharacterMovement->MaxWalkSpeed = speed;
}

float AWizardCharacter::GetSpeed() const
{
	return CharacterMovement->MaxWalkSpeed;
}

void AWizardCharacter::SetBounces(int bounces)
{
	Bounces = bounces;
}

int AWizardCharacter::GetBounces() const
{
	return Bounces;
}

float AWizardCharacter::GetSpellDamageMultiplier() const
{
	return SpellDamageMultiplier;
}

void AWizardCharacter::SetSpellDamageMultiplier(float newSpellDamage)
{
	SpellDamageMultiplier = newSpellDamage;
}

float AWizardCharacter::GetBaseProjecileDamageMultiplier() const
{
	return BaseDamageMultiplier;
}

void AWizardCharacter::SetBaseProjectileDamageMultiplier(float newBaseProjectileMultiplier)
{
	BaseDamageMultiplier = newBaseProjectileMultiplier;
}

float AWizardCharacter::GetDamageTakenMultiplier() const
{
	return DamageTakenMultiplier;
}

void AWizardCharacter::SetDamageTakenMultiplier(float newDamageTaken)
{
	DamageTakenMultiplier = newDamageTaken;
}

float AWizardCharacter::GetBurnDurationMultiplier() const
{
	return BurnDurationMultiplier;
}

void AWizardCharacter::SetBurnDurationMultiplier(float newBurnDurationMultiplier)
{
	BurnDurationMultiplier = newBurnDurationMultiplier;
}

float AWizardCharacter::GetSlowDurationMultiplier() const
{
	return SlowDurationMultiplier;
}

void AWizardCharacter::SetSlowDurationMultiplier(float newSlowDurationMultiplier)
{
	SlowDurationMultiplier = newSlowDurationMultiplier;
}

float AWizardCharacter::GetStunDurationMultiplier() const
{
	return StunDurationMultiplier;
}

void AWizardCharacter::SetStunDurationMultiplier(float newStunDurationMultiplier)
{
	StunDurationMultiplier = newStunDurationMultiplier;
}

float AWizardCharacter::GetDashForce() const
{
	return DashForce;
}

void AWizardCharacter::SetDashForce(float newDashForce)
{
	DashForce = newDashForce;
}

void AWizardCharacter::AddBaseAttackStatusEffect(const FStatusEffect& effect)
{
	BaseAttackEffects.Add(effect);
}

void AWizardCharacter::RemoveBaseAttackStatusEffect(const FStatusEffect& effect)
{
	BaseAttackEffects.Remove(effect);
}

void AWizardCharacter::AddReflectStatusEffect(const FStatusEffect& effect)
{
	ReflectEffects.Add(effect);
}

void AWizardCharacter::RemoveReflectStatusEffect(const FStatusEffect& effect)
{
	ReflectEffects.Remove(effect);
}

TArray<FStatusEffect>& AWizardCharacter::GetBaseAttackEffectsRef()
{
	return BaseAttackEffects;
}

float AWizardCharacter::GetExperiencePercentage() const
{
	return Experience/(float)ExeperienceRequired;
}

void AWizardCharacter::PickupExperience(int experience)
{
	Experience += experience;
	if (Experience > ExeperienceRequired)
	{
		Experience -= ExeperienceRequired;
		ExeperienceRequired *= NextLevelExperienceRequired;
		OnLevelUp();
	}
	PlayerHud->SetExperiencePercentage(float(Experience) / ExeperienceRequired);
}

int AWizardCharacter::GetCombinedElementLevel() const
{
	return FireLevel + WindLevel + FrostLevel;
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

void AWizardCharacter::Pause()
{
	UGameplayStatics::SetGamePaused(GetWorld(), !UGameplayStatics::IsGamePaused(GetWorld()));
	
	//Add/remove Pause menu widget to screen
	if (UGameplayStatics::IsGamePaused(GetWorld()))
	{
		PlayerHud->SetVisibility(ESlateVisibility::Collapsed);
		if (PauseMenu->IsValidLowLevel())
		{
			PauseMenu->AddToPlayerScreen();
			return;
		}

		PauseMenu = CreateWidget<UUserWidget>(GetController<APlayerController>(), PauseMenuType);
		PauseMenu->AddToPlayerScreen();
	}
	else
	{
		if (PauseMenu->IsValidLowLevel())
		{
			PauseMenu->RemoveFromParent();
		}
		PlayerHud->SetVisibility(ESlateVisibility::Visible);
	}
}

void AWizardCharacter::ShowMap()
{
	InputComponent->BindAxis("LMB");
	InputComponent->RemoveActionBinding("RMB", IE_Pressed);
	TargetSpringArmLength = SpringArmZoomedOutLength;
	//Camera->SetProjectionMode(ECameraProjectionMode::Orthographic); need to gradually change fov to work
}

void AWizardCharacter::HideMap()
{
	InputComponent->BindAxis("LMB", this, &AWizardCharacter::Fire);
	InputComponent->BindAction("RMB", IE_Pressed, this, &AWizardCharacter::CastSpell);
	TargetSpringArmLength = SpringArmLength;
	//Camera->SetProjectionMode(ECameraProjectionMode::Perspective);
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
		BaseDamageMultiplier /= 1+DamagePerFire;
		break;
	case WizardElement::Frost:
		DamageTakenMultiplier /= 1-DamageReductionPerFrost;
		break;
	case WizardElement::Wind:
		GetCharacterMovement()->MaxWalkSpeed /= 1+SpeedPerWind;
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
		BaseDamageMultiplier *= 1+DamagePerFire;
		break;
	case WizardElement::Frost:
		FirstElementBillboard->SetSprite(FrostElementTexture);
		DamageTakenMultiplier *= 1-DamageReductionPerFrost;
		break;
	case WizardElement::Wind:
		FirstElementBillboard->SetSprite(WindElementTexture);
		GetCharacterMovement()->MaxWalkSpeed *= 1 + SpeedPerWind;
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
	if (input < 1 || GetWorld()->GetTimerManager().IsTimerActive(BasicAttackTimer))
		return;

	GetWorld()->GetTimerManager().SetTimer(BasicAttackTimer, BasicAttackCooldown, false);

	APlayerController* controller = GetController<APlayerController>();

	FVector mousePosWorld, mouseDirWorld;
	controller->DeprojectMousePositionToWorld(mousePosWorld, mouseDirWorld);
	mousePosWorld = FMath::RayPlaneIntersection(mousePosWorld, mouseDirWorld, FPlane(GetActorLocation(), FVector(0.f, 0.f, 1.f))); // screen position raycast to plane at actor height

	FVector direction{ (mousePosWorld - GetActorLocation()).GetSafeNormal() }; // aiming direction

	FActorSpawnParameters spawnParams{};
	spawnParams.Owner = this;
	spawnParams.Instigator = this;

	auto projectile = GetWorld()->SpawnActor<ABaseProjectile>(BaseProjectile.Get(), spawnParams);

	InitProjectile(projectile, direction);

	SpawnSpreadProjectiles(spawnParams, direction);
}

void AWizardCharacter::SpawnSpreadProjectiles(FActorSpawnParameters& spawnParams, FVector& direction)
{
	for (int i = 0; i < Spread; i++)
	{
		float angleOffset = ((i + 1) * 45.f / Spread);
		auto projectileCW = GetWorld()->SpawnActor<ABaseProjectile>(BaseProjectile.Get(), spawnParams);
		auto projectileCCW = GetWorld()->SpawnActor<ABaseProjectile>(BaseProjectile.Get(), spawnParams);

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
		for (const auto& effect : BaseAttackEffects)
		{
			projectile->AddStatusEffect(effect);
		}

		projectile->InitSpell(FVector(), direction, this);

		if (ExplosiveBaseAttack)
		{
			projectile->SetExplosion(ExplosionRadius, ExplosionDamage);
		}

		projectile->Initialized = true;
	}
}

// Fire -> 0, Frost -> 1, Wind -> 3
// 0 + 0 = 0  ---> FlameColumn
// 0 + 1 = 1  ---> IceZone
// 1 + 1 = 2  ---> IceWall
// 0 + 3 = 3  ---> Tornado
// 1 + 3 = 4  ---> ChainLightning
// 3 + 3 = 6  ---> Shockwave

void AWizardCharacter::CastSpell()
{
	FVector mousePosAtActorHeight;
	FHitResult raycastHit;
	RaycastMouseOnLevel(mousePosAtActorHeight, raycastHit);
	FVector projectileDirection = (mousePosAtActorHeight - GetActorLocation()).GetSafeNormal(); // direction for projectile spells

	int spellID{};
	for ( auto element : CurrentElements)
	{
		spellID += int(element);
	}

	if (GetWorld()->GetTimerManager().IsTimerActive(CooldownTimers[spellID])) //spell is on cooldown
		return; 

	GetWorld()->GetTimerManager().SetTimer(CooldownTimers[spellID], Cooldowns[spellID], false); //start 

	auto spellType = Spells.Find(spellID);
	if (spellType == nullptr)
		return;

	FActorSpawnParameters spawnParams{};
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	auto spell = GetWorld()->SpawnActor<ABaseSpell>(*spellType, spawnParams);
	if (spell != nullptr)
		spell->InitSpell(raycastHit.Location, projectileDirection, this); //virtual init overriden in derived spells

	for (auto& trigger : TriggerEffects)
		if (trigger->Condition == TriggerCondition::OnCast)
			trigger->OnTrigger(this, nullptr, nullptr);
}

void AWizardCharacter::Dash()
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(DashCooldownTimer))
	{
		Cast<UCharacterMovementComponent>(GetMovementComponent())->AddImpulse(GetMovementComponent()->Velocity.GetSafeNormal() * DashForce, true);
		GetWorld()->GetTimerManager().SetTimer(DashCooldownTimer, DashCooldown, false);
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

void AWizardCharacter::SetupUpgrades()
{
	auto gameInstance = GetGameInstance<UOmnimancerGameInstance>();
	if (gameInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Game instance nullptr"))
		return;
	}
	auto& unlockedUpgrades = gameInstance->GetUnlockedUpgrades();
	unlockedUpgrades.Add("FreeUpgrade");

	for (const auto& upgradeTag : unlockedUpgrades)
	{
		UCharacterUpgrade** matchingUpgrade = CharacterUpgrades.FindByPredicate([upgradeTag](UCharacterUpgrade* upgrade) 
			{
				return upgrade->Tag == upgradeTag;
			});
		
		if (matchingUpgrade != nullptr)
		{
			(*matchingUpgrade)->Apply(this);
		}
	}
}

void AWizardCharacter::RaycastMouseOnLevel(FVector& mouseAtWizardHeight, FHitResult& raycastHit)
{
	FVector worldPos, worldDir;
	GetController<APlayerController>()->DeprojectMousePositionToWorld(worldPos, worldDir);
	FCollisionObjectQueryParams collisionParams{};
	collisionParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	GetWorld()->LineTraceSingleByObjectType(raycastHit, worldPos, worldPos + worldDir * 100000, collisionParams); //get mouse position on static level
	mouseAtWizardHeight = FMath::RayPlaneIntersection(worldPos, worldDir, FPlane(GetActorLocation(), FVector(0, 0, 1)));
}

