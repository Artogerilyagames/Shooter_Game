// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "DrawDebugHelpers.h"
#include "EnemyController.h"
#include "ShooterCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/SkeletalMeshSocket.h"


#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundCue.h"


// Sets default values
AEnemy::AEnemy() :

Health(100.f),
MaxHealth(100.f),
HealthBarDisplayTime(4.f),
HitReactTimeMin(.5f),
HitReactTimeMax(3.f),
bCanHitReact(true),
HitNumberDestroyTime(1.f),
bStunned(false),

StunChance(0.5f),
AttackFast(TEXT("AttackFast")),
AttackChase(TEXT("AttackChase")),
AttackIdle(TEXT("AttackIdle")),
BaseDamage(20.f),
LeftHandSocket(TEXT("hand_lSocket")),
RightHandSocket(TEXT("hand_rSocket")),
bCanAttack(true),
AttackWaitTime(1.f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	AgroSphere->SetupAttachment(GetRootComponent());

	CombatRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatRange"));
	CombatRangeSphere->SetupAttachment(GetRootComponent());
// Construct left and right hand collision boxes
	LeftHandCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Left Hand Collision"));
	LeftHandCollision->SetupAttachment(GetMesh(), FName("LeftHandCollisionSocket"));

	RightHandCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Right Hand Collision"));
	RightHandCollision->SetupAttachment(GetMesh(), FName("RightHandCollisionSocket"));

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	AgroSphere->OnComponentBeginOverlap.AddDynamic
	(this, &AEnemy::AgroSphereOverlap);
	
	CombatRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatRangeOverlap);
	CombatRangeSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatRangeEndOverlap);
	
	// Bind function overlap events for weapon boxes
	LeftHandCollision->OnComponentBeginOverlap.AddDynamic
	(this, &AEnemy::OnLeftHandCollisionOverlap);
	
	RightHandCollision->OnComponentBeginOverlap.AddDynamic
	(this, &AEnemy::OnRightHandCollisionOverlap);
	
	// Set Collision presets for weapon boxes
	LeftHandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftHandCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	LeftHandCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	LeftHandCollision->SetCollisionResponseToChannel
	(ECollisionChannel::ECC_Pawn,
		ECollisionResponse::ECR_Overlap);

	RightHandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHandCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	RightHandCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	RightHandCollision->SetCollisionResponseToChannel
	(ECollisionChannel::ECC_Pawn,
		ECollisionResponse::ECR_Overlap);
	// Hit montage animation & Particles effects
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,ECollisionResponse::ECR_Ignore);
	
	// Get AI Controller
	EnemyController = Cast<AEnemyController>(GetController());
	if(EnemyController)
	{
		EnemyController->GetBlackBoardComponent()->SetValueAsBool(FName("CanAttack"), true);
	}


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



void AEnemy::Die()
{
	HideHealthBar();
}

void AEnemy::PlayHitMontage(FName Section, float PlayRate)
{
	if(bCanHitReact)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if(AnimInstance == nullptr) return;
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

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// Set the target blackboard key to agro the character
	if(EnemyController)
	{
		EnemyController->GetBlackBoardComponent()->SetValueAsObject(FName("Target"),DamageCauser);
	}
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

void AEnemy::StoreHitNumber(UUserWidget* HitNumber, FVector Location)
{
	HitNumbers.Add(HitNumber, Location);
	FTimerHandle HitNumberTimer;
	FTimerDelegate HitNumberDelegate;
	HitNumberDelegate.BindUFunction(this, FName("DestroyHitNumber"), HitNumber);
	GetWorld()->GetTimerManager().SetTimer
	(HitNumberTimer,
		HitNumberDelegate,
		HitNumberDestroyTime,
		false);
}

void AEnemy::DestroyHitNumber(UUserWidget* HitNumber)
{
	HitNumbers.Remove(HitNumber);
	HitNumber->RemoveFromParent();
}

void AEnemy::AgroSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
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

void AEnemy::CombatRangeOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor == nullptr) return;
	auto ShooterCharacter = Cast<AShooterCharacter>(OtherActor);
	if(ShooterCharacter)
	{
		bInAttackRange = true;
		if(EnemyController)
		{
			EnemyController->GetBlackBoardComponent()->SetValueAsBool(TEXT("InAttackRange"), true);
		}
	}
	
}

void AEnemy::CombatRangeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	if(OtherActor == nullptr) return;
	auto ShooterCharacter = Cast<AShooterCharacter>(OtherActor);
	if(ShooterCharacter)
	{
		bInAttackRange = false;
		if(EnemyController)
		{
			EnemyController->GetBlackBoardComponent()->SetValueAsBool(TEXT("InAttackRange"), false);
		}
	}
	
}

void AEnemy::PlayAttackMontage(FName Section, float PlayRate)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		AnimInstance->Montage_JumpToSection(Section, AttackMontage);
	}
	bCanAttack = false;
	GetWorldTimerManager().SetTimer
	(AttackWaitTimer,
		this,
		&AEnemy::ResetCanAttack,
		AttackWaitTime);
	if(EnemyController)
	{
		EnemyController->GetBlackBoardComponent()->SetValueAsBool(FName("CanAttack"), false);
	}
}

FName AEnemy::GetAttackSectionName()
{
	FName SectionName;
	const int32 Section{FMath::RandRange(1, 3)};
	switch (Section)
	{
	case 1:
		SectionName = AttackFast;
		break;
	case 2:
		SectionName = AttackChase;
		break;

	case 3:
		SectionName = AttackIdle;
		break;
	default: ;
	}
	return SectionName;
}

void AEnemy::OnLeftHandCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Character = Cast<AShooterCharacter>(OtherActor);
	if(Character)
	{
		DoDamage(Character);
		SpawnBlood(Character, LeftHandSocket);
		StunCharacter(Character);
		
	}
	
}

void AEnemy::OnRightHandCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Character = Cast<AShooterCharacter>(OtherActor);
	if(Character)
	{
		DoDamage(Character);
		SpawnBlood(Character, RightHandSocket);
		StunCharacter(Character);
		
	}
}

void AEnemy::ActivateLeftWeapon()
{
	LeftHandCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AEnemy::DeactivateLeftWeapon()
{
	LeftHandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AEnemy::ActivateRightWeapon()
{
	RightHandCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

}

void AEnemy::DeactivateRightWeapon()
{
	RightHandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::DoDamage(AShooterCharacter* Victim)
{
	if(Victim ==  nullptr) return;
	UGameplayStatics::ApplyDamage
		(Victim,
			BaseDamage,
			EnemyController,
			this,
			UDamageType::StaticClass());
	if(Victim->GetMeleeImpactSound())
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			Victim->GetMeleeImpactSound(),
			GetActorLocation());
	}
	
	
}

void AEnemy::SpawnBlood(AShooterCharacter* Victim, FName SocketName)
{
	
	const USkeletalMeshSocket* TipSocket{GetMesh()->GetSocketByName(SocketName)};
	if(TipSocket)
	{
		const FTransform SocketTransform{TipSocket->GetSocketTransform(GetMesh())};
		if(Victim->GetBloodParticles())
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Victim->GetBloodParticles(),SocketTransform);
		}
			
	}
}

void AEnemy::StunCharacter(AShooterCharacter* Victim)
{
	if(Victim)
	{
		const float Stun{FMath::FRandRange(0.f, 1.f)};
		if(Stun <= Victim->GetStunChance())
		{
			Victim->Stun();
		}
	}
}

void AEnemy::ResetCanAttack()
{
	bCanAttack = true;
	if(EnemyController)
	{
		EnemyController->GetBlackBoardComponent()->SetValueAsBool(FName("CanAttack"),true);
	}
}