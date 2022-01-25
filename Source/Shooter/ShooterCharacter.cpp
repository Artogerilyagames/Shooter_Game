// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"


#include "Item.h"
#include "Weapon.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "Ammo.h"
#include "BulletHitInterface.h"
#include "Enemy.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
/*#include "Shooter.h"*/


// Sets default values
AShooterCharacter::AShooterCharacter() :
BaseTurnRate(45.f),
BaseLookUpRate(45.f),


HipTurnRate(90.f),
HipLookUpRate(90.f),
AimingTurnRate(20.f),
AimingLookUpRate(20.f),
MouseHipTurnRate(1.0f),
MouseHipLookUpRate(1.0f),
MouseAimingTurnRate(0.2f),
MouseAimingLookUpRate(0.2f),

bAiming(false),
//Camera spread factors
CameraDefaultFOV(0.f),
CameraZoomedFOV(35.f),
CameraCurrentFOV(0.f),
ZoomInterpSpeed(20.f),
// Crosshair spread factors
CrosshairSpreadMultiplier(0.f),
CrosshairVelocityFactor(0.f),
CrosshairAimFactor(0.f),
CrosshairShootingFactor(0.f),
//Bullet fire timer variables
ShootTimeDuration(0.05f),
bFiringBullet(false),
//Automatic gun fire variables

bFireButtonPressed(false),
bShouldFire(true),

bShouldTraceForItems(false),
Starting9mmAmmo(85),



StartingARAmmo(120),
CombatState(ECombatState::ECS_Unoccupied),
// Combat Variables
bCrosshairsTriger(false),
bCrouching(false),
bProne(false),
BaseMovementSpeed(650.f),
CrouchMovementSpeed(300.f),
ProneMovementSpeed(100),
StandingCapsuleHalfHeight(88.f),
CrouchingCapsuleHalfHeight(44.f),
ProneCapsuleHalfHeight(20),
BaseGroundFriction(2.f),
CrouchingGroundFriction(100.f),
bAimingButtonPressed(false),
bReloadingIcon(false),
bShouldPlayPickupSound(true),
bShouldPlayEquipSound(true),
PickupSoundResetTime(0.2f),
EquipSoundResetTime(0.2f),
Health(100.f),
MaxHealth(100.f)






{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SocketOffset = FVector(0.f, 80.f, 90.f);
	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(GetRootComponent());


	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	//Create Hand Scene Component
	HandSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HandSceneComp"));
	

	
}

float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if(Health - DamageAmount <= 0.f)
	{
		 Health = 0.f;
	}
	else
	{
		Health -= DamageAmount;
	}
	return DamageAmount;
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GetMesh()->HideBoneByName(TEXT("weapon_l"), EPhysBodyOp::PBO_None);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	GetMesh()->HideBoneByName(TEXT("ult_weapon_l"), EPhysBodyOp::PBO_None);
	GetMesh()->HideBoneByName(TEXT("ult_weapon_r"), EPhysBodyOp::PBO_None);
	GetMesh()->HideBoneByName(TEXT("ik_hand_gun"), EPhysBodyOp::PBO_None);
	GetMesh()->HideBoneByName(TEXT("grenade"), EPhysBodyOp::PBO_None);


	if(FollowCamera)
	{
		CameraDefaultFOV = GetFollowCamera()->FieldOfView;
		CameraCurrentFOV = CameraDefaultFOV;
	}
	EquipWeapon(SpawnDefaultWeapon());
	Inventory.Add(EquippedWeapon);
	EquippedWeapon->SetSlotIndex(0);
	InitializeAmmoMap();
	GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
	EquippedWeapon->SetCharacter(this);
	

	
	
}

void AShooterCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate* BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::Turn(float Value)
{
	
	if(bAiming)
	{
		TurnScaleFactor = MouseAimingTurnRate;
	}
	else
	{
		TurnScaleFactor = MouseHipTurnRate;
	}
	AddControllerYawInput(Value* TurnScaleFactor);
}

void AShooterCharacter::LookUp(float Value)
{
	
	if(bAiming)
	{
		LookUpScaleFactor = MouseAimingLookUpRate;
		bReloadingIcon = true;
	}
	else
	{
		LookUpScaleFactor = MouseHipLookUpRate;
	}
	AddControllerPitchInput(Value* LookUpScaleFactor);
}

void AShooterCharacter::FireWeapon() 
{
	if(EquippedWeapon == nullptr) return;
	if(CombatState != ECombatState::ECS_Unoccupied) return;
	
	if(WeaponHasAmmo())
	{
		
		PlayFireSound();
		SendBullet();
		PlayGunfireMontage();
		EquippedWeapon->DecrementAmmo();

		StartFireTimer();
		if(EquippedWeapon->GetWeaponType() == EWeaponType::EWT_Pistol)
		{
			EquippedWeapon->StartSlideTimer();
		}
	}

	

	

	StartCrosshairBulletFire();
}

void AShooterCharacter::Punch()
{
	if(PunchSound)
	{
		UGameplayStatics::PlaySound2D(this, PunchSound);
	}
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && PunchMontage)
	{
		AnimInstance->Montage_Play(PunchMontage);
		AnimInstance->Montage_JumpToSection(FName("Punch"));
	}
	
}


bool AShooterCharacter::GetBeamEndLocation
(const FVector& MuzzleSocketLocation, FHitResult& OutHitResult) 
{
	FVector OutBeamLocation;
	FHitResult CrosshairHitResult;
	bool bCrosshairHit = TraceUnderCrosshairs(CrosshairHitResult, OutBeamLocation);
	if(bCrosshairHit)
	{
		OutBeamLocation = CrosshairHitResult.Location;
	}
	else //no crosshair trace hit
		{
		// Out Beam location is the end location fot the line trace
		}
	// Perform a second trace this time from the gun barrel
	
	const FVector WeaponTraceStart{MuzzleSocketLocation};
	const FVector WeaponTraceEnd{OutBeamLocation};
	GetWorld()->LineTraceSingleByChannel(
		OutHitResult,
		WeaponTraceStart,
		WeaponTraceEnd,
		ECollisionChannel::ECC_Visibility);
	if(!OutHitResult.bBlockingHit)
	{
		OutHitResult.Location = OutBeamLocation;
		return false;
	}
	return true;
}
	

void AShooterCharacter::AimingButtonPressed()
{
	bAimingButtonPressed = true;

	if(CombatState != ECombatState::ECS_Reloading && CombatState != ECombatState::ECS_Equipping && CombatState !=ECombatState::ECS_Stunned)
	{
		ShowCrosshairs();
		Aim();
	}
}

void AShooterCharacter::AimingButtonReleased()
{
	bAimingButtonPressed = false;
	StopAiming();
	HideCrosshairs();
}

void AShooterCharacter::CameraInterpZoom(float DeltaTime)
{
	if(bAiming)
	{
		CameraCurrentFOV = FMath::FInterpTo
		(CameraCurrentFOV,
			CameraZoomedFOV,
			DeltaTime,
			ZoomInterpSpeed);
	}
	else
	{
		CameraCurrentFOV = FMath::FInterpTo
		(CameraCurrentFOV,
		CameraDefaultFOV,
		DeltaTime,
		ZoomInterpSpeed);
	}
	GetFollowCamera()->SetFieldOfView(CameraCurrentFOV);

	
}

void AShooterCharacter::SetLookRates()
{
	if(bAiming)
	{
		BaseTurnRate = AimingTurnRate;
		BaseLookUpRate = AimingLookUpRate;
	}
	else
	{
		BaseTurnRate = HipTurnRate;
		BaseLookUpRate = HipLookUpRate;
	}
}

void AShooterCharacter::CalculateCrosshairSpread(float DeltaTime)
{
	FVector2D WalkSpeedRange{0.f, 600.f};
	FVector2D VelocityMultiplierRange{0.f, 1.f};
	FVector Velocity{GetVelocity()};
	Velocity.Z = 0.f;
	CrosshairVelocityFactor= FMath::GetMappedRangeValueClamped
	(WalkSpeedRange,
		VelocityMultiplierRange,
		Velocity.Size());
	
	if(GetCharacterMovement()->IsFalling())
	{
		CrosshairInAirFactor = FMath::FInterpTo
		(CrosshairInAirFactor,
			2.25f,
			DeltaTime,
			2.25f);
	}
	else
	{
		CrosshairInAirFactor = FMath::FInterpTo
	(CrosshairInAirFactor,
		0.f,
		DeltaTime,
		30.f);
	}
	if(bAiming)
	{
		CrosshairAimFactor = FMath::FInterpTo
		(CrosshairAimFactor,
			0.6f,
			DeltaTime,
			30.f);
	}
	else // Not Aiming
	{
		CrosshairAimFactor = FMath::FInterpTo
		(CrosshairAimFactor,
			0.f,
			DeltaTime,
			30.f);
	}
	// True 0.05 second after firing
	if(bFiringBullet)
	{
		CrosshairShootingFactor = FMath::FInterpTo
		(CrosshairShootingFactor,
			0.3f,
			DeltaTime,
			60.f);
	}
	else
	{
		CrosshairShootingFactor = FMath::FInterpTo
		(CrosshairAimFactor,
			0.f,
			DeltaTime,
60.f);
	}


	
	CrosshairSpreadMultiplier =
		0.5f +
			CrosshairVelocityFactor +
				CrosshairInAirFactor -
					CrosshairAimFactor +
					CrosshairShootingFactor;
}

void AShooterCharacter::FireButtonPressed()
{
	bFireButtonPressed = true;
	FireWeapon();
	ShowCrosshairs();
	

	
	
}

void AShooterCharacter::FireButtonReleased()
{
	bFireButtonPressed = false;
	if(bAiming)
	{
		ShowCrosshairs();
	}
	else
	{
		HideCrosshairs();
	}
	
	
}

void AShooterCharacter::StartFireTimer()
{
	if(EquippedWeapon == nullptr) return;
	CombatState = ECombatState::ECS_FireTimerInProgress;
	
		GetWorldTimerManager().SetTimer
		(AutoFireTimer,
			this,
			&AShooterCharacter::AutoFireReset,
			EquippedWeapon->GetAutoFireRate());
	
}

void AShooterCharacter::AutoFireReset()
{
	if(CombatState == ECombatState::ECS_Stunned) return;
	
	CombatState = ECombatState::ECS_Unoccupied;
	if(EquippedWeapon == nullptr) return;
	if(WeaponHasAmmo())
	{
		if(bFireButtonPressed && EquippedWeapon->GetAutomatic())
		{
			FireWeapon();
		}
	}
	else
	{
		ReloadWeapon();
	}

	
}

bool AShooterCharacter::TraceUnderCrosshairs(FHitResult& OutHitResult, FVector& OutHitLocation) const
{
	FVector2D ViewportSize;
	if(GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}
	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 1.6f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController
	(this, 0),
	CrosshairLocation,
	CrosshairWorldPosition,
	CrosshairWorldDirection);
	if(bScreenToWorld)
	{
		const FVector Start{CrosshairWorldPosition};
		const FVector End{Start + CrosshairWorldDirection * 50'000.f};
		OutHitLocation = End;
		
		GetWorld()->LineTraceSingleByChannel
		(OutHitResult,
			Start,
			End,
			ECollisionChannel::ECC_Visibility);
		if(OutHitResult.bBlockingHit)
		{
			OutHitLocation = OutHitResult.Location;
			return true;
		}
	}
	return false;
}

void AShooterCharacter::TraceForItems()
{
	if(bShouldTraceForItems)
	{
		FHitResult ItemTraceResult;
		FVector HitLocation;
		TraceUnderCrosshairs(ItemTraceResult, HitLocation);
		if(ItemTraceResult.bBlockingHit)
		{
			TraceHitItem = Cast<AItem>(ItemTraceResult.Actor);
			if(TraceHitItem && TraceHitItem->GetItemState()== EItemState::EIS_EquipInterping)
			{
				TraceHitItem = nullptr;
			}
			if(TraceHitItem && TraceHitItem->GetPickupWidget())
			{
				TraceHitItem->GetPickupWidget()->SetVisibility(true);
				TraceHitItem->EnableCustomDepth();
				if(Inventory.Num() >= INVENTORY_CAPACITY)
				{
					TraceHitItem->SetCharacterInventoryFull(true);
				}
				else
				{
					TraceHitItem->SetCharacterInventoryFull(false);
				}
					
			}
			if(TraceHitItemLastFrame)
			{
				if(TraceHitItem != TraceHitItemLastFrame)
				{
					TraceHitItemLastFrame->GetPickupWidget()->SetVisibility(false);
					TraceHitItemLastFrame->DisableCustomDepth();
				}
			}
			//Store a reference to HitItem for next frame
			TraceHitItemLastFrame = TraceHitItem;
		}
		
	}
	else if (TraceHitItemLastFrame)
	{
		TraceHitItemLastFrame->GetPickupWidget()->SetVisibility(false);
		TraceHitItemLastFrame->DisableCustomDepth();
	}
}

void AShooterCharacter::ReloadingIcon()
{
	bReloadingIcon = true;
}

void AShooterCharacter::UnreloadingIcon()
{
	bReloadingIcon = false;
}

AWeapon* AShooterCharacter::SpawnDefaultWeapon()
{
	if(DefaultWeaponClass)
	{
	 return  GetWorld()->SpawnActor<AWeapon>(DefaultWeaponClass);
	}
	return nullptr;
}

void AShooterCharacter::EquipWeapon(AWeapon* WeaponToEquip, bool bSwapping)
{
	if(WeaponToEquip)
	{
		const USkeletalMeshSocket* HandSocket = GetMesh()->GetSocketByName
		(FName("RightHandSocket"));
		if(HandSocket)
		{
			HandSocket->AttachActor(WeaponToEquip, GetMesh());
		}
		if(EquippedWeapon == nullptr)
		{
			// -1 == no EquippedWeapon yet. no need to reverse the icon animation
			EquipItemDelegate.Broadcast(-1,WeaponToEquip->GetSlotIndex());
		}
		else if (!bSwapping)
		{
			EquipItemDelegate.Broadcast(EquippedWeapon->GetSlotIndex(), WeaponToEquip->GetSlotIndex());
		}
		EquippedWeapon = WeaponToEquip;
		EquippedWeapon->SetItemState(EItemState::EIS_Equipped);
	}
}

void AShooterCharacter::StartCrosshairBulletFire()
{
	
	bFiringBullet =true;
	GetWorldTimerManager().SetTimer
	(CrosshairShootTimer,
		this,
		&AShooterCharacter::FinishCrosshairBulletFire,
		ShootTimeDuration);
}

void AShooterCharacter::DropWeapon()
{
	if(EquippedWeapon)
	{
		FDetachmentTransformRules DetachementTransformRules(EDetachmentRule::KeepWorld, true);
		EquippedWeapon->GetItemMesh()->DetachFromComponent(DetachementTransformRules);
		EquippedWeapon->SetItemState(EItemState::EIS_Falling);
		EquippedWeapon->ThrowWeapon();
	}
}

void AShooterCharacter::SelectButtonPressed()
{
	if(CombatState != ECombatState::ECS_Unoccupied) return;
	if(TraceHitItem)
		
	{
		TraceHitItem->StartItemCurve(this ,true);
		TraceHitItem = nullptr;
		
	}
}

void AShooterCharacter::SelectButtonReleased()
{

}

void AShooterCharacter::FinishCrosshairBulletFire()
{
	bFiringBullet =false;
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CameraInterpZoom(DeltaTime);
	SetLookRates();
	CalculateCrosshairSpread(DeltaTime);
	TraceForItems();
	InterpCapsuleHalfHeight(DeltaTime);
	
	
	

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis("TurnRate", this, &AShooterCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AShooterCharacter::LookUpAtRate);
	PlayerInputComponent->BindAxis("Turn", this, &AShooterCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AShooterCharacter::LookUp);
	
	
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this,  &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AShooterCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("FireButton", IE_Pressed, this, &AShooterCharacter::FireButtonPressed);
	PlayerInputComponent->BindAction("FireButton", IE_Released, this, &AShooterCharacter::FireButtonReleased);

	PlayerInputComponent->BindAction("Attack1", IE_Pressed, this, &AShooterCharacter::Punch);

	PlayerInputComponent->BindAction("AimingButton", IE_Pressed, this, &AShooterCharacter::AimingButtonPressed);
	PlayerInputComponent->BindAction("AimingButton", IE_Released, this, &AShooterCharacter::AimingButtonReleased);
	
	PlayerInputComponent->BindAction("Select", IE_Pressed, this, &AShooterCharacter::SelectButtonPressed);
	PlayerInputComponent->BindAction("Select", IE_Released, this, &AShooterCharacter::SelectButtonReleased);
	
	PlayerInputComponent->BindAction("ReloadButton", IE_Pressed, this, &AShooterCharacter::ReloadButtonPressed);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AShooterCharacter::CrouchButtonPressed);
	PlayerInputComponent->BindAction("Prone", IE_Pressed, this, &AShooterCharacter::ProneButtonPressed);

	PlayerInputComponent->BindAction("FKey", IE_Pressed, this, &AShooterCharacter::FKeyPressed);
	PlayerInputComponent->BindAction("1Key", IE_Pressed, this, &AShooterCharacter::OneKeyPressed);
	PlayerInputComponent->BindAction("2Key", IE_Pressed, this, &AShooterCharacter::TwoKeyPressed);
	PlayerInputComponent->BindAction("3Key", IE_Pressed, this, &AShooterCharacter::ThreeKetPressed);
	PlayerInputComponent->BindAction("4Key", IE_Pressed, this, &AShooterCharacter::FourKeyPressed);
	PlayerInputComponent->BindAction("5Key", IE_Pressed, this, &AShooterCharacter::FiveKeyPressed);






	
}

void AShooterCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation{Controller->GetControlRotation()};
		const FRotator YawRotation{0, Rotation.Yaw, 0};
		const FVector Direction{FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::X)};
		AddMovementInput(Direction, Value);
		
	}
}

void AShooterCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation{Controller->GetControlRotation()};
		const FRotator YawRotation{0, Rotation.Yaw, 0};
		const FVector Direction{FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::Y)};
		AddMovementInput(Direction, Value);
		
	}
}

void AShooterCharacter::SwapWeapon(AWeapon* WeaponToSwap)
{
	if(Inventory.Num() - 1 >= EquippedWeapon->GetSlotIndex())
	{
		Inventory[EquippedWeapon->GetSlotIndex()] = WeaponToSwap;
		WeaponToSwap->SetSlotIndex(EquippedWeapon->GetSlotIndex());
	}
	DropWeapon();
	EquipWeapon(WeaponToSwap, true);
	TraceHitItem =nullptr;
	TraceHitItemLastFrame = nullptr;
}

void AShooterCharacter::InitializeAmmoMap()
{
	AmmoMap.Add(EAmmoType::EAT_9mm, Starting9mmAmmo);
	AmmoMap.Add(EAmmoType::EAT_AR, StartingARAmmo);
}

bool AShooterCharacter::WeaponHasAmmo()
{
	if(EquippedWeapon == nullptr) return false;
	return EquippedWeapon->GetAmmo()> 0;
	
}

void AShooterCharacter::PlayFireSound() const
{
	//Play Fire Sound
	if(EquippedWeapon->GetFireSound())
	{
		UGameplayStatics::PlaySound2D(this, EquippedWeapon->GetFireSound());
	}
}

void AShooterCharacter::SendBullet()
{
	// Send Bullet
	const USkeletalMeshSocket* BarrelSocket =
		EquippedWeapon->GetItemMesh()->GetSocketByName("BarrelSocket");
	if(BarrelSocket)
	{
		const FTransform SocketTransform = BarrelSocket->GetSocketTransform
		(EquippedWeapon->GetItemMesh());
		if(EquippedWeapon->GetMuzzleFlash())
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EquippedWeapon->GetMuzzleFlash(), SocketTransform);
		}
		FHitResult BeamHitResult;
		bool bBeamEnd = GetBeamEndLocation(SocketTransform.GetLocation(), BeamHitResult);
		if (bBeamEnd)
		{
			// Does hit actor implement BulletHitInterface?
			if(BeamHitResult.Actor.IsValid())
			{
				IBulletHitInterface* BulletHitInterface = Cast<IBulletHitInterface>(BeamHitResult.Actor.Get());
				if (BulletHitInterface)
				{
					BulletHitInterface->BulletHit_Implementation(BeamHitResult);
				}
				AEnemy* HitEmemy = Cast<AEnemy>(BeamHitResult.Actor.Get());
				if(HitEmemy)
				{
					if(BeamHitResult.BoneName.ToString() == HitEmemy->GetHeadBone())
					{
						// Head shot
						UGameplayStatics::ApplyDamage
					(BeamHitResult.Actor.Get(),
						EquippedWeapon->GetHeadShotDamage(),
						GetController(),
						this,
						UDamageType::StaticClass());
					}
					else 
					{
						// Body Shot
						UGameplayStatics::ApplyDamage
					(BeamHitResult.Actor.Get(),
						EquippedWeapon->GetDamage(),
						GetController(),
						this,
						UDamageType::StaticClass());
						//UE_LOG(LogTemp, Warning, TEXT("Hit Component: %s"), *BeamHitResult.BoneName.ToString());
					}
					
					
				}
			}
			else
			{
				//Spawn default particles
				if(ImpactParticles)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
					ImpactParticles,
					BeamHitResult.Location);
				}
			}
			
			UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BeamParticles, SocketTransform);
			if(Beam)
			{
				Beam->SetVectorParameter(FName("Target"), BeamHitResult.Location);
			}
		}
	}
}

void AShooterCharacter::PlayGunfireMontage()
{
	// Play Hip Fire Montage
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && HipFireMontage)
	{
		AnimInstance->Montage_Play(HipFireMontage);
		AnimInstance->Montage_JumpToSection(FName("StartFire"));
	}

}

void AShooterCharacter::ReloadButtonPressed()
{
	ReloadWeapon();
	
}

void AShooterCharacter::ReloadWeapon()
{
	
	if(CombatState != ECombatState::ECS_Unoccupied) return;
	
	if(EquippedWeapon == nullptr) return;
	
	if(CarryingAmmo() && !EquippedWeapon->ClipIsFull())
	{
		if (bAiming)
		{
			StopAiming();
			
		}

		CombatState = ECombatState::ECS_Reloading;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if(AnimInstance && ReloadMontage)
		{
			AnimInstance->Montage_Play(ReloadMontage);
			AnimInstance->Montage_JumpToSection
			(EquippedWeapon->GetReloadMontageSection());
		}
	}
}

bool AShooterCharacter::CarryingAmmo()
{
	if(EquippedWeapon == nullptr) return false;
	auto AmmoType = EquippedWeapon->GetAmmoType();
	if(AmmoMap.Contains(AmmoType))
	{
		return AmmoMap[AmmoType] > 0;
	}
	return false;
}

void AShooterCharacter::ShowCrosshairs()
{
   bCrosshairsTriger = true;

}

void AShooterCharacter::HideCrosshairs()
{
	bCrosshairsTriger = false;
}

void AShooterCharacter::GrabClip() 
{
	if(EquippedWeapon == nullptr) return;
	if(HandSceneComponent == nullptr) return;
	const int32 ClipBoneIndex{EquippedWeapon->GetItemMesh()->GetBoneIndex(EquippedWeapon->GetClipBoneName())};
	ClipTransform = EquippedWeapon->GetItemMesh()->GetBoneTransform(ClipBoneIndex);
	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, true);
	HandSceneComponent->AttachToComponent(GetMesh(), AttachmentRules,FName(TEXT("hand_l")));
	HandSceneComponent->SetWorldTransform(ClipTransform);

	EquippedWeapon->SetMovingClip(true);
}

void AShooterCharacter::ReleasedClip()
{
	EquippedWeapon->SetMovingClip(false);
}

void AShooterCharacter::CrouchButtonPressed()
{
	if(!GetCharacterMovement()->IsFalling())
	{
		bCrouching = !bCrouching;
	}
	if(bCrouching)
	{
		GetCharacterMovement()->MaxWalkSpeed = CrouchMovementSpeed;
		GetCharacterMovement()->GroundFriction = CrouchingGroundFriction;
		
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
		GetCharacterMovement()->GroundFriction = BaseGroundFriction;
	}
}

void AShooterCharacter::ProneButtonPressed()
{
	if(!GetCharacterMovement()->IsFalling())
	{
		bProne = !bProne;
	}
	if (bProne)
	{
		GetCharacterMovement()->MaxWalkSpeed = ProneMovementSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
	}
}

void AShooterCharacter::InterpCapsuleHalfHeight(float DeltaTime) const
{
	
	float TargetCapsuleHalfHeight;
	if(bCrouching || bProne)
	{
		TargetCapsuleHalfHeight = CrouchingCapsuleHalfHeight;
	}
	else
	{
		TargetCapsuleHalfHeight = StandingCapsuleHalfHeight;
	}
	const float InterpHalfHeight{FMath::FInterpTo(GetCapsuleComponent()->GetScaledCapsuleHalfHeight(),TargetCapsuleHalfHeight,DeltaTime,20.f)};
	const float DeltaCapsuleHalfHeight{InterpHalfHeight - GetCapsuleComponent()->GetScaledCapsuleHalfHeight()};
	const FVector MeshOffset{0.f, 0.f, - DeltaCapsuleHalfHeight};
	GetMesh()->AddLocalOffset(MeshOffset);
	GetCapsuleComponent()->SetCapsuleHalfHeight(InterpHalfHeight);
}


void AShooterCharacter::Jump()
{
	if(bCrouching || bProne)
	{
		bCrouching = false;
		bProne = false;
		
		GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
	}
	else
	{
		ACharacter::Jump();
	}
}

void AShooterCharacter::Aim()
{
	bAiming = true;
    ShowCrosshairs();
	GetCharacterMovement()->MaxWalkSpeed = CrouchMovementSpeed;
	
}

void AShooterCharacter::StopAiming()
{
	bAiming = false;
	
	if(!bCrouching)
	{
		GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
		HideCrosshairs();
	}
	
	
}

void AShooterCharacter::PickupAmmo(AAmmo* Ammo)
{

	
	if(AmmoMap.Find(Ammo->GetAmmoType()))
	{
		int32 AmmoCount{AmmoMap[Ammo->GetAmmoType()]};
		AmmoCount += Ammo->GetItemCount();
		AmmoMap[Ammo->GetAmmoType()] = AmmoCount;
	}
	if(EquippedWeapon->GetAmmoType() == Ammo->GetAmmoType())
	{
		if(EquippedWeapon->GetAmmo() == 0)
		{
			ReloadWeapon();
		}
	}
	Ammo->Destroy();
}

void AShooterCharacter::ResetPickupSoundTimer()
{
	bShouldPlayPickupSound = true;
}

void AShooterCharacter::ResetEquipSoundTimer()
{
	bShouldPlayEquipSound = true;
}



void AShooterCharacter::FKeyPressed()
{
	if(EquippedWeapon->GetSlotIndex()== 0) return;
	ExchangeInventoryItems(EquippedWeapon->GetSlotIndex(), 0);
}

void AShooterCharacter::OneKeyPressed()
{
	if(EquippedWeapon->GetSlotIndex()== 1) return;
	ExchangeInventoryItems(EquippedWeapon->GetSlotIndex(), 1);
}

void AShooterCharacter::TwoKeyPressed()
{
	if(EquippedWeapon->GetSlotIndex()== 2) return;
	ExchangeInventoryItems(EquippedWeapon->GetSlotIndex(), 2);
}

void AShooterCharacter::ThreeKetPressed()
{
	if(EquippedWeapon->GetSlotIndex()== 3) return;
	ExchangeInventoryItems(EquippedWeapon->GetSlotIndex(), 3);
}

void AShooterCharacter::FourKeyPressed()
{
	if(EquippedWeapon->GetSlotIndex()== 4) return;
	ExchangeInventoryItems(EquippedWeapon->GetSlotIndex(), 4);
}

void AShooterCharacter::FiveKeyPressed()
{
	if(EquippedWeapon->GetSlotIndex()== 5) return;
	ExchangeInventoryItems(EquippedWeapon->GetSlotIndex(), 5);
}



EPhysicalSurface AShooterCharacter::GetSurfaceType()
{
	FHitResult HitResult;
	const FVector Start{GetActorLocation()};
	const FVector End{Start + FVector(0.f, 0.f, -400.f)};
	FCollisionQueryParams QueryParams;
	QueryParams.bReturnPhysicalMaterial = true;
	GetWorld()->LineTraceSingleByChannel
	(HitResult,
		Start,
		End,
		ECollisionChannel::ECC_Visibility,
		QueryParams);
	return UPhysicalMaterial::DetermineSurfaceType(HitResult.PhysMaterial.Get());
	
	


}

void AShooterCharacter::EndStun()
{
	CombatState = ECombatState::ECS_Unoccupied;
	if(bAimingButtonPressed)
	{
		Aim();
	}
}


void AShooterCharacter::ExchangeInventoryItems(int32 CurrentItemIndex, int32 NewItemIndex)
{
	const bool bCanExchangeItems =
		(CurrentItemIndex != NewItemIndex) &&
		(NewItemIndex < Inventory.Num()) &&
			(CombatState == ECombatState::ECS_Unoccupied || CombatState == ECombatState::ECS_Equipping);

		if(bCanExchangeItems)
	{
			if(bAiming)
			{
				StopAiming();
			}
		const auto OldEquippedWeapon = EquippedWeapon;
		const auto NewWeapon = Cast<AWeapon>(Inventory[NewItemIndex]);
		EquipWeapon(NewWeapon);

		OldEquippedWeapon->SetItemState(EItemState::EIS_PickUp);
		NewWeapon->SetItemState(EItemState::EIS_Equipped);
		CombatState = ECombatState::ECS_Equipping;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if(AnimInstance && EquipMontage)
		{
			AnimInstance->Montage_Play(EquipMontage, 1.0f);
			AnimInstance->Montage_JumpToSection(FName("Equip"));
		}
		NewWeapon->PlayEquipSound(true);
	}
	
	
}



void AShooterCharacter::FinishReloading()
{
	if(CombatState == ECombatState::ECS_Stunned) return;
	
	CombatState = ECombatState::ECS_Unoccupied;
	if(bAimingButtonPressed)
	{
		Aim();
	}
	if(EquippedWeapon == nullptr) return;
	const auto AmmoType{EquippedWeapon->GetAmmoType()};
	if(AmmoMap.Contains(EquippedWeapon->GetAmmoType()))
	{
		int32 CarriedAmmo = AmmoMap[EquippedWeapon->GetAmmoType()];
		const int32 MagEmptySpace =
			EquippedWeapon->GetMagazineCapacity()
		- EquippedWeapon->GetAmmo();
		if(MagEmptySpace > CarriedAmmo)
		{
			EquippedWeapon->ReloadAmmo(CarriedAmmo);
			CarriedAmmo = 0;
			AmmoMap.Add(AmmoType, CarriedAmmo);
		}
		else
		{
			EquippedWeapon->ReloadAmmo(MagEmptySpace);
			CarriedAmmo -= MagEmptySpace;
			AmmoMap.Add(AmmoType, CarriedAmmo);
		}
	}
}

void AShooterCharacter::FinishEquipping()
{
	if(CombatState == ECombatState::ECS_Stunned) return;
	CombatState = ECombatState::ECS_Unoccupied;
	if(bAimingButtonPressed)
	{
		Aim();
	}
}

float AShooterCharacter::GetCrosshairSpreadmultiplier() const
{
	return CrosshairSpreadMultiplier;
}

void AShooterCharacter::IncrementOverlappedItemCount(int8 Amount)
{
	if(OverlappedItemCount + Amount <= 0 )
	{
		OverlappedItemCount = 0;
		bShouldTraceForItems = false;
	}
	else
	{
		OverlappedItemCount += Amount;
		bShouldTraceForItems = true;
	}
}

FVector AShooterCharacter::GetCameraInterpLocation()
{
	// ReSharper disable once CppDeclaratorNeverUsed
	const FVector CameraWorldLocation{FollowCamera->GetComponentLocation()};
	// ReSharper disable once CppDeclaratorNeverUsed
	const FVector CameraForward{FollowCamera->GetForwardVector()};
	return {};
}

void AShooterCharacter::StartPickupSoundTimer()
{
	bShouldPlayPickupSound = false;
	GetWorldTimerManager().SetTimer
	(PickupSoundTimer,
		this,
		&AShooterCharacter::ResetPickupSoundTimer,
		PickupSoundResetTime);
}

void AShooterCharacter::StartEquipSoundTimer()
{
	bShouldPlayEquipSound = false;
	GetWorldTimerManager().SetTimer
	(PickupSoundTimer,
		this,
		&AShooterCharacter::ResetEquipSoundTimer,
		EquipSoundResetTime);
}

void AShooterCharacter::GetPickupItem(AItem* Item)
{
	Item->PlayEquipSound();
	auto Weapon = Cast<AWeapon>(Item);
	if(Weapon)
	{
		if(Inventory.Num() < INVENTORY_CAPACITY)
		{
			Weapon->SetSlotIndex(Inventory.Num());
			Inventory.Add(Weapon);
			Weapon->SetItemState(EItemState::EIS_PickUp);
		}
		else // Inventory is full! Swap with Equipped Weapon
			{
			SwapWeapon(Weapon);
			}
	}
	
	auto Ammo = Cast<AAmmo>(Item);
	if(Ammo)
	{
		PickupAmmo(Ammo);
		
	}
}

