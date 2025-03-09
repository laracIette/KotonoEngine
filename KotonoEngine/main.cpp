#include <kotono_framework/Framework.h>
#include "Engine.h"

KtFramework Framework;
KEngine Engine;

int main()
{
    Framework.Init();
    Engine.Init();

    auto& event = Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_SPACE);
    event.AddListener(&Framework.GetWindow(), &KtWindow::OnKeySpacePressed);

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