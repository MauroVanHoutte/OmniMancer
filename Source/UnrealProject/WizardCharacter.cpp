 // Fill out your copyright notice in the Description page of Project Settings.


#include "WizardCharacter.h"
#include <DrawDebugHelpers.h>
#include "Spells/BaseProjectile.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/CapsuleComponent.h>
#include <AIController.h>
#include "Powerups/PowerUpEffect.h"

// Sets default values
AWizardCharacter::AWizardCharacter()
	: ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_SpringArm = CreateDefaultSubobject<USpringArmComponent>("Springarm");
	m_SpringArm->SetupAttachment(RootComponent);
	m_SpringArm->SetRelativeLocationAndRotation(FVector(-10.f, 0, 300.f), FRotator(FMath::RadiansToDegrees(FMath::Asin(0.1)) + 270.f, 0, 0));
	m_SpringArm->bEnableCameraLag = false;
	m_SpringArm->CameraLagSpeed = 10.f;
	m_SpringArm->TargetArmLength = 1200.f;

	m_Camera = CreateDefaultSubobject<UCameraComponent>("TopDownCamera");
	m_Camera->SetupAttachment(m_SpringArm, USpringArmComponent::SocketName);

	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	UStaticMesh* cubeMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;
	m_Mesh->SetStaticMesh(cubeMesh);
	m_Mesh->SetupAttachment(RootComponent);

	m_CurrentElements.Init(WizardElement::Fire, m_MaxElements);

	m_FirstElementBillboard = CreateDefaultSubobject<UBillboardComponent>("First Element");
	m_FirstElementBillboard->bHiddenInGame = false;
	m_FirstElementBillboard->SetupAttachment(RootComponent);
	m_FirstElementBillboard->SetRelativeLocation(FVector(0, -50, 100));
	m_FirstElementBillboard->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.25f));
	m_SecondElementBillboard = CreateDefaultSubobject<UBillboardComponent>("Second Element");
	m_SecondElementBillboard->bHiddenInGame = false;
	m_SecondElementBillboard->SetupAttachment(RootComponent);
	m_SecondElementBillboard->SetRelativeLocation(FVector(0, 50, 100));
	m_SecondElementBillboard->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.25f));

	if (m_FlameColumnClass == nullptr)
		UE_LOG(LogTemp, Warning, TEXT("FlameColumn not set on Wizard"));
	if (m_IceZoneClass == nullptr)
		UE_LOG(LogTemp, Warning, TEXT("IceZone not set on Wizard"));
	if (m_IceWallClass == nullptr)
		UE_LOG(LogTemp, Warning, TEXT("IceWall not set on Wizard"));
	if (m_TornadoClass == nullptr)
		UE_LOG(LogTemp, Warning, TEXT("Tornado not set on Wizard"));
	if (m_ChainLighningClass == nullptr)
		UE_LOG(LogTemp, Warning, TEXT("Chainlightning not set on Wizard"));
	if (m_ShockwaveClass == nullptr)
		UE_LOG(LogTemp, Warning, TEXT("Shockwave not set on Wizard"));

}

// Called when the game starts or when spawned
void AWizardCharacter::BeginPlay()
{
	Super::BeginPlay();
	m_FirstElementBillboard->SetSprite(m_FireElementTexture);
	m_SecondElementBillboard->SetSprite(m_FireElementTexture);

	for ( auto& pair : m_CooldownCounter)
	{
		m_CooldownCounter[pair.Key] = m_Cooldowns[pair.Key];
	}
}

void AWizardCharacter::TakeSpellDamage(float damage)
{
	m_Health -= damage * (1 + m_DamageTakenMultiplier/100.f);
	SpawnDamageText(damage * (1 + m_DamageTakenMultiplier / 100.f));
	CheckDeath();
}

void AWizardCharacter::TakeTickDamage(float damage)
{
	m_Health -= damage * (1 + m_DamageTakenMultiplier / 100.f);
	SpawnDamageText(damage * (1 + m_DamageTakenMultiplier / 100.f));
	CheckDeath();
}

// Called every frame
void AWizardCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_BasicAttackTimer += DeltaTime;

	APlayerController* controller = GetController<APlayerController>();
	if (controller == nullptr)
		return;

	controller->bShowMouseCursor = true;

	//m_Rotation += m_TurnSpeed * DeltaTime * m_TurnDirection;

	//FVector frontOfActor = GetActorLocation() + FRotator(0.f, m_Rotation, 0.f).Vector() * 50;

	//m_Mesh->SetRelativeRotation(FRotator(0.f, m_Rotation, 0.f));

	//FVector negativeFovBound = FRotator( 0.f, m_Rotation - m_FovAngle / 2.f, 0.f ).Vector();
	//FVector positiveFovBound = FRotator(0.f, m_Rotation + m_FovAngle / 2.f, 0.f).Vector();

	//DrawDebugLine(GetWorld(), frontOfActor, frontOfActor + negativeFovBound * 2000.f, FColor::Cyan, false, 0.02f, 1, 5.f);
	//DrawDebugLine(GetWorld(), frontOfActor, frontOfActor + positiveFovBound * 2000.f, FColor::Cyan, false, 0.02f, 1, 5.f);

	FVector mousePosWorld, mouseDirWorld;
	controller->DeprojectMousePositionToWorld(mousePosWorld, mouseDirWorld);
	mousePosWorld = FMath::RayPlaneIntersection(mousePosWorld, mouseDirWorld, FPlane(GetActorLocation(), FVector(0.f, 0.f, 1.f))); //screen position raycast to plane at actor height

	m_Mesh->SetRelativeRotation((mousePosWorld - GetActorLocation()).Rotation());
	
	/*float distanceToActor = FVector::Dist(mousePosWorld, frontOfActor);

	FVector crossOne = FVector::CrossProduct(negativeFovBound, mousePosWorld - frontOfActor);
	FVector crossTwo = FVector::CrossProduct(mousePosWorld - frontOfActor, positiveFovBound);*/

	/*if (crossOne.Z < 0 && crossTwo.Z < 0)
	{
		FVector mouseToActor = frontOfActor - mousePosWorld;

		FVector newPos = frontOfActor + mouseToActor;
		
		FVector2D screenPos;
		controller->ProjectWorldLocationToScreen(newPos, screenPos);

		controller->SetMouseLocation(screenPos.X, screenPos.Y);
	}
	else if (crossOne.Z < 0)
	{
		FVector newPos = frontOfActor + negativeFovBound * distanceToActor;

		FVector2D screenPos;
		controller->ProjectWorldLocationToScreen(newPos, screenPos);

		controller->SetMouseLocation(screenPos.X, screenPos.Y);
	}
	else if (crossTwo.Z < 0)
	{
		FVector newPos = frontOfActor + positiveFovBound * distanceToActor;

		FVector2D screenPos;
		controller->ProjectWorldLocationToScreen(newPos, screenPos);

		controller->SetMouseLocation(screenPos.X, screenPos.Y);
	}*/

	//cooldowns
	for (auto& pair : m_CooldownCounter)
	{
		m_CooldownCounter[pair.Key] += DeltaTime;
	}

	UpdatePowerups(DeltaTime);

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

TArray<int> AWizardCharacter::GetCurrentElements()
{
	TArray<int> elements{};

	for (auto element : m_CurrentElements)
	{
		elements.Add(int(element));
	}

	return elements;
}

TMap<int, float>& AWizardCharacter::GetCooldowns()
{
	return m_Cooldowns;
}

TMap<int, float>& AWizardCharacter::GetCooldownCounters()
{
	return m_CooldownCounter;
}

void AWizardCharacter::AddPowerUpEffect( UPowerUpEffect* effect)
{
	effect->Apply(this);
	m_PowerUpEffects.Add(effect);
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
	switch (m_CurrentElements[1])
	{
	case WizardElement::Fire:
		m_BaseDamageMultiplier -= m_DamagePerFire;
		break;
	case WizardElement::Frost:
		m_DamageTakenMultiplier += m_DamageReductionPerFrost;
		break;
	case WizardElement::Wind:
		GetCharacterMovement()->MaxWalkSpeed /= (1+m_SpeedPerWind/100.f);
		break;
	default:
		break;
	}

	m_CurrentElements.Pop();
	m_CurrentElements.Insert(element, 0);
	
	switch (m_CurrentElements[0])
	{
	case WizardElement::Fire:
		m_FirstElementBillboard->SetSprite(m_FireElementTexture);
		m_BaseDamageMultiplier += m_DamagePerFire;
		break;
	case WizardElement::Frost:
		m_FirstElementBillboard->SetSprite(m_FrostElementTexture);
		m_DamageTakenMultiplier -= m_DamageReductionPerFrost;
		break;
	case WizardElement::Wind:
		m_FirstElementBillboard->SetSprite(m_WindElementTexture);
		GetCharacterMovement()->MaxWalkSpeed *= (1 + m_SpeedPerWind / 100.f);
		break;
	}

	switch (m_CurrentElements[1])
	{
	case WizardElement::Fire:
		m_SecondElementBillboard->SetSprite(m_FireElementTexture);
		break;
	case WizardElement::Frost:
		m_SecondElementBillboard->SetSprite(m_FrostElementTexture);
		break;
	case WizardElement::Wind:
		m_SecondElementBillboard->SetSprite(m_WindElementTexture);
		break;
	}

	OnAddElement();
}

void AWizardCharacter::Fire(float input)
{
	if (input < 1.f)
		return;

	if (m_BasicAttackTimer > m_BasicAttackCooldown)
		m_BasicAttackTimer = 0;
	else
		return;


	APlayerController* controller = GetController<APlayerController>();

	FVector mousePosWorld, mouseDirWorld;
	controller->DeprojectMousePositionToWorld(mousePosWorld, mouseDirWorld);
	mousePosWorld = FMath::RayPlaneIntersection(mousePosWorld, mouseDirWorld, FPlane(GetActorLocation(), FVector(0.f, 0.f, 1.f))); //screen position raycast to plane at actor height

	FVector direction{ (mousePosWorld - GetActorLocation()).GetSafeNormal() };

	FActorSpawnParameters spawnParams{};
	spawnParams.Owner = this;
	spawnParams.Instigator = GetInstigator();

	auto actor = GetWorld()->SpawnActor<AActor>(m_BaseProjectile.Get(), spawnParams);
	auto projectile = Cast<ABaseProjectile>(actor);

	if (actor)
	{
		projectile->SetDamageMultiplier(m_BaseDamageMultiplier);
		projectile->SetActorScale3D(FVector(0.75f, 0.75f, 0.75f));
		projectile->SetActorLocationAndRotation(GetActorLocation(), direction.Rotation());
		projectile->FireInDirection(direction);

		projectile->SetInstigator(GetInstigator());
		projectile->SetOwner(this);

		if (m_ExplosiveBaseAttack)
		{
			projectile->SetExplosion(m_ExplosionRadius, m_ExplosionDamage);
		}
	}

	for (size_t i = 0; i < m_Spread; i++)
	{
		float angleOffset = ((i + 1) * 45.f / m_Spread);
		auto actorCW = GetWorld()->SpawnActor<AActor>(m_BaseProjectile.Get(), spawnParams);
		auto actorCCW = GetWorld()->SpawnActor<AActor>(m_BaseProjectile.Get(), spawnParams);
		auto projectileCW = Cast<ABaseProjectile>(actorCW);
		auto projectileCCW = Cast<ABaseProjectile>(actorCCW);

		projectileCW->SetInstigator(GetInstigator());
		projectileCW->SetOwner(this);

		projectileCCW->SetInstigator(GetInstigator());
		projectileCCW->SetOwner(this);

		projectileCW->SetActorScale3D(FVector(0.75f, 0.75f, 0.75f));
		projectileCCW->SetActorScale3D(FVector(0.75f, 0.75f, 0.75f));

		projectileCW->SetDamageMultiplier(m_BaseDamageMultiplier);
		projectileCCW->SetDamageMultiplier(m_BaseDamageMultiplier);

		auto rotation = direction.Rotation();
		auto rotationCW = rotation;
		rotationCW.Yaw += angleOffset;
		auto rotationCCW = rotation;
		rotationCCW.Yaw -= angleOffset;

		projectileCW->SetActorLocationAndRotation(GetActorLocation(), rotationCW);
		projectileCW->FireInDirection(rotationCW.Vector());

		projectileCCW->SetActorLocationAndRotation(GetActorLocation(), rotationCCW);
		projectileCCW->FireInDirection(rotationCCW.Vector());

		if (m_ExplosiveBaseAttack)
		{
			projectileCW->SetExplosion(m_ExplosionRadius, m_ExplosionDamage);
			projectileCCW->SetExplosion(m_ExplosionRadius, m_ExplosionDamage);
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

	for ( auto element : m_CurrentElements)
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

	if (m_CooldownCounter[spellID] < m_Cooldowns[spellID])
	{
		return;
	}

	m_CooldownCounter[spellID] = 0;

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
	Cast<UCharacterMovementComponent>(GetMovementComponent())->AddImpulse(GetMovementComponent()->Velocity.GetSafeNormal() * m_DashForce, true);
}

void AWizardCharacter::UpdatePowerups(float deltaTime)
{
	for (size_t i = 0; i < m_PowerUpEffects.Num(); i++)
	{
		m_PowerUpEffects[i]->Timer += deltaTime;
		if (m_PowerUpEffects[i]->Timer > m_PowerUpEffects[i]->Duration)
		{
			m_PowerUpEffects[i]->Remove(this);
			m_PowerUpEffects[i]->ConditionalBeginDestroy();
			m_PowerUpEffects.RemoveAt(i);
			i--;
		}
	}
}

void AWizardCharacter::CastFlameColumn(const FVector& worldPos)
{
	auto spell = GetWorld()->SpawnActor(m_FlameColumnClass);
	spell->SetOwner(this);
	spell->SetInstigator(GetInstigator());
	spell->SetActorLocation(worldPos);
}

void AWizardCharacter::CastIceZone(const FVector& worldPos)
{
	auto spell = GetWorld()->SpawnActor(m_IceZoneClass);
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
	auto spell = GetWorld()->SpawnActor(m_IceWallClass);
	spell->SetOwner(this);
	spell->SetInstigator(GetInstigator());
	spell->SetActorLocation(worldPos);
	spell->SetActorRotation( (worldPos - GetActorLocation()).Rotation() );
}

void AWizardCharacter::CastTornado(const FVector& direction)
{
	auto spell = GetWorld()->SpawnActor(m_TornadoClass);
	spell->SetOwner(this);
	spell->SetInstigator(GetInstigator());
	spell->SetActorLocation(GetActorLocation());
	Cast<ATornado>(spell)->FireInDirection(direction);
}

void AWizardCharacter::CastChainLightning(const FVector& direction)
{
	auto spell = GetWorld()->SpawnActor(m_ChainLighningClass);
	spell->SetOwner(this);
	spell->SetInstigator(GetInstigator());
	spell->SetActorLocation(GetActorLocation());
	Cast<AChainLightning>(spell)->FireInDirection(direction);
}

void AWizardCharacter::CastShockwave()
{
	auto spell = GetWorld()->SpawnActor(m_ShockwaveClass);
	spell->SetOwner(this);
	spell->SetInstigator(GetInstigator());
	spell->SetActorLocation(GetActorLocation());
}

