#include <kotono_framework/Framework.h>
#include <kotono_framework/Window.h>
#include <kotono_engine/Engine.h>
#include <kotono_editor/Editor.h>

int main()
{
    Framework.Init();
    Engine.Init();
    Editor.Init();

    while (!Framework.Window().GetShouldClose())
    {
        Engine.Update();
    }

    Editor.Cleanup();
    Engine.Cleanup();
    Framework.Cleanup();

	return 0;
}