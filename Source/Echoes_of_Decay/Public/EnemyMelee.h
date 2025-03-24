#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "EnemyMelee.generated.h"

UCLASS()
class ECHOES_OF_DECAY_API AEnemyMelee : public AEnemyBase
{
	GENERATED_BODY()
	
protected:
	virtual void AttackPlayer() override;
};
