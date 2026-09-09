#include "Generator.h"
#include <kotono_common/PathManager.h>
#include <kotono_reflection/Reflector.h>

std::filesystem::path SPathManager::projectPath_{ "" };

int main()
{
	Reflector.Reflect();
	SGenerator::GenerateUpdated();
	return 0;
}