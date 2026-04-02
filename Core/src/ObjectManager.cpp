#include "ObjectManager.h"
#include "Object.h"
#include "TimeManager.h"
#include <kotono_common/log.h>
#include <kotono_input/Keyboard.h>
#include <kotono_platform/Window.h>
#include <kotono_timing/Timer.h>
#include <kotono_timing/TimerManager.h>

#define KT_LOG_IMPORTANCE_LEVEL_OBJECT ELogImportanceLevel::Medium

void SObjectManager::Init()
{
	Keyboard.EventKey(EKey::Escape, EInputState::Pressed).AddListener(this, &SObjectManager::Quit);

	auto& logUPSTimer{ TimerManager.GetTimer("log ups timer") };
	logUPSTimer.SetDuration(1.0f);
	logUPSTimer.SetIsRepeat(true);
	logUPSTimer.EventCompleted().AddListener(this, &SObjectManager::LogUPS);
	logUPSTimer.Start();
}

void SObjectManager::Cleanup()
{
	Keyboard.EventKey(EKey::Escape, EInputState::Pressed).RemoveListener(this, &SObjectManager::Quit);
}

void SObjectManager::Quit()
{
	Window.SetShouldClose(true);
}

UPtr<KObject>& SObjectManager::GetSelectedObject()
{
	return selectedObject_;
}

void SObjectManager::SetSelectedObject(const UPtr<KObject>& object)
{
	if (selectedObject_ == object)
	{
		return;
	}

	selectedObject_ = object;
	eventSelectedObjectChanged_.Broadcast();
}

UEvent<>& SObjectManager::EventSelectedObjectChanged()
{
	return eventSelectedObjectChanged_;
}

void SObjectManager::LogUPS() const
{
	KT_LOG(ELogImportanceLevel::High, "Core", "{:.2f} ups", 1.0f / TimeManager.AverageUpdateTime());
}
