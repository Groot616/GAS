// Copyright Groot


#include "Input/AuraInputConfig.h"
#include "InputAction.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
    // 루프를 통한 AbilityInputAction 배열 순회
    for (const FAuraInputAction& Action : AbilityInputActions)
    {
        // Action.InputAction 유효성 검사 && Action.InputTag가 함수의 파라미터와 동일한지 확인
        if (Action.InputAction && Action.InputTag == InputTag)
        {
            return Action.InputAction;
        }
    }

    // 오류 발생시 로그 출력
    if (bLogNotFound)
    {
        UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputAction for InputTag [%s], on InputConfig [%s]"), *InputTag.ToString(), *GetNameSafe(this));
    }

    return nullptr;
}
