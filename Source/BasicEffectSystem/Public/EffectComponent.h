#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EffectTypes.h"
#include "IStatable.h"
#include "EffectComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectApplied,  const FActiveEffect&, Effect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectRemoved,  const FActiveEffect&, Effect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectExpired,  const FActiveEffect&, Effect);

UCLASS(ClassGroup=(EffectSystem), meta=(BlueprintSpawnableComponent))
class BASICEFFECTSYSTEM_API UEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEffectComponent();
	
	UFUNCTION(BlueprintCallable, Category="Effects")
	void ApplyEffect(const FEffectData& EffectData);

	UFUNCTION(BlueprintCallable, Category="Effects")
	void RemoveEffect(FName EffectID);

	UFUNCTION(BlueprintCallable, Category="Effects")
	void RemoveAllEffects();

	UFUNCTION(BlueprintPure, Category="Effects")
	bool HasEffect(FName EffectID) const;

	UFUNCTION(BlueprintPure, Category="Effects")
	TArray<FActiveEffect> GetActiveEffects() const { return ActiveEffects; }
	
	UPROPERTY(BlueprintAssignable) FOnEffectApplied OnEffectApplied;
	UPROPERTY(BlueprintAssignable) FOnEffectRemoved OnEffectRemoved;
	UPROPERTY(BlueprintAssignable) FOnEffectExpired OnEffectExpired;
	
	void TickEffects(float DeltaTime);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;

private:
	UPROPERTY() TArray<FActiveEffect> ActiveEffects;

	void ApplyInstant(const FEffectData& Data);
	void ApplyModifier(const FEffectData& Data);
	void RevertModifier(const FActiveEffect& Effect);
	
	IStatable* GetStatable() const;
};