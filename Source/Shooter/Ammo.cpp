// Fill out your copyright notice in the Description page of Project Settings.


#include "Ammo.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "ShooterCharacter.h"
#include "Kismet/GameplayStatics.h"



AAmmo::AAmmo()
{
	AmmoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AmmoMesh"));
	SetRootComponent(AmmoMesh);

	GetCollisionBox()->SetupAttachment(GetRootComponent());
	GetPickupWidget()->SetupAttachment(GetRootComponent());
	GetAreaSphere()->SetupAttachment(GetRootComponent());
	// Ammo Pickup Overlap code
	AmmoCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AmmoCollisionSphere"));
	AmmoCollisionSphere->SetupAttachment(GetRootComponent());
	AmmoCollisionSphere->SetSphereRadius(50.f);
}

void AAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}


void AAmmo::BeginPlay()
{
	Super::BeginPlay();
    //Ammo Pickup Overlap code
	AmmoCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AAmmo::AAmmo::AmmoSphereOverlap);
	
}

void AAmmo::SetItemProperties(EItemState State)
{
	Super::SetItemProperties(State);
	switch (State)
	{
	case EItemState::EIS_Pickup:
		AmmoMesh->SetSimulatePhysics(false);
		AmmoMesh->SetEnableGravity(false);
		AmmoMesh->SetVisibility(true);
		AmmoMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		AmmoMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
		break;
	case EItemState::EIS_Equipped:
	
		AmmoMesh->SetSimulatePhysics(false);
		AmmoMesh->SetEnableGravity(false);
		AmmoMesh->SetVisibility(true);
		AmmoMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		AmmoMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	case EItemState::EIS_Falling:
		AmmoMesh->SetSimulatePhysics(true);
		AmmoMesh->SetEnableGravity(true);
		AmmoMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AmmoMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		AmmoMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
		break;
	case EItemState::EIS_PickUp:
		AmmoMesh->SetSimulatePhysics(false);
		
	
	default: ;
	}
}


// Ammo Pickup Overlap code
void AAmmo::AmmoSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor)
	{
		 auto OverlapCharacter = Cast<AShooterCharacter>(OtherActor);
		if(OverlapCharacter)
		{
			
		
			StartItemCurve(OverlapCharacter);
			AmmoCollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

void AAmmo::EnableCustomDepth()
{
	Super::EnableCustomDepth();
	AmmoMesh->SetRenderCustomDepth(true);
}

void AAmmo::DisableCustomDepth()
{
	Super::DisableCustomDepth();
	AmmoMesh->SetRenderCustomDepth(false);
}


