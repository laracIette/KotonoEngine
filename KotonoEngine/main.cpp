#include <kotono_framework/Framework.h>
#include <kotono_framework/Window.h>
#include <kotono_framework/InputManager.h>
#include "Engine.h"

int main()
{
    Framework.Init();
    Engine.Init();

    while (!Framework.GetWindow().GetShouldClose())
    {
        Framework.Update();
        Engine.Update();
    }

    Engine.Cleanup();
    Framework.Cleanup();

	return 0;
}