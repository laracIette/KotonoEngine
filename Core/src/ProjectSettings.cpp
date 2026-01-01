#include "ProjectSettings.h"
#include <kotono_common/PathManager.h>

std::filesystem::path SProjectSettings::Path()
{
	return PathManager.Project() / "projectSettings.json";
}
