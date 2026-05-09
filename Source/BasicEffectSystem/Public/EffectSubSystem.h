// EffectSubsystem.h
#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EffectSubsystem.generated.h"

class UEffectComponent;

UCLASS()
class BASICEFFECTSYSTEM_API UEffectSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	void RegisterComponent(UEffectComponent* Comp);
	void UnregisterComponent(UEffectComponent* Comp);
	
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UEffectSubsystem, STATGROUP_Tickables); }
	virtual bool IsTickable() const override { return !RegisteredComponents.IsEmpty(); }

private:
	UPROPERTY() TArray<TObjectPtr<UEffectComponent>> RegisteredComponents;
};