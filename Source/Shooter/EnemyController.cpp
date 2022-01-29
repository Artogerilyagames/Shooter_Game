// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "Enemy.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Kismet/GameplayStatics.h"

//Set Default Value
AEnemyController::AEnemyController() :

AcceptanceRadius(200.f)

{
	BlackBoardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoardComponent"));
	check(BlackBoardComponent);
	
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	check(BehaviorTreeComponent);
}

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();
	if (AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);
		const APawn* PlayerPawn =  UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		GetBlackBoardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
		GetBlackBoardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());

	}
	
}


void AEnemyController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	
	
}




void AEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if(InPawn == nullptr) return;
	AEnemy* Enemy = Cast<AEnemy>(InPawn);
	if(Enemy)
	{
		if(Enemy->GetBehaviorTree())
		{
			BlackBoardComponent->InitializeBlackboard(*(Enemy->GetBehaviorTree()->BlackboardAsset));
		}
	}
}
