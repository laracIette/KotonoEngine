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
    isConstructed_ = true; // todo: maybe move that
}

void KObject::Update() 
{
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

bool KObject::GetCanUpdate() const
{
    return canUpdate_;
}

const std::filesystem::path KObject::Path() const
{
    return Framework.Path().Project() / "assets" / "objects" / std::format("{}.kobject", static_cast<std::string>(guid_));
}

const std::string& KObject::GetName() const
{
    return name_;
}

std::string KObject::GetTypeName() const
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

void KObject::SetCanUpdate(const bool canUpdate)
{
    canUpdate_ = canUpdate;
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

void KObject::DelayDelete(const UDuration& delay)
{
    Delay(KtDelegate(this, &KObject::Delete), delay);
}

void KObject::Serialize() const
{
    nlohmann::json json{};
    KtSerializer serializer{};
    SerializeTo(json);
    auto a = Path();
    serializer.WriteData(Path(), json);
}

void KObject::Deserialize()
{
    nlohmann::json json{};
    KtSerializer serializer{};
    serializer.ReadData(Path(), json);
    DeserializeFrom(json);
}

std::string KObject::ToString() const
{
    return name_;
}

void KObject::Delay(const KtDelegate<>& delegate, const UDuration& delay) const
{
    UPtr timer = Engine.ObjectManager().Create<KTimer>();
    timer->EventCompleted().AddListener(KtDelegate(timer.Get(), &KTimer::Delete));
    timer->EventCompleted().AddListener(delegate);
    timer->SetDuration(delay);
    timer->Start();
}

void KObject::Delay(KtDelegate<>&& delegate, const UDuration& delay) const
{
    UPtr timer = Engine.ObjectManager().Create<KTimer>();
    timer->EventCompleted().AddListener(KtDelegate(timer.Get(), &KTimer::Delete));
    timer->EventCompleted().AddListener(std::move(delegate));
    timer->SetDuration(delay);
    timer->Start();
}
