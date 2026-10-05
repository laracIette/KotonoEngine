#include "Generator/Generator.h"

#include <Path/PathManager.h>
#include <Reflector/Reflector.h>

UPath SPathManager::projectPath_{ "" };

int main()
{
	Reflector.Reflect();
	SGenerator::GenerateUpdated();
	SGenerator::GenerateRegistrator();
	return 0;
}