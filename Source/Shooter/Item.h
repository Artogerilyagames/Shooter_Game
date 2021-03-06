// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Item.generated.h"
UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	EIR_Damaged UMETA(DisplayName = "Damaged"),
	EIR_Common UMETA(DisplayName = "Common"),
	EIR_Uncommon UMETA(DisplayName = "Uncommon"),
	EIR_Rare UMETA(DisplayName = "Rare"),
	EIR_Legendary UMETA(DisplayName = "Legendary"),

	EIR_MAX UMETA(DisplayName = "DefaultMAX"),
};

UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_Pickup UMETA(DisplayName = "Pickup"),
	EIS_EquipInterping UMETA(DisplayName = "EquipInterping"),
	EIS_PickUp UMETA(DisplayName = "PickUp"),
	EIS_Equipped UMETA(DisplayName = "Equipped"),
	EIS_Falling UMETA(DisplayName = "Falling"),
	
	EIS_MAX UMETA(DisplayName = "DefaultMAX"),
};

USTRUCT(BlueprintType)
struct FItemRarityTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor GlowColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor LightColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor DarkColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumberOfStars;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* IconBackground;
};

UCLASS()
class SHOOTER_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called when overlapping AreaSphere
    UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* Overlapped,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex);

	void SetActiveStars();
	virtual void SetItemProperties(EItemState State);

	void FinishInterping();

	void ItemInterp(float DeltaTime);
	void PlayPickupSound(bool bForcePlaySound = false);
	virtual void InitializeCustomDepth();
	virtual void OnConstruction(const FTransform& Transform) override;


	


	
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PlayEquipSound(bool bForcePlaySound = false);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item Properties", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CollisionBox;
	
	/** Popup widget for when the player looks at the Item*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* PickupWidget;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Item Properties", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* AreaSphere;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Item Properties", meta = (AllowPrivateAccess = "true"))
	FString ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Item Properties", meta = (AllowPrivateAccess = "true"))
	int32 ItemCount;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Rarity", meta = (AllowPrivateAccess = "true"))
	EItemRarity ItemRarity;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Item Properties", meta = (AllowPrivateAccess = "true"))
	TArray<bool> ActiveStars;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Item Properties", meta = (AllowPrivateAccess = "true"))
	EItemState ItemState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UCurveFloat* ItemZCurve;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Item Properties", meta = (AllowPrivateAccess = "true")) 
	FVector ItemInterpStartLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Item Properties", meta = (AllowPrivateAccess = "true")) 
	FVector CameraTargetLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Item Properties", meta = (AllowPrivateAccess = "true")) 
	bool bInterping;
	
	FTimerHandle ItemInterpTimer;
	class AShooterCharacter* Character;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Item Properties", meta = (AllowPrivateAccess = "true"))
	float ZCurveTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Item Properties", meta = (AllowPrivateAccess = "true"))
	class USoundCue* PickupSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Item Properties", meta = (AllowPrivateAccess = "true"))
	USoundCue* EquipSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Inventory, meta = (AllowPrivateAccess = "true"))
	UTexture2D* IconItem;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Inventory, meta = (AllowPrivateAccess = "true"))
	UTexture2D* AmmoItem;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Inventory, meta = (AllowPrivateAccess = "true"))
	int32 SlotIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Inventory, meta = (AllowPrivateAccess = "true"))
	bool bCharacterInventoryFull;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= DataTable, meta = (AllowPrivateAccess = "true"))
	class UDataTable* ItemRarityDataTable;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Rarity, meta = (AllowPrivateAccess = "true"))
	FLinearColor GlowColor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Rarity, meta = (AllowPrivateAccess = "true"))
	FLinearColor LightColor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Rarity, meta = (AllowPrivateAccess = "true"))
	FLinearColor DarkColor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Rarity, meta = (AllowPrivateAccess = "true"))
	int32 NumberOfStars;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Inventory, meta = (AllowPrivateAccess = "true"))
	UTexture2D* IconBackground;


	

	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category= "Item Properties", meta = (AllowPrivateAccess = "true"))
	int32 MaterialIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Item Properties", meta = (AllowPrivateAccess = "true"))
	UMaterialInstanceDynamic*  DynamicMaterialInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Item Properties", meta = (AllowPrivateAccess = "true"))
	UMaterialInstance* MaterialInstance;
	bool bCanChangeCustomDepth;
	


public:
	FORCEINLINE UWidgetComponent* GetPickupWidget() const {return PickupWidget;}
	FORCEINLINE USphereComponent* GetAreaSphere() const {return  AreaSphere;}
	FORCEINLINE UBoxComponent* GetCollisionBox() const {return  CollisionBox;}
	FORCEINLINE EItemState GetItemState() const { return ItemState;}
	FORCEINLINE void SetItemState(EItemState State);
	FORCEINLINE USkeletalMeshComponent* GetItemMesh() const {return ItemMesh;}
	void StartItemCurve(AShooterCharacter* Char, bool bForcePlaySound = false);
	

	
	FORCEINLINE USoundCue* GetPickupSound() const {return PickupSound;}
	FORCEINLINE void SetPickupSound(USoundCue* Sound) {PickupSound = Sound;}
	FORCEINLINE USoundCue* GetEquipSound() const {return EquipSound;}
	FORCEINLINE void SetEquipSound(USoundCue* Sound) {EquipSound = Sound;}
	FORCEINLINE int32 GetItemCount() const {return ItemCount;}
	FORCEINLINE int32 GetSlotIndex() const {return SlotIndex;}
	FORCEINLINE void SetSlotIndex(int32 Index) {SlotIndex = Index;}
	FORCEINLINE void SetCharacter(AShooterCharacter* Char) {Character = Char;}
	FORCEINLINE void SetCharacterInventoryFull(bool bFull){bCharacterInventoryFull = bFull;}
	
	FORCEINLINE void SetItemName(FString Name) {ItemName = Name;}
	FORCEINLINE void SetIconItem(UTexture2D* Icon){IconItem = Icon;}
	FORCEINLINE void SetAmmoIcon(UTexture2D* Icon) { AmmoItem = Icon;}

	virtual void EnableCustomDepth();
	virtual void DisableCustomDepth();

	

};
