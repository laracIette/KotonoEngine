#include "Generator/Generator.h"

#include <Path/Path.h>
#include <Reflector/Reflector.h>

std::string_view UPath::enginePath_{ ENGINE_DIRECTORY };
std::string_view UPath::projectPath_{ "" };

int main()
{
	Reflector.Reflect();
	SGenerator::GenerateUpdated();
	SGenerator::GenerateRegistrator();
	return 0;
}