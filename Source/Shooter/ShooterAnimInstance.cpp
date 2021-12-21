// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAnimInstance.h"
#include "ShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UShooterAnimInstance::UShooterAnimInstance() :
Speed(0.f),
bIsInAir(false),
bIsAccelerating(false),
MovementOffsetYaw(0.f),
LastMovementOffsetYaw(0.f),
bAiming(false),
CharacterYaw(0.f),
CharacterYawLastFrame(0.f),
RootYawOffset(0.f)
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
		/*FString OffsetMessage = FString::Printf
		(TEXT("Movement Offset Yaw: %f"),
			MovementRotation.Yaw);*/
		/*FString MovementRotationMessage = FString::Printf(TEXT("Movement Rotation: %f"), MovementRotation.Yaw);*/
		
		/*
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::White,OffsetMessage);
		}
		*/
		
	}
	TurnInPlace();
}

void UShooterAnimInstance::NativeInitializeAnimation()
{
	ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
}

void UShooterAnimInstance::TurnInPlace()
{
	if(ShooterCharacter == nullptr) return;
	if(Speed > 0)
	{
		// Dont want to Turn in Place; Character is moving
		RootYawOffset = 0.f;
		CharacterYaw =  ShooterCharacter->GetActorRotation().Yaw;
		CharacterYawLastFrame = CharacterYaw;
		RotationCurveLastFrame = 0.f;
		RotationCurve = 0.f;
	}
	else
	{
		CharacterYawLastFrame = CharacterYaw;
		CharacterYaw = ShooterCharacter->GetActorRotation().Yaw;
		const float YawDelta{CharacterYaw - CharacterYawLastFrame };
		// Root Yaw Offset, Update and clamped to [-180,180]
		RootYawOffset = UKismetMathLibrary::NormalizeAxis(RootYawOffset - YawDelta);

		const float Turning{GetCurveValue(TEXT("Turning"))};
		if(Turning > 0)
		{
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
		if(GEngine) GEngine->AddOnScreenDebugMessage(1, -1, FColor::Cyan, FString::Printf(TEXT("")));
		
	}
}
