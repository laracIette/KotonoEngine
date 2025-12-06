#include <kotono_graphics/Framework.h>
#include <kotono_platform/Window.h>
#include <kotono_engine/Engine.h>
#ifdef EDITOR
    #include <kotono_editor/Editor.h>
#endif

int main()
{
    Framework.Init();
    Engine.Init();
#   ifdef EDITOR
        Editor.Init();
#   endif

    while (!Window.GetShouldClose())
    {
        Framework.Update();
        Engine.Update();
#       ifdef EDITOR
            Editor.Update();
#       endif
    }

#   ifdef EDITOR
        Editor.Cleanup();
#   endif
    Engine.Cleanup();
    Framework.Cleanup();

	return 0;
}