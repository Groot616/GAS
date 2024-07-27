// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"
#include "Interaction/EnemyInterface.h"
#include "Input/AuraInputComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "AuraGameplayTags.h"
#include "E:/Unreal Engine/UE_5.3/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "E:/Unreal Engine/UE_5.3/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "NavigationSystem.h"
#include "NavMesh/NavMeshPath.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAuraPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CursorTrace();
	AutoRun();
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);

	// InputMappingContext를 추가하기 위해 EnahcedInputLocalPlayerSubsystem에 access해야함
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}
	

	bShowMouseCursor = true;
	// 마우스커서 기본으로 설정
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	// 마우스를 뷰포트에 고정할건지 자유롭게 놔둘건지 지정
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	// 캡쳐 모드에서 마우스를 숨길건지에 대한 지정
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// CustomInputComponent를 사용할 것이므로 UEnhancedInputComonent가 아닌 UAuraInputComponent를 캐스팅
	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	// CustomInputComponent인 AuraInputComponent 사용
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);

}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	// Get<FVector2D>() 는 입력 데이터를 FVector2D 타입으로 변환하여 반환하는 함수
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	// 회전행렬을 통해 Yaw축 기준 회전후 X축을 ForwardDirection으로 지정, RightDirection도 동일
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// Controller를 소유하는 pawn의 유효성 검사
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	if (LastActor != ThisActor)
	{
		if (LastActor) LastActor->UnHighlightActor();
		if (ThisActor) ThisActor->HighlightActor();
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	// LMB 태그와 InputTag를 비교하여 LMB와 일치할 경우 bAutoRunning = false;
	// 추가로 타게팅 대상이 존재하는지 확인후 bool값 할당
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = ThisActor ? true : false;
		bAutoRunning = false;
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	// LMB가 아닌 다른 키 입력 유지시 해당 태그에 맞는 Ability가 발동되도록 AbilityInputTagHeld(InputTag) 실행
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
		return;
	}

	// 타게팅 중인 경우 해당 태그에 맞는 Ability가 발동되도록 AbilityInputTagHeld(InputTag) 실행
	// ex) LMB로 공격 Abiliity 사용시
	if (bTargeting)
	{
		if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
	}
	// 타케팅중이 아닐 경우
	else
	{
		APawn* ControlledPawn = GetPawn();
		// 클릭시 AbilityInputTagHeld()가 무조건 먼저 실행되므로 FollowTime 값에 변동 발생
		// 키 입력 유지시간이 클릭 최소 요구시간 이하일 경우 Spline을 통한 이동 보정
		if (FollowTime <= ShortPressThreshold && ControlledPawn)
		{
			
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
			{
				Spline->ClearSplinePoints();
				for (const FVector& PointLoc : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
					DrawDebugSphere(GetWorld(), PointLoc, 8.f, 8, FColor::Green, false, 5.f);
				}
				// 목표 지점 직전 위치에 생성되는 포인트로 도달 지점 지정
				// 액터 중심으로 이동을 시도할 경우 액터 앞에서 멈추게 됨
				CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
				bAutoRunning = true;
			}
		}
		FollowTime = 0.f;
		bTargeting = false;
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	// LMB가 아닌 다른 키 입력 유지시 해당 태그에 맞는 Ability가 발동되도록 AbilityInputTagHeld(InputTag) 실행
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
		// RMB 클릭이 아니므로 Ability 활성화후 리턴
		return;
	}

	// 타게팅 중인 경우 해당 태그에 맞는 Ability가 발동되도록 AbilityInputTagHeld(InputTag) 실행
	// ex) LMB로 공격 Abiliity 사용시
	if (bTargeting)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
		// RMB 클릭일 경우 이동기능까지 하고 종료되어야 하므로 return 사용x
	}
	// 타케팅중이 아닐 경우
	else
	{
		// 키 입력 유지시간
		FollowTime += GetWorld()->GetDeltaSeconds();

		// 마우스 커서 지점 이동을 위한 코드
		FHitResult Hit;
		if(GetHitResultUnderCursor(ECC_Visibility, false, Hit))
		{
			CachedDestination = Hit.ImpactPoint;
		}
		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}

	
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraAbilitySystemComponent;
}

void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning) return;
	if (APawn* ControlledPawn = GetPawn())
	{
		/*
		 * Tick함수를 통해 지속적으로 Spline을 통해 위치와 방향벡터를 구해서 캐릭터를 이동함
		 */
		// Spline 위에서 캐릭터의 위치와 가장 가까운 지점의 위치를 벡터 형태로 반환
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		// Spline 위에서 LocationOnSpline에 대한 방향 벡터(Spline의 곡선 방향)를 반환
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		// 캐릭터를 Spline 방향으로 이동시키기 위한 방향 벡터 사용
		ControlledPawn->AddMovementInput(Direction);

		// 오차범위 내 도달시 자동 이동 취소
		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}
