#include "ProjectSettings.h"
#include <kotono_common/Path.h>

std::filesystem::path SProjectSettings::Path()
{
	return KtPath::Project() / "projectSettings.json";
}
