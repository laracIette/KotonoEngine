#include "ObjectManager.h"
#include <kotono_framework/Framework.h>
#include <kotono_timing/TimeManager.h>
#include <kotono_timing/Timer.h>
#include <kotono_platform/Window.h>
#include <kotono_input/InputManager.h>
#include "log.h"
#include "Object.h"
#include "TimeManager.h"
#include "Timer.h"
#include "InterfaceObject.h"
#include "InterfaceComponent.h"
#include "Interface.h"
#include <nlohmann/json.hpp>
#include <kotono_io/Serializer.h>

#define KT_LOG_IMPORTANCE_LEVEL_OBJECT KT_LOG_IMPORTANCE_LEVEL_NONE

void SObjectManager::Init()
{
	InputManager.Keyboard().EventKey(KT_KEY_ESCAPE, KT_INPUT_STATE_PRESSED)
		.AddListener(KtDelegate(this, &SObjectManager::Quit));
	InputManager.Mouse().EventButton(KT_BUTTON_LEFT, KT_INPUT_STATE_PRESSED)
		.AddListener(KtDelegate(this, &SObjectManager::OnMouseButtonLeftPressed));

	auto& logUPSTimer{ Framework.TimeManager().GetTimer("log ups timer") };
	logUPSTimer.SetDuration(1.0f);
	logUPSTimer.SetIsRepeat(true);
	logUPSTimer.EventCompleted().AddListener(KtDelegate(this, &SObjectManager::LogUPS));
	logUPSTimer.Start();
}

void SObjectManager::Cleanup()
{
	KtPool<UPtrOwnerBase*> objects{};
	objects.Merge(inits_);
	objects.Merge(objects_);
	for (auto* ptr : objects)
	{
		auto* object{ static_cast<KObject*>(ptr->Get()) };
		object->Cleanup();
	}
	for (auto* ptr : objects)
	{
		auto* object{ static_cast<KObject*>(ptr->Get()) };
		KT_LOG_KE(KT_LOG_IMPORTANCE_LEVEL_OBJECT, "DEL object %s", object->GetName().c_str());
		delete object;
		delete ptr;
	}
	deletes_.Clear();

	InputManager.Keyboard().EventKey(KT_KEY_ESCAPE, KT_INPUT_STATE_PRESSED)
		.RemoveListener(KtDelegate(this, &SObjectManager::Quit));
	InputManager.Mouse().EventButton(KT_BUTTON_LEFT, KT_INPUT_STATE_PRESSED)
		.RemoveListener(KtDelegate(this, &SObjectManager::OnMouseButtonLeftPressed));
}

void SObjectManager::Register(KObject* object, UPtrOwnerBase* ptrOwner)
{
	inits_.Add(ptrOwner);
	object->initIndex_ = inits_.LastIndex();
	object->type_ = object->TypeName();
	object->SetName(std::format("{}_{}", object->TypeName(), object->Guid().ToString()));
	KT_LOG_KE(KT_LOG_IMPORTANCE_LEVEL_OBJECT, "REG object %s", object->GetName().c_str());
}

void SObjectManager::Delete(UPtrOwnerBase* ptrOwner)
{
	deletes_.Add(ptrOwner);
}

void SObjectManager::Quit()
{
	Window.SetShouldClose(true);
}

void SObjectManager::InitObjects()
{
	if (inits_.Empty())
	{
		return;
	}

	for (size_t i{ 0 }; i < inits_.size(); ++i)
	{
		auto* ptr{ inits_[i] };
		auto* object{ static_cast<KObject*>(ptr->Get()) };
		object->Init();
		object->isInit_ = true;

		objects_.Add(ptr);
		object->objectIndex_ = objects_.LastIndex();
	}
	inits_.Clear();

	KT_LOG_KE(KT_LOG_IMPORTANCE_LEVEL_OBJECT, "object count %llu", objects_.size());
}

void SObjectManager::DeleteObjects()
{
	if (deletes_.Empty())
	{
		return;
	}

	for (size_t i{ 0 }; i < deletes_.size(); ++i)
	{
		auto* ptr{ deletes_[i] };
		auto* object{ static_cast<KObject*>(ptr->Get()) };

		object->Cleanup();

		if (object->isInit_)
		{
			const size_t index{ object->objectIndex_ };
			if (objects_.RemoveAt(index) == KtPoolRemoveResult::ItemSwappedAndRemoved)
			{
				auto* object{ static_cast<KObject*>(objects_[index]->Get()) };
				object->objectIndex_ = index;
			}
		}
		else
		{
			const size_t index{ object->initIndex_ };
			if (inits_.RemoveAt(index) == KtPoolRemoveResult::ItemSwappedAndRemoved)
			{
				auto* object{ static_cast<KObject*>(inits_[index]->Get()) };
				object->initIndex_ = index;
			}
		}
	}
	for (auto* ptr : deletes_)
	{
		auto* object{ static_cast<KObject*>(ptr->Get()) };
		KT_LOG_KE(KT_LOG_IMPORTANCE_LEVEL_OBJECT, "DEL object %s", object->GetName().c_str());
		delete object;
		delete ptr;
	}

	deletes_.Clear();
}

const UPtr<KObject>& SObjectManager::SelectedObject() const
{
	return selectedObject_;
}

void SObjectManager::LogUPS() const
{
	KT_LOG_KE(KT_LOG_IMPORTANCE_LEVEL_HIGH, "%.2f ups", 1.0f / Framework.TimeManager().AverageUpdateTime());
}

void SObjectManager::OnMouseButtonLeftPressed()
{
	KInterfaceComponent* selectedComponent{ nullptr };
	for (const auto& ptr : objects_)
	{
		auto* object{ static_cast<KObject*>(ptr->Get()) };
		if (auto* asInterfaceComponent = dynamic_cast<KInterfaceComponent*>(object))
		{
			if (!asInterfaceComponent->IsHovered())
			{
				continue;
			}
			if (!selectedComponent || asInterfaceComponent->GetLayer() > selectedComponent->GetLayer())
			{
				selectedComponent = asInterfaceComponent;
			}
		}
	}

	if (selectedComponent)
	{
		selectedObject_ = selectedComponent->Owner();
	}

	if (selectedObject_)
	{
		KT_LOG_KE(KT_LOG_COMPILE_TIME_LEVEL, "selected %s", selectedObject_->GetName().c_str());
	}
}
