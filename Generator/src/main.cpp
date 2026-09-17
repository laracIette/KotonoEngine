#include "Generator.h"
#include <kotono_common/PathManager.h>
#include <kotono_reflection/Reflector.h>

UPath SPathManager::projectPath_{ "" };

int main()
{
	Reflector.Reflect();
	SGenerator::GenerateUpdated();
	SGenerator::GenerateRegistrator();
	return 0;
}