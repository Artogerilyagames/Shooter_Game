// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "ShooterCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


// Sets default values
AItem::AItem() :
ItemName(FString("Default")),
ItemCount(0),
ItemRarity(EItemRarity::EIR_Common),
ItemState(EItemState::EIS_Pickup),
ItemInterpStartLocation(FVector(0.f)),
CameraTargetLocation(FVector(0.f)),
bInterping(false),
ZCurveTime(0.7f),
SlotIndex(0),
MaterialIndex(0),
bCanChangeCustomDepth(true)




{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(ItemMesh);
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(ItemMesh);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel
	(ECollisionChannel::ECC_Visibility,
		ECollisionResponse::ECR_Block);
	
	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(GetRootComponent());

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	if(PickupWidget)
	{
		PickupWidget->SetVisibility(false);
	}
	SetActiveStars();

	AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
	SetItemProperties(ItemState);


	InitializeCustomDepth();
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor)
	{
		AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(OtherActor);
		if(ShooterCharacter)
		{
			ShooterCharacter->IncrementOverlappedItemCount(1);
		}
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* Overlapped, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex)
{
	if(OtherActor)
	{
		AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(OtherActor);
		if(ShooterCharacter)
		{
			ShooterCharacter->IncrementOverlappedItemCount(-1);
		}
	}
}

void AItem::SetActiveStars()
{
	
		for (int32 i = 0; i<=5; i++)
		{
			ActiveStars.Add(false);
		}
		switch (ItemRarity)
		{
		case EItemRarity::EIR_Damaged:
			ActiveStars[1] = true;
			break;
		case EItemRarity::EIR_Common:
			ActiveStars[1] = true;
			ActiveStars[2] = true;
			break;
		case EItemRarity::EIR_Uncommon: 
			ActiveStars[1] = true;
			ActiveStars[2] = true;
			ActiveStars[3] = true;
			break;
		case EItemRarity::EIR_Rare:
			ActiveStars[1] = true;
			ActiveStars[2] = true;
			ActiveStars[3] = true;
			ActiveStars[4] = true;
			break;
		case EItemRarity::EIR_Legendary:
			ActiveStars[1] = true;
			ActiveStars[2] = true;
			ActiveStars[3] = true;
			ActiveStars[4] = true;
			ActiveStars[5] = true;
			break;
		default: ;
		}
	
}

void AItem::SetItemProperties(EItemState State)
{
	switch (State)
	{
	case EItemState::EIS_Pickup:
		ItemMesh->SetSimulatePhysics(false);
		ItemMesh->SetEnableGravity(false);
		ItemMesh->SetVisibility(true);
		ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollisionBox->SetCollisionResponseToChannel
		(ECollisionChannel::ECC_Visibility,
			ECollisionResponse::ECR_Block);
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;
	case EItemState::EIS_Equipped:
		PickupWidget->SetVisibility(false);
		ItemMesh->SetSimulatePhysics(false);
		ItemMesh->SetEnableGravity(false);
		ItemMesh->SetVisibility(true);
		ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		break;
	case EItemState::EIS_Falling:
	
		ItemMesh->SetSimulatePhysics(true);
		ItemMesh->SetEnableGravity(true);
		ItemMesh->SetVisibility(true);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
		AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	case EItemState::EIS_EquipInterping:
		PickupWidget->SetVisibility(false);
		ItemMesh->SetSimulatePhysics(false);
		ItemMesh->SetEnableGravity(false);
		ItemMesh->SetVisibility(true);
		ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		break;
	case EItemState::EIS_PickUp:
		PickupWidget->SetVisibility(false);
		ItemMesh->SetSimulatePhysics(false);
		ItemMesh->SetEnableGravity(false);
		ItemMesh->SetVisibility(false);
		ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	default: ;
	}
}

void AItem::FinishInterping()
{
	bInterping = false;
	if(Character)
	{
		/*Character->IncrementInterpLocItemCount(InterpLocIndex, -1);*/
		Character->GetPickupItem(this);
	}
	bCanChangeCustomDepth = true;
	DisableCustomDepth();
	
}

void AItem::ItemInterp(float DeltaTime)
{
	if(!bInterping) return;
	if(Character &&  ItemZCurve)
	{
		const float ElapsedTime = GetWorldTimerManager().GetTimerElapsed(ItemInterpTimer);
		const float CurveValue = ItemZCurve->GetFloatValue(ElapsedTime);
		FVector ItemLocation = ItemInterpStartLocation;
		const FVector CameraInterpLocation{Character->GetCameraInterpLocation()};
		const FVector ItemToCamera{FVector(0.f, 0.f,(CameraInterpLocation - ItemLocation).Z)};
		const float DeltaZ = ItemToCamera.Size();
		ItemLocation.Z += CurveValue * DeltaZ;
		SetActorLocation(ItemLocation, true, nullptr, ETeleportType::TeleportPhysics);
	}
	
}

void AItem::PlayPickupSound(bool bForcePlaySound)
{
	if(Character)
	{
		if(bForcePlaySound)
		{
			if(PickupSound)
			{
				UGameplayStatics::PlaySound2D(this, PickupSound);
			}
		}
		else if(Character->ShouldPlayPickupSound())
		{
			Character->StartPickupSoundTimer();
			if(PickupSound)
			{
				UGameplayStatics::PlaySound2D(this, PickupSound);
			}
		}
	}
}

void AItem::EnableCustomDepth()
{
	if(bCanChangeCustomDepth)
	{
		ItemMesh->SetRenderCustomDepth(true);
	}
	
}

void AItem::DisableCustomDepth()
{
	if(bCanChangeCustomDepth)
	{
		ItemMesh->SetRenderCustomDepth(false);
	}
	
}

void AItem::InitializeCustomDepth()
{
	DisableCustomDepth();
}

void AItem::OnConstruction(const FTransform& Transform)
{
  
	
	// Path to the Item Rarity Data table
   const FString RarityTablePath(TEXT("DataTable'/Game/Game/DataTable/ItemRarityDataTable.ItemRarityDataTable'"));
	UDataTable* RarityTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *RarityTablePath));
	if(RarityTableObject)
	{
		
		// ReSharper disable once CppDeclaratorNeverUsed
		FItemRarityTable* RarityRow = nullptr;
		switch (ItemRarity)
		{
		case EItemRarity::EIR_Damaged:
		    RarityTableObject->FindRow<FItemRarityTable>(FName("Damaged"), TEXT(""));
			break;
		case EItemRarity::EIR_Common:
		    RarityTableObject->FindRow<FItemRarityTable>(FName("Common"), TEXT(""));
			break;
		case EItemRarity::EIR_Uncommon:
			RarityTableObject->FindRow<FItemRarityTable>(FName("Uncommon"), TEXT(""));
			break;
		case EItemRarity::EIR_Rare:
			RarityTableObject->FindRow<FItemRarityTable>(FName("Rare"), TEXT(""));
			break;
		case EItemRarity::EIR_Legendary:
			RarityTableObject->FindRow<FItemRarityTable>(FName("Legendary"), TEXT(""));
			break;
		    // ReSharper disable once CppUnreachableCode
		    if(RarityRow)
			{
				GlowColor = RarityRow->GlowColor;
				LightColor = RarityRow->LightColor;
				DarkColor = RarityRow->DarkColor;
				NumberOfStars = RarityRow->NumberOfStars;
				IconBackground = RarityRow->IconBackground;
		    	
			}

		default: ;
		}
	}
	if(MaterialInstance)
	{
		DynamicMaterialInstance = UMaterialInstanceDynamic::Create(MaterialInstance, this);
		DynamicMaterialInstance->SetVectorParameterValue(TEXT("FresnelColor"), GlowColor);
		ItemMesh->SetMaterial(MaterialIndex, DynamicMaterialInstance);
		/*// EnableGlowMaterial();*/
	}
	
}



void AItem::PlayEquipSound(bool bForcePlaySound)
{
	if(Character)
	{
		if(bForcePlaySound)
		{
			if(EquipSound)
			{
				UGameplayStatics::PlaySound2D(this, EquipSound);
			}
		}
		else if(Character->ShouldPlayEquipSound())
		{
			Character->StartEquipSoundTimer();
			if(EquipSound)
			{
				UGameplayStatics::PlaySound2D(this, EquipSound);
			}
		}
	}
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ItemInterp(DeltaTime);
}
void AItem::SetItemState(EItemState State)
{
	ItemState = State;
	SetItemProperties(State);
}

void AItem::StartItemCurve(AShooterCharacter* Char, bool bForcePlaySound)
{
	Character = Char;
    PlayPickupSound(bForcePlaySound);
	ItemInterpStartLocation = GetActorLocation();
	bInterping = true;
	SetItemState(EItemState::EIS_EquipInterping);

	GetWorldTimerManager().SetTimer(ItemInterpTimer, this,
		&AItem::FinishInterping,
		ZCurveTime);
	bCanChangeCustomDepth = false;
}


