// Copyright Groot


#include "Player/AuraPlayerController.h"
#include "E:/Unreal Engine/UE_5.3/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "E:/Unreal Engine/UE_5.3/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "GameplayTagContainer.h"
#include "Input/AuraInputComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/SplineComponent.h"
#include "AuraGameplayTags.h"
#include "NavigationSystem.h"
#include "NavMesh/NavMeshPath.h"
#include "GameFramework/Character.h"
#include "UI/Widget/DamageTextComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	// 리플리케이션 플래그 활성화
	bReplicates = true;

    Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    CursorTrace();
    AutoRun();
}

void AAuraPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit)
{
    // Target과 클래스 유효성 검사
    if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
    {
        // DamageText생성
        UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
        // RegisterComponent() : 컴포넌트를 현재 월드에 등록, 컴포넌트, 생성 후 월드에서 동작하도록 하게끔 하기 위해 호출
        DamageText->RegisterComponent();
        DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
        DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
        DamageText->SetDamageText(DamageAmount, bBlockedHit, bCriticalHit);
    }
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

    check(AuraContext);
    // InputMappingContext를 추가하기 위해서 EnhancedInputLocalPlayerSubsystem에 access해야함
    UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    if (SubSystem)
    {
        SubSystem->AddMappingContext(AuraContext, 0);
    }

    // 마우스커서 상시 보이도록 수정 및 기본 마우스커서 적용
    bShowMouseCursor = true;
    DefaultMouseCursor = EMouseCursor::Default;

    // InputModeDate를 통해 키보드와 마우스로부터의 input을 사용가능하게 함
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
    // UEnhancedInputComponent -> UAuraInputComponent
    UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
    // 기존에 에디터에서 Mapping해둔 키워드 사용 대신 헤더파일에서 선언한 MoveAction 사용
    // 기존에 사용하던 `IE_Pressed` 대신 `ETriggerEvent::VALUE` 사용, 버튼 클릭시 트리거 발동
    // CustomInputComponent인 AuraInputComponent 사용
    AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
    AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AAuraPlayerController::ShiftPressed);
    AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AAuraPlayerController::ShiftReleased);
    // 세 함수를 바인딩하기 위한 코드
    AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
    const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
    const FRotator Rotation = GetControlRotation();
    const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

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
    // LMB 클릭이 아닐 경우 ASC의 AbilityInputTagReleased() 함수 실행
    if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
    {
        if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
        return;
    }

    // ASC의 AbilityInputTagReleased() 함수 실행
    if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);

    // 타게팅하지 않을 경우 해당 지점으로 이동
    if(!bTargeting && !bShiftKeyDown)
    {
        const APawn* ControlledPawn = GetPawn();
        // 클릭시 AbilityInputTagHeld()가 무조건 먼저 실행되므로 FollowTime 값에 변동 발생
        // 키 입력 유지시간이 클릭 최소 요구시간 이하일 경우 Spline을 통한 이동 보정
        if (FollowTime <= ShortPressThreshold && ControlledPawn)
        {
            if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
            {
                Spline->ClearSplinePoints();
                // Build.cs 파일에 NavigationSystem 모듈 추가 필요
                for (const FVector& PointLoc : NavPath->PathPoints)
                {
                    Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
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
        if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
        return;
    }

    // 타게팅 중인 경우 해당 태그에 맞는 Ability가 발동되도록 AbilityInputTagHeld(InputTag) 실행
    // ex) LMB로 공격 Abiliity 사용시AbilityInputTagHeld(InputTag) 실행
    if (bTargeting || bShiftKeyDown)
    {
        if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
        // RMB 클릭일 경우 이동기능까지 하고 종료되어야 하므로 return 사용x
    }
    else
    {
        // 키 입력 유지시간
        FollowTime += GetWorld()->GetDeltaSeconds();

        if (CursorHit.bBlockingHit) CachedDestination = CursorHit.ImpactPoint;

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
    // bAutorunning이 false인 경우 AutoRun하지 않도록 하기 위함
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
