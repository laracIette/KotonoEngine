#include "ObjectManager.h"
#include <kotono_framework/Framework.h>

void KObjectManager::Init()
{
	Framework.GetInputManager().GetKeyboard()
		.GetEvent(KT_KEY_ESCAPE, KT_INPUT_STATE_PRESSED)
		.AddListener(this, &KObjectManager::Quit);
}

void KObjectManager::Update()
{
}

void KObjectManager::Cleanup()
{
}

void KObjectManager::Quit()
{
	Framework.GetWindow().Close();
}
