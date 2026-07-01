#include "ProjectSettings.h"

const UPath& SProjectSettings::Path()
{
	static UPath projectSettings{ "${PROJECT_DIRECTORY}/projectSettings.json" };
	return projectSettings;
}
