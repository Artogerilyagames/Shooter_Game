// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BulletHitInterface.h"
#include "Components/SphereComponent.h"
#include "Editor/PropertyEditorTestObject.h"
#include "GameFramework/Character.h"
#include "Sound/SoundCue.h"
#include "Enemy.generated.h"

UCLASS()
class SHOOTER_API AEnemy : public ACharacter, public IBulletHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent)
	void ShowHealthBar();
	void ShowHealthBar_Implementation();
	
	UFUNCTION(BlueprintNativeEvent)
	void HideHealthBar();
	void Die();
	void PlayHitMontage(FName Section, float PlayRate = 1.0f);
	
	void ResetHitReactTimer();
	
	UFUNCTION()
	void AgroSphereOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable)
	void SetStunned(bool Stunned);


	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Combat, meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* ImpactParticles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Combat, meta = (AllowPrivateAccess = "true"))
	class USoundCue* ImpactSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Combat, meta = (AllowPrivateAccess = "true"))
	float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Combat, meta = (AllowPrivateAccess = "true"))
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Combat, meta = (AllowPrivateAccess = "true"))
	FString HeadBone;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Combat, meta = (AllowPrivateAccess = "true"))
	float HealthBarDisplayTime;

	FTimerHandle HealthBarTimer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitMontage;

	FTimerHandle HitReactTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Combat, meta = (AllowPrivateAccess = "true"))
	float HitReactTimeMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Combat, meta = (AllowPrivateAccess = "true"))
	float HitReactTimeMax;
	
	bool bCanHitReact;
	
	UPROPERTY(EditAnywhere, Category= "BehaviorTree",meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* BehaviorTree;
	UPROPERTY(EditAnywhere, Category= "BehaviorTree", meta = (AllowPrivateAccess = "true", MakeEditWidget = "true"))
	FVector PatrolPoint;

	UPROPERTY(EditAnywhere, Category= "BehaviorTree", meta = (AllowPrivateAccess = "true", MakeEditWidget = "true"))
	FVector PatrolPoint2;

	class AEnemyController* EnemyController;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Combat, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* AgroSphere;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Combat, meta= (AllowPrivateAccess = "true"))
	bool bStunned;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Combat, meta= (AllowPrivateAccess = "true"))
	float StunChance;



	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BulletHit_Implementation(FHitResult HitResult) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	FORCEINLINE FString GetHeadBone() const {return HeadBone;}
	FORCEINLINE UBehaviorTree* GetBehaviorTree() const {return BehaviorTree;}
};
