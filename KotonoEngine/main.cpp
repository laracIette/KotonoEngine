#include <kotono_framework/Framework.h>

KtFramework Framework;

int main()
{
    Framework.Init();

    auto& event = Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_SPACE);
    event.AddListener(&Framework.GetWindow(), &KtWindow::OnKeySpacePressed);

    while (!Framework.GetWindow().GetShouldClose())
    {
        Framework.GetInputManager().GetKeyboard().Update();
        Framework.GetRenderer().DrawFrame();
    }

    vkDeviceWaitIdle(Framework.GetContext().GetDevice());

    Framework.Cleanup();

	return 0;
}