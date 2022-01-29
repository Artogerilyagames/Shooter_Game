// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameFramework/PlayerState.h"
#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API AEnemyController : public AAIController
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaSeconds) override;
	
protected:
	virtual void BeginPlay() override;
public:
	AEnemyController();
	virtual void OnPossess(APawn* InPawn) override;
	
private:
	
	// ReSharper disable once UnrealHeaderToolError
	UPROPERTY(BlueprintReadWrite, Category= "AI Behavior", meta = (AllowPrivateAccess = "true"))
	UBlackboardComponent* BlackBoardComponent;
	UPROPERTY(BlueprintReadWrite, Category= "AI Behavior", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Combat,meta = (AllowPrivateAccess = "true"))
	float AcceptanceRadius;
	// Game Dev Course
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Combat,meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* AIBehavior;
	


public:
	
	FORCEINLINE UBlackboardComponent* GetBlackBoardComponent() const {return BlackBoardComponent;}
	
	
};
