// Copyright Groot


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Aura/Public/AuraGameplayTags.h"
#include "GameplayEffectExtension.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData); 
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
    // 서버에 스폰 가능한지 확인
    // HasAuthority() : 객체가 서버에서 실행중이거나 싱글 플레이어 게임에서 실행중일 경우 true, 객체가 클라이언트에서 실행 중일 경우 false
    // 능력의 활성화 과정에 관한 정보를 가지고 있는지 확인하여 서버에서 동작하는지 체크하는 bool타입 변수
    const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
    if (!bIsServer) return;

    // GetAvatarAcorFromActorInfo() 함수를 통해 Ability를 사용하는 주체(아바타를 가져옴)
    // 주어진 액터가 ICombatInterface를 구현하고 있는지 확인하고, 인터페이스 호출 가능하도록 함
    ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
    if (CombatInterface)
    {
        const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
        FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
        // 지면으로부터 평행하게 이동하기 위함
        Rotation.Pitch = 0.f;
        // 스폰 Transform을 저장할 구조체
        FTransform SpawnTransform;
        SpawnTransform.SetLocation(SocketLocation);
        SpawnTransform.SetRotation(Rotation.Quaternion());
        // SpawnActorDeferred<AActorClass>(ActorClass, Transform, Owner, Instigator, SpawnParameters) : 액터를 생성할 때 사용되는 함수. 즉시 액터를 활성화하지 않고 필요한 설정을 완료한 후에 액터를 활성화할 수 있도록 함
        // ProjectileClass : 생성할 ActorClass
        // SpawnTransform : 생성 Transform 정보
        // GetOwningActorFromActorInfo() : 생성된 액터의 소유자
        // Cast<APawn>(GetOwningActorFromActorInfo()) : Instigator(가해자)
        // ESpawnActorCollisionHandleMethod : 충돌 처리 방식 설정
        /*
         * AlwaysSpawn : 충돌 여부에 관계없이 항상 액터를 생성
         * AdjustIfPossibleButAlwaysSpawn : 가능하면 충돌을 피하지만, 충돌하더라도 항상 생성
         * AdjustIfPossibleButSpawnIfColliding : 가능하면 충돌을 피하지만, 충돌할 경우 생성하지 않음
         * DonSpawnIfColliding : 충돌할 경우 액터를 생성하지 않음
         */
        AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
            ProjectileClass,
            SpawnTransform,
            GetOwningActorFromActorInfo(),
            Cast<APawn>(CurrentActorInfo->AvatarActor)
            /*Cast<APawn>(GetOwningActorFromActorInfo())*/,
            ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

        const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
        FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
        EffectContextHandle.SetAbility(this);
        EffectContextHandle.AddSourceObject(Projectile);
        TArray<TWeakObjectPtr<AActor>> Actors;
        Actors.Add(Projectile);
        EffectContextHandle.AddActors(Actors);
        FHitResult HitResult;
        HitResult.Location = ProjectileTargetLocation;
        EffectContextHandle.AddHitResult(HitResult);
        
        const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);

        const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();

        for (auto& Pair : DamageTypes)
        {
            // DamageTypes의 밸류값을 통해 값 전달
            const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
            // DamageTypes의 키값을 통해 태그 전달
            UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage);
        }

        Projectile->DamageEffectSpecHandle = SpecHandle;

        Projectile->FinishSpawning(SpawnTransform);
    }
}
