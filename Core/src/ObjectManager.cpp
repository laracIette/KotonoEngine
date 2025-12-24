#include "ObjectManager.h"
#include "InterfaceComponent.h"
#include "InterfaceObject.h"
#include "Object.h"
#include "TimeManager.h"
#include <kotono_common/log.h>
#include <kotono_input/Keyboard.h>
#include <kotono_input/Mouse.h>
#include <kotono_io/Serializer.h>
#include <kotono_platform/Window.h>
#include <kotono_timing/Timer.h>
#include <kotono_timing/TimerManager.h>

#define KT_LOG_IMPORTANCE_LEVEL_OBJECT ELogImportanceLevel::Medium

void SObjectManager::Init()
{
	Keyboard.EventKey(EKey::Escape, EInputState::Pressed)
		.AddListener(KtDelegate(this, &SObjectManager::Quit));
	Mouse.EventButton(EButton::Left, EInputState::Pressed)
		.AddListener(KtDelegate(this, &SObjectManager::OnMouseButtonLeftPressed));

	auto& logUPSTimer{ TimerManager.GetTimer("log ups timer") };
	logUPSTimer.SetDuration(1.0f);
	logUPSTimer.SetIsRepeat(true);
	logUPSTimer.EventCompleted().AddListener(KtDelegate(this, &SObjectManager::LogUPS));
	logUPSTimer.Start();
}

void SObjectManager::Cleanup()
{
	KtPool<UPtrOwnerBase*> deletes{};
	deletes.Merge(inits_);
	deletes.Merge(objects_);
	for (int64_t i{ deletes.LastIndex() }; i >= 0; --i)
	{
		Delete(deletes[i]);
	}

	Keyboard.EventKey(EKey::Escape, EInputState::Pressed)
		.RemoveListener(KtDelegate(this, &SObjectManager::Quit));
	Mouse.EventButton(EButton::Left, EInputState::Pressed)
		.RemoveListener(KtDelegate(this, &SObjectManager::OnMouseButtonLeftPressed));
}

void SObjectManager::Register(KObject* object, UPtrOwnerBase* ptrOwner)
{
	inits_.Add(ptrOwner);
	object->initIndex_ = inits_.LastIndex();
	object->type_ = object->TypeName();
	object->SetName(std::format("{}_{}", object->TypeName(), object->Guid().ToString()));
	KT_LOG(KT_LOG_IMPORTANCE_LEVEL_OBJECT, "Core.SObjectManager::Register()", "register object %s", object->GetName().c_str());
}

void SObjectManager::Delete(UPtrOwnerBase* ptrOwner)
{
	auto* object{ static_cast<KObject*>(ptrOwner->Get()) };

	object->Cleanup();

	if (object->isInit_)
	{
		const size_t index{ object->objectIndex_ };
		if (objects_.RemoveAt(index) == KtPoolRemoveResult::ItemSwappedAndRemoved)
		{
			auto* swapped{ static_cast<KObject*>(objects_[index]->Get()) };
			swapped->objectIndex_ = index;
		}
	}
	else
	{
		const size_t index{ object->initIndex_ };
		if (inits_.RemoveAt(index) == KtPoolRemoveResult::ItemSwappedAndRemoved)
		{
			auto* swapped{ static_cast<KObject*>(inits_[index]->Get()) };
			swapped->initIndex_ = index;
		}
	}

	KT_LOG(KT_LOG_IMPORTANCE_LEVEL_OBJECT, "Core.SObjectManager::Delete()", "delete object %s", object->GetName().c_str());

	delete object;
	delete ptrOwner;
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

	KT_LOG(KT_LOG_IMPORTANCE_LEVEL_OBJECT, "Core.SObjectManager::InitObjects()", "object count %llu", objects_.size());
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

KtEvent<>& SObjectManager::EventSelectedObjectChanged()
{
	return eventSelectedObjectChanged_;
}

void SObjectManager::LogUPS() const
{
	KT_LOG(ELogImportanceLevel::High, "Core.SObjectManager::LogUPS()", "%.2f ups", 1.0f / TimeManager.AverageUpdateTime());
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
		selectedObject_ = selectedComponent->GetOwner();
	}

	if (selectedObject_)
	{
		KT_LOG(KT_LOG_COMPILE_TIME_LEVEL, "Core.SObjectManager::OnMouseButtonLeftPressed()", "selected %s", selectedObject_->GetName().c_str());
	}
}
