#include <kotono_common/PathManager.h>
#include <kotono_reflection/Reflector.h>
#include "Generator.h"

std::filesystem::path SPathManager::projectPath_{ "" };

int main()
{
	Reflector.Reflect();
	Generator.GenerateUpdated();
	return 0;
}