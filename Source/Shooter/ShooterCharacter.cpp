// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("BeginPlay() Called"));

	int myInt{42};
	UE_LOG(LogTemp, Warning, TEXT("int my Int: %d"), myInt);

	float myFloat {3.14195};
	UE_LOG(LogTemp, Warning, TEXT("float myFloat: %f"), myFloat);
	double myDouble{0.0000756};
	UE_LOG(LogTemp, Warning, TEXT("float myFloat: %lf"), myDouble);
	
	char myChar{'J'};
	UE_LOG(LogTemp, Warning, TEXT("char myChar: %c"), myChar);

	wchar_t wideChar{'L'};
	UE_LOG(LogTemp, Warning, TEXT("wchar_t wideChar: %lc"), wideChar);
	bool myBool{true};
	UE_LOG(LogTemp, Warning, TEXT("bool myBool: %d"), myBool);


	
	
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

