#include <kotono_framework/Framework.h>
#include <kotono_framework/Window.h>
#include <kotono_engine/Engine.h>
#include <kotono_engine_interface/EngineInterface.h>

int main()
{
    Framework.Init();
    Engine.Init();
    EngineInterface.Init();

    while (!Framework.Window().GetShouldClose())
    {
        Framework.Update();
        Engine.Update();
    }

    EngineInterface.Cleanup();
    Engine.Cleanup();
    Framework.Cleanup();

	return 0;
}