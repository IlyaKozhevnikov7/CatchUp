#include "CatchUpTypes.h"
#include "CUCharacter.h"

void FDamageModifier::Process(ACUCharacter* Target)
{
	if (auto HealthComponent = Target->GetHealthComponent())
		HealthComponent->TakeDamage(Damage);
}
