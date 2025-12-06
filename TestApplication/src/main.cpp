#include <kotono_platform/Window.h>
#include <kotono_core/Core.h>
#ifdef EDITOR
    #include <kotono_editor/Editor.h>
#endif

int main()
{
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