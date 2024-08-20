#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

// FGameplayEffectContext 구조체로부터 파생된 구조체 생성
USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
    GENERATED_BODY()

public:
    bool IsBlockedHit() const { return bIsBlockedHit; }
    bool IsCriticalHit() const { return bIsCriticalHit; }

    void SetIsBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }
    void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }

    /** Returns the actual struct used for serialization, subclass must override this! */
    virtual UScriptStruct* GetScriptStruct() const
    {
        return StaticStruct();
    }

    /** Creates a copy of this context, used to duplicate for later modifications */
    virtual FAuraGameplayEffectContext* Duplicate() const
    {
        FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
        *NewContext = *this;
        if (GetHitResult())
        {
            // Does a deep copy of the hit result
            NewContext->AddHitResult(*GetHitResult(), true);
        }
        return NewContext;
    }

    /** Custom serialization, subclasses must override this */
    virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

protected:
    UPROPERTY()
    bool bIsBlockedHit = false;

    UPROPERTY()
    bool bIsCriticalHit = false;
};

/*
 * FAuraGameplayEffectContext라는 구조체에 대해 Unreal Engine의 특성 시스템을 활용하여
 * 네트워크 직렬화와 복사 기능을 지원하도록 설정함
 * 이를 통해 FAuraGameplayEffectContext는 네트워크를 통해 데이터를 송수신할 수 있으며
 * 복사 연산을 안전하게 수행할 수 있는 구조체로 정의됨
 * Custom GameplayEffectContext를 생성하는데 필요함
 */
template<>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
    enum
    {
        WithNetSerializer = true,
        withCopy = true
    };
};