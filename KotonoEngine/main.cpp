#include <kotono_framework/Framework.h>
#include <kotono_framework/Window.h>
#include <kotono_framework/Renderer.h>
#include <kotono_framework/InputManager.h>
#include "Engine.h"
#include "Time.h"
#include "ObjectManager.h"

int main()
{
    Framework.Init();
    Engine.Init();

    while (!Framework.GetWindow().GetShouldClose())
    {
        Framework.GetInputManager().Update();
        Engine.GetTime().Update();
        Engine.GetObjectManager().Update();
    }

    Engine.Cleanup();
    Framework.Cleanup();

	return 0;
}