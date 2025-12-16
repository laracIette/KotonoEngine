#include <kotono_common/Path.h>
#include <kotono_platform/Window.h>
#include <kotono_core/Core.h>
#ifdef EDITOR
    #include <kotono_editor/Editor.h>
#endif

void init_project_path()
{
    Path.projectPath_ = PROJECT_DIRECTORY;
}

int main()
{
    init_project_path();

    Core.Init();
#   ifdef EDITOR
        Editor.Init();
#   endif

    while (!Window.GetShouldClose())
    {
        Core.Update();
#       ifdef EDITOR
            Editor.Update();
#       endif
    }

#   ifdef EDITOR
        Editor.Cleanup();
#   endif
    Core.Cleanup();

	return 0;
}