// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAnimInstance.h"
#include "ShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon.h"
/*#include "WeaponType.h"*/

UShooterAnimInstance::UShooterAnimInstance() :
Speed(0.f),
bIsInAir(false),
bIsAccelerating(false),
MovementOffsetYaw(0.f),
LastMovementOffsetYaw(0.f),
bAiming(false),

TIPCharacterYaw(0.f),

TIPCharacterYawLastFrame(0.f),

RootYawOffset(0.f),
Pitch(0.f),

bReloading(false),
OffsetState(EOffsetState::EOS_Hip),
CharacterRotation(FRotator(0.f)),
CharacterRotationLastFrame(FRotator(0.f)),
YawDelta(0.f),
RecoilWeight(1.0),
bTurningInPlace(false),
EquippedWeaponType(EWeaponType::EWT_MAX),
bShouldUseFABRIK(false)
{
	
}

void UShooterAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if(ShooterCharacter == nullptr)
	{
		ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
	}
	if(ShooterCharacter)
	{
		bProne = ShooterCharacter->GetProne();
		bCrouching = ShooterCharacter->GetCrouching();
		bReloading = ShooterCharacter->GetCombatState() == ECombatState::ECS_Reloading;
		bEquipping = ShooterCharacter->GetCombatState() == ECombatState::ECS_Equipping;
		bShouldUseFABRIK = ShooterCharacter->GetCombatState() ==
			ECombatState::ECS_Unoccupied|| ShooterCharacter->GetCombatState() ==
			ECombatState::ECS_FireTimerInProgress;
		
		FVector Velocity{ShooterCharacter->GetVelocity()};
		Velocity.Z = 0;
		Speed = Velocity.Size();
		bIsInAir = ShooterCharacter->GetCharacterMovement()->IsFalling();
		if(ShooterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f)
		{
			bIsAccelerating = true;
		}
		else
		{
			bIsAccelerating = false;
		}
		FRotator AimRotation = ShooterCharacter->GetBaseAimRotation();
		FRotator MovementRotation =
			UKismetMathLibrary::MakeRotFromX
		(ShooterCharacter->GetVelocity());
		MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator
		(MovementRotation,
			AimRotation).Yaw;
		if(ShooterCharacter->GetVelocity().Size() > 0.f)
		{
			LastMovementOffsetYaw = MovementOffsetYaw;
		}
		bAiming = ShooterCharacter->GetAiming();
		
		if (bReloading)
		{
			OffsetState = EOffsetState::EOS_Reloading;
		}
		else if (bIsInAir)
		{
			OffsetState = EOffsetState::EOS_InAir;
		}
		else if(ShooterCharacter->GetAiming())
		{
			OffsetState = EOffsetState::EOS_Aiming;
		}
		else
		{
			OffsetState = EOffsetState::EOS_Hip;
		}
		if(ShooterCharacter->GetEquippedWeapon())
		{
			EquippedWeaponType = ShooterCharacter->GetEquippedWeapon()->GetWeaponType();
		}
			
	}
	TurnInPlace();
	Lean(DeltaTime);
}

void UShooterAnimInstance::NativeInitializeAnimation()
{
	ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
}

void UShooterAnimInstance::TurnInPlace()
{
	if(ShooterCharacter == nullptr) return;
	Pitch = ShooterCharacter->GetBaseAimRotation().Pitch;
	if(Speed > 0 || bIsInAir)
	{
		// Dont want to Turn in Place; Character is moving
		RootYawOffset = 0.f;
		TIPCharacterYaw =  ShooterCharacter->GetActorRotation().Yaw;
		TIPCharacterYawLastFrame = TIPCharacterYaw;
		RotationCurveLastFrame = 0.f;
		RotationCurve = 0.f;
	}
	else
	{
		TIPCharacterYawLastFrame = TIPCharacterYaw;
		TIPCharacterYaw = ShooterCharacter->GetActorRotation().Yaw;
		const float TIPYawDelta{TIPCharacterYaw - TIPCharacterYawLastFrame };
		// Root Yaw Offset, Update and clamped to [-180,180]
		RootYawOffset = UKismetMathLibrary::NormalizeAxis(RootYawOffset - TIPYawDelta);

		const float Turning{GetCurveValue(TEXT("Turning"))};
		if(Turning > 0)
		{
			bTurningInPlace = false;
			RotationCurveLastFrame = RotationCurve;
			RotationCurve = GetCurveValue(TEXT("CurveRotation"));
			// ReSharper disable once CppDeclaratorNeverUsed
			const float DeltaRotation{RotationCurve - RotationCurveLastFrame};

			RootYawOffset > 0 ?  RootYawOffset -= DeltaRotation : RootYawOffset += DeltaRotation;
			const float ABSRootYawOffset{FMath::Abs(RootYawOffset)};
			if(ABSRootYawOffset > 90.f)
			{
				const float YawExcess{ABSRootYawOffset - 90.f};
				RootYawOffset > 0? RootYawOffset -= YawExcess : RootYawOffset += YawExcess;
			}
		}
		
		//if(GEngine) GEngine->AddOnScreenDebugMessage(1, -1, FColor::Cyan, FString::Printf(TEXT("")));
		else
		{
			bTurningInPlace = true;
		}
	}
	//Set the Recoil Weight
	if(bTurningInPlace)
	{
		if(bReloading || bEquipping)
		{
			RecoilWeight = 1.f;
		}
		else
		{
			RecoilWeight = 0.1f;
		}
	}
	else //not Turning in Place
	{
		if(bCrouching || bEquipping)
		{
			if(bReloading)
			{
				RecoilWeight = 1.f;
			}
			else
			{
				RecoilWeight = 0.f;
			}
		}
		else
		{
			if(bAiming || bReloading || bEquipping)
			{
				RecoilWeight = 1.f;
			}
			else
			{
				RecoilWeight = 0.5f;
			}
		}
	}	
	
	
		
	
}

void UShooterAnimInstance::Lean(float DeltaTime)
{
	if(ShooterCharacter == nullptr) return;
	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = ShooterCharacter->GetActorRotation();
	const FRotator Delta{UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame)};
	const float Target{Delta.Yaw / DeltaTime};
	const float Interp{FMath::FInterpTo(YawDelta, Target, DeltaTime, 6.f)};
	YawDelta = FMath::Clamp(Interp, -90.f, 90.f);

	if(GEngine) GEngine->AddOnScreenDebugMessage
	(2, -1, FColor::Cyan,
		FString::Printf(TEXT("YawDelta: %f"),
			YawDelta));
	if(GEngine) GEngine->AddOnScreenDebugMessage
	(2, -1, FColor::Cyan,
		FString::Printf(TEXT("Delta.Yaw: %f"),
			Delta.Yaw));
	
}
