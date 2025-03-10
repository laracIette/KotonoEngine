#include <kotono_framework/Framework.h>
#include "Engine.h"

KtFramework Framework;
KEngine Engine;

int main()
{
    Framework.Init();
    Engine.Init();

    while (!Framework.GetWindow().GetShouldClose())
    {
        Framework.GetInputManager().Update();
        Engine.Update();
        Framework.GetRenderer().DrawFrame();
    }

    Engine.Cleanup();
    Framework.Cleanup();

	return 0;
}