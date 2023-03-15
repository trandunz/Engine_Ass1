#include "Engine_Ass1GameMode.h"
#include "Engine_Ass1Character.h"
#include "UObject/ConstructorHelpers.h"

AEngine_Ass1GameMode::AEngine_Ass1GameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_AssCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
