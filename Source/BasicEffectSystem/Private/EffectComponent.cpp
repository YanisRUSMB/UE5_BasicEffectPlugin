#include "EffectComponent.h"
#include "EffectSubsystem.h"
#include "IStatable.h"
#include "Engine/GameInstance.h"

UEffectComponent::UEffectComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UEffectComponent::BeginPlay()
{
    Super::BeginPlay();
    if (auto* Sub = GetOwner()->GetGameInstance()->GetSubsystem<UEffectSubsystem>())
        Sub->RegisterComponent(this);
}

void UEffectComponent::EndPlay(const EEndPlayReason::Type Reason)
{
    RemoveAllEffects();
    if (auto* Sub = GetOwner()->GetGameInstance()->GetSubsystem<UEffectSubsystem>())
        Sub->UnregisterComponent(this);
    Super::EndPlay(Reason);
}

void UEffectComponent::ApplyEffect(const FEffectData& EffectData)
{
    if (EffectData.Type == EEffectType::Instant)
    {
        ApplyInstant(EffectData);
        return;
    }

    // Stacking : on cherche un effet existant du même ID
    FActiveEffect* Existing = ActiveEffects.FindByPredicate(
        [&](const FActiveEffect& E){ return E.Data.EffectID == EffectData.EffectID; });

    if (Existing && Existing->CurrentStacks < EffectData.MaxStacks)
    {
        Existing->CurrentStacks++;
        Existing->RemainingTime = EffectData.Duration;
    }
    else if (!Existing)
    {
        FActiveEffect New;
        New.Data           = EffectData;
        New.RemainingTime  = EffectData.Duration;
        New.CurrentStacks  = 1;
        New.InstanceID     = FGuid::NewGuid();

        ApplyModifier(EffectData);
        ActiveEffects.Add(New);
        OnEffectApplied.Broadcast(New);
    }
}

void UEffectComponent::RemoveEffect(FName EffectID)
{
    int32 Idx = ActiveEffects.IndexOfByPredicate(
        [&](const FActiveEffect& E){ return E.Data.EffectID == EffectID; });

    if (Idx != INDEX_NONE)
    {
        RevertModifier(ActiveEffects[Idx]);
        OnEffectRemoved.Broadcast(ActiveEffects[Idx]);
        ActiveEffects.RemoveAt(Idx);
    }
}

void UEffectComponent::RemoveAllEffects()
{
    for (const FActiveEffect& E : ActiveEffects)
        RevertModifier(E);
    ActiveEffects.Empty();
}

bool UEffectComponent::HasEffect(FName EffectID) const
{
    return ActiveEffects.ContainsByPredicate(
        [&](const FActiveEffect& E){ return E.Data.EffectID == EffectID; });
}

void UEffectComponent::TickEffects(float DeltaTime)
{
    for (int32 i = ActiveEffects.Num() - 1; i >= 0; --i)
    {
        FActiveEffect& E = ActiveEffects[i];
        if (E.Data.Duration <= 0.f) continue;

        E.RemainingTime -= DeltaTime;
        if (E.RemainingTime <= 0.f)
        {
            RevertModifier(E);
            OnEffectExpired.Broadcast(E);
            ActiveEffects.RemoveAt(i);
        }
    }
}


void UEffectComponent::ApplyInstant(const FEffectData& Data)
{
    if (IStatable* S = GetStatable())
        S->ModifyStat(Data.StatTarget, Data.ModifierOp, Data.Value);
}

void UEffectComponent::ApplyModifier(const FEffectData& Data)
{
    if (IStatable* S = GetStatable())
        S->ModifyStat(Data.StatTarget, Data.ModifierOp, Data.Value);
}

void UEffectComponent::RevertModifier(const FActiveEffect& Effect)
{
    if (IStatable* S = GetStatable())
    {
        float Inverse = (Effect.Data.ModifierOp == EModifierOp::Multiply)
            ? 1.f / Effect.Data.Value
            : -Effect.Data.Value;
        S->ModifyStat(Effect.Data.StatTarget, Effect.Data.ModifierOp, Inverse);
    }
}

IStatable* UEffectComponent::GetStatable() const
{
    return Cast<IStatable>(GetOwner());
}