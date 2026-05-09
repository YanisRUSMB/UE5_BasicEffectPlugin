#include "EffectSubsystem.h"
#include "EffectComponent.h"

void UEffectSubsystem::RegisterComponent(UEffectComponent* Comp)
{
	RegisteredComponents.AddUnique(Comp);
}

void UEffectSubsystem::UnregisterComponent(UEffectComponent* Comp)
{
	RegisteredComponents.Remove(Comp);
}

void UEffectSubsystem::Tick(float DeltaTime)
{
	for (UEffectComponent* Comp : RegisteredComponents)
	{
		if (IsValid(Comp))
			Comp->TickEffects(DeltaTime);
	}
}