#include "Enemies/EnemyHealthBar.h"
#include "Components/ProgressBar.h"

void UEnemyHealthBar::UpdateHealthBar(float CurrentHealth, float MaxHealth)
{
	HealthBar->SetPercent(CurrentHealth / MaxHealth);
}
