#include "Object.h"
#include <nlohmann/json.hpp>
#include <kotono_framework/Serializer.h>
#include "Engine.h"
#include "ObjectManager.h"
#include "Timer.h"
#include "log.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/Path.h>

KObject::KObject(UPtrOwnerBase* ptrOwner) :
    ptrOwner_(ptrOwner),
    name_(guid_)
{
    ptrOwner_->Set(this);
}

void KObject::Init()
{
    type_ = TypeName();
    isConstructed_ = true; // todo: maybe move that
}

void KObject::Cleanup() 
{
    eventCleanup_.Broadcast();
}

const UGuid& KObject::Guid() const
{
    return guid_;
}

const std::type_info& KObject::Type() const
{
    return typeid(*this);
}

bool KObject::IsConstructed() const
{
    return isConstructed_;
}

bool KObject::IsDelete() const
{
    return isDelete_;
}

const std::filesystem::path KObject::Path() const
{
    return Framework.Path().Project() / "assets" / "objects" / std::format("{}.kobject", guid_.ToString());
}

const std::string& KObject::GetName() const
{
    return name_;
}

std::string KObject::TypeName() const
{
    std::string_view name = Type().name();
    return std::string(name.substr(6));
}

KtEvent<>& KObject::GetEventCleanup()
{
    return eventCleanup_;
}

void KObject::SetName(const std::string& name)
{
    name_ = name;
}

void KObject::Delete()
{
    if (isDelete_)
    {
        return;
    }
    isDelete_ = true;
    Engine.ObjectManager().Delete(ptrOwner_);
}

//void KObject::DelayDelete(const UDuration& delay)
//{
//    Delay(KtDelegate(this, &KObject::Delete), delay);
//}

void KObject::Serialize() const
{
    nlohmann::json json{};
    SerializeTo(json);
    KtSerializer::Serialize(json, Path());
}

void KObject::Deserialize()
{
    nlohmann::json json{};
    KtSerializer::Deserialize(json, Path());
    DeserializeFrom(json);
}

std::string KObject::ToString() const
{
    return name_;
}

//void KObject::Delay(const KtDelegate<>& delegate, const UDuration& delay) const
//{
//    UTimer timer{};
//    timer.duration = delay;
//    timer.eventCompleted.AddListener(KtDelegate(&timer, &UTimer::Delete));
//    timer.eventCompleted.AddListener(delegate);
//    timer->Start();
//}
//
//void KObject::Delay(KtDelegate<>&& delegate, const UDuration& delay) const
//{
//    UPtr timer = Engine.ObjectManager().Create<UTimer>();
//    timer->EventCompleted().AddListener(KtDelegate(timer.Get(), &UTimer::Delete));
//    timer->EventCompleted().AddListener(std::move(delegate));
//    timer->SetDuration(delay);
//    timer->Start();
//}
