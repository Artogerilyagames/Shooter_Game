// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "DrawDebugHelpers.h"
#include "EnemyController.h"
#include "ShooterCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AEnemy::AEnemy() :

Health(100.f),
MaxHealth(100.f),
HealthBarDisplayTime(4.f),
HitReactTimeMin(.5f),
HitReactTimeMax(3.f),
bCanHitReact(true),
bStunned(false),
StunChance(0.5f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	AgroSphere->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	AgroSphere->OnComponentBeginOverlap.AddDynamic
	(this,
		&AEnemy::AgroSphereOverlap);
	
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	// Get AI Controller
	EnemyController = Cast<AEnemyController>(GetController());

	const FVector WorldPatrolPoint = UKismetMathLibrary::TransformLocation
	(GetActorTransform(),
		PatrolPoint);
	DrawDebugSphere(GetWorld(),WorldPatrolPoint, 25.f, 12, FColor::Red, true);
	const FVector WorldPatrolPoint2 = UKismetMathLibrary::TransformLocation
	(GetActorTransform(),
		PatrolPoint2);
	DrawDebugSphere(GetWorld(),WorldPatrolPoint2, 25.f, 12, FColor::Red, true);
	if(EnemyController)
	{
		EnemyController->GetBlackBoardComponent()->SetValueAsVector
		(TEXT("PatrolPoint"),
			WorldPatrolPoint);
		
		EnemyController->GetBlackBoardComponent()->SetValueAsVector
		(TEXT("PatrolPoint2"),
			WorldPatrolPoint2);
		EnemyController->RunBehaviorTree(BehaviorTree);
	}
}



void AEnemy::ShowHealthBar_Implementation()
{
	GetWorldTimerManager().ClearTimer(HealthBarTimer);
	GetWorldTimerManager().SetTimer
	(HealthBarTimer,
		this,
		&AEnemy::HideHealthBar,
		HealthBarDisplayTime);
}

void AEnemy::HideHealthBar_Implementation()
{
}

void AEnemy::Die()
{
	HideHealthBar();
}

void AEnemy::PlayHitMontage(FName Section, float PlayRate)
{
	if(bCanHitReact)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if(AnimInstance)
		{
			AnimInstance->Montage_Play(HitMontage, PlayRate);
			AnimInstance->Montage_JumpToSection(Section, HitMontage);
		}
		bCanHitReact = false;
		const float HitReactTime{FMath::FRandRange(HitReactTimeMin,HitReactTimeMax)};
		GetWorldTimerManager().SetTimer(HitReactTimer, this, &AEnemy::ResetHitReactTimer, HitReactTime);
	}
}


// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::BulletHit_Implementation(FHitResult HitResult)
{
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}
	if (ImpactParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, HitResult.Location, FRotator(0.f), true);
	}
	ShowHealthBar();
	const float Stunned = FMath::FRandRange(0.f, 1.f);
	if(Stunned <= StunChance)
	{
		PlayHitMontage(FName("HitReactFront"));
	SetStunned(true);
	}
	
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if(Health - DamageAmount <= 0.f)
	{
		Health = 0.f;
		Die();
	}
	else
	{
		Health -= DamageAmount;
	}
	return  DamageAmount;
		
}

void AEnemy::ResetHitReactTimer()
{
	bCanHitReact =  true;
}

void AEnemy::AgroSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor == nullptr) return;
	auto Character = Cast<AShooterCharacter>(OtherActor);
	if(Character)
	{
		EnemyController->GetBlackBoardComponent()->SetValueAsObject
		(TEXT("Target"),
			Character);
	}
}

void AEnemy::SetStunned(bool Stunned)
{
	bStunned = Stunned;
	if(EnemyController)
	{
		EnemyController->GetBlackBoardComponent()->SetValueAsBool
		(TEXT("Stunned"),
			Stunned);
	}
}

