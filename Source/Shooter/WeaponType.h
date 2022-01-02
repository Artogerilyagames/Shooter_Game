// Fill out your copyright notice in the Description page of Project Settings.


#pragma once
#include "WeaponType.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_SubmachineGun UMETA(DisplayName = "SubmachineGun"),
	EWT_AssultRiffle UMETA(DisplayName = "AssultRiffle"),

	EWT_MAX UMETA(DisplayName = "DefaultMAX"),
};