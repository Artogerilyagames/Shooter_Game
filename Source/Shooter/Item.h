// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
	
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Item Properties", meta = (AllowPrivateAccess = "true"))
	EItemRarity ItemRarity;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Item Properties", meta = (AllowPrivateAccess = "true"))
	TArray<bool> ActiveStars;
	
public:
	FORCEINLINE UWidgetComponent* GetPickupWidget() const {return PickupWidget;}

};
