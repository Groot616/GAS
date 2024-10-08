// Copyright Groot


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"
#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
    UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);

    return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
    // Super 미사용 이유 : 부모 클래스의 함수에서 log출력밖에 안함

    // 컨트롤러가 로컬 플레이어에 의해 제어되고 있는지 확인
    const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();

    if (bIsLocallyControlled)
    {
        // 클라이언트 측에서 서버로 데이터 전송
        SendMouseCursorData();
    }
    else
    {
        // 컨트롤러가 서버측에 의해 제어되고 있으므로 TargetData를 listen 해야함
        // Activate와 TargetData가 있다면 델리게이트를 진행
        const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
        const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
        AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);
        // 델리게이트를 요청하지 않았다면 아직 Activate 또는 TargetData가 서버에 도달하지 않았다는 의미이므로 대기
        const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey);
        if (!bCalledDelegate)
        {
            SetWaitingOnRemotePlayerData();
        }
    }
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
    /*
     * 예측적 스코프와 관련된 데이터 기능을 관리하는 구조체
     * 클라이언트 측에서 Ability가 실행될 때, 서버와의 지연을 고려하여 예측적으로 능력의 효과를 시뮬레이션하는데 사용
     * 예측 데이터와 관련된 정보를 저장(클라이언트는 타겟이 이동하거나 변동할 것으로 예측하며, 이를 기반으로 능력의 효과를 미리 시뮬레이션)
     */
    FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

    APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
    FHitResult CursorHit;
    PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

    // TargetData를 관리하고 전달하는데 사용되는 구조체
    FGameplayAbilityTargetDataHandle DataHandle;
    // TargetData를 동적으로 생성함
    // FGameplayAbilityTargetData_SingleTargetHit는 FGamelayAbilityTargetData 파생 클래스
    FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
    // 커서의 로케이션을 Data의 HitResult에 저장하고 이를 DataHandle에 추가함
    Data->HitResult = CursorHit;
    DataHandle.Add(Data);

    /*
     * SetServerReplicatedTargetData 서버에 TargetData를 리플리케이트하여 전송하는 함수
     * GetActivationPredictionKey() : Ability가 예측적으로 활성화될 때 클라이언트 측에서 사용하는 PredictionKey를 반환
     * PredictionKey : 아래에 자세한 설명 있음. 간략하게 서버와의 지연을 줄이기 위해 사용되는 식별자로써,
     * 				   클라이언트는 이를 사용하여 능력의 효과를 시뮬레이션하고,
     * 				   서버의 응답을 기다리면서 사용자에게 즉각적인 피드백을 제공함
     * ScopedPredictionKey : 예측적 데이터의 범위(Scope)와 유효성을 관리
                             클라이언트에서 서버로 전송되는 데이터의 예측 범위를 정의하고 동기화
     * 간단 요약 : (ScopedPredictionKey)데이터의 예측 범위를 정의하고 동기화 vs (GetActivationPredictionKey())예측적으로 처리된 데이터를 서버에 전송할 때 사용
     */
    AbilitySystemComponent->ServerSetReplicatedTargetData(
        GetAbilitySpecHandle(),
        GetActivationPredictionKey(),
        DataHandle,
        FGameplayTag(),
        AbilitySystemComponent->ScopedPredictionKey);

    // Ability가 활성화되지 않은 상태에서 Broadcast하는 것을 방지하기 위함
    if (ShouldBroadcastAbilityTaskDelegates())
    {
        ValidData.Broadcast(DataHandle);
    }
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
    // ConsumeClientReplicatedTargetData : 클라이언트가 서버로부터 수신한 타겟 데이터를 처리하여 해당 능력을 실행하거나 업데이트함
    // 클라이언트 측에서 서버로부터 받은 데이터를 바탕으로 적절한 게임플레이 로직을 실행함
    AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
    if (ShouldBroadcastAbilityTaskDelegates())
    {
        ValidData.Broadcast(DataHandle);
    }
}
