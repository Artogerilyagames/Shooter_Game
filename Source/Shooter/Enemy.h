// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BulletHitInterface.h"
#include "ShooterCharacter.h"
#include "Editor/PropertyEditorTestObject.h"
#include "GameFramework/Character.h"
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
	
	UFUNCTION(BlueprintImplementableEvent)
	void HideHealthBar();
	void Die();
	void PlayHitMontage(FName Section, float PlayRate = 1.0f);
	
	void ResetHitReactTimer();
    UFUNCTION(BlueprintCallable)
	void StoreHitNumber(UUserWidget* HitNumber, FVector Location);
    UFUNCTION()
	void DestroyHitNumber(UUserWidget* HitNumber);

	UFUNCTION()
	void AgroSphereOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintCallable)
	void SetStunned(bool Stunned);
	
    UFUNCTION()
	void CombatRangeOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
    UFUNCTION()
	void CombatRangeEndOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex);
    UFUNCTION(BlueprintCallable)
	void PlayAttackMontage(FName Section, float PlayRate);
	
    UFUNCTION(BlueprintPure)
	FName GetAttackSectionName();
	
	UFUNCTION()
	void OnLeftHandCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnRightHandCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	// Activate /Deactivate Collision for weapon boxes
    UFUNCTION(BlueprintCallable)
	void ActivateLeftWeapon();
	UFUNCTION(BlueprintCallable)
	void DeactivateLeftWeapon();
	UFUNCTION(BlueprintCallable)
	void ActivateRightWeapon();
	UFUNCTION(BlueprintCallable)
	void DeactivateRightWeapon();
	void DoDamage(class AShooterCharacter* Victim);
	void SpawnBlood(AShooterCharacter* Victim, FName SocketName);
	void StunCharacter(AShooterCharacter* Victim);

	void ResetCanAttack();
	UFUNCTION(BlueprintCallable)
	void FinishDeath();
	UFUNCTION()
	void DestroyEnemy();

	FName HitBone();
	void DisableAttacking();


	
	






	
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
	
	UPROPERTY(VisibleAnywhere, Category= Combat,meta = (AllowPrivateAccess = "true"))
	TMap<UUserWidget*, FVector> HitNumbers;
	UPROPERTY(EditAnywhere, Category= Combat,meta = (AllowPrivateAccess = "true"))
	float HitNumberDestroyTime;
	
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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category= Combat, meta= (AllowPrivateAccess = "true"))
	bool  bInAttackRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Combat, meta = (AllowPrivateAccess = "true"))
	USphereComponent* CombatRangeSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackMontage;
	
	//Attack Montage
	FName AttackFast;
	FName AttackChase;
	FName AttackIdle;
	
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category= Combat, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent*  LeftHandCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category= Combat, meta = (AllowPrivateAccess = "true"))
    UBoxComponent*  RightHandCollision;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Combat, meta = (AllowPrivateAccess = "true"))
	float BaseDamage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Combat,meta =(AllowPrivateAccess = "true"))
	FName LeftHandSocket;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Combat,meta =(AllowPrivateAccess = "true"))
	FName RightHandSocket;
	UPROPERTY(VisibleAnywhere, Category= Combat,meta =(AllowPrivateAccess = "true"))
	bool bCanAttack;

	FTimerHandle AttackWaitTimer;
	UPROPERTY(EditAnywhere, Category= Combat, meta = (AllowPrivateAccess = "true"))
	float AttackWaitTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Combat,meta =(AllowPrivateAccess = "true"))
	UAnimMontage* DeathMontage;
	
	bool bDying;
	FTimerHandle DeathTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Combat, meta = (AllowPrivateAccess = "true"))
	float DeathTime;

	FName UpperArmr;
	FName LowerArmr;
	FName UpperArml;
	FName LowerArml;
	FName Handr;
	FName Handl;
	FName Calfr;
	FName Calfl;
	






	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BulletHit_Implementation(FHitResult HitResult, AActor* Shooter, AController* ShooterController) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	FORCEINLINE FString GetHeadBone() const {return HeadBone;}
	FORCEINLINE UBehaviorTree* GetBehaviorTree() const {return BehaviorTree;}
	
	
    UFUNCTION(BlueprintImplementableEvent)
	void ShowHitNumber(int32 Damage, FVector HitLocation);
	
};
