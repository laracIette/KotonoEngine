#pragma once
#include "generated/Object.generated.h"
#include "Guid.h"
#include "Ptr.h"
#include "VariableInfo.h"
#include <kotono_common/Event.h>
#include <kotono_common/log.h>
#include <kotono_io/serialize_base.h>
#include <nlohmann/json_fwd.hpp>
#include <string>
#include <unordered_set>
#include "ObjectFactory.h"

#define RegisterDelegate(Owner, Event, Instance, Function)				\
static_assert(std::string_view(#Owner) != std::string_view("Ptr()") && std::string_view(#Owner) != std::string_view("this->Ptr()"), "Please use the event's AddListener when registering an event owned by this object.");	\
Event._AddListener(_MAKE_DELEGATE(Instance, Function));					\
unregisterDelegates_.push_back(											\
	[=]()																\
	{																	\
		if (Owner)														\
		{																\
			Event._RemoveListener(_MAKE_DELEGATE(Instance, Function));	\
		}																\
	}																	\
)

class UPath;

class KObject
{
	GENERATED_KOBJECT()

	friend class SObjectFactory;

	template <typename T>
	friend UPtr<T> Create();

public:
	KObject(UPtrOwnerBase* ptrOwner);
	virtual ~KObject();

	void OnConstructed();

public:
	const UGuid& Guid() const;
	const std::type_info& Type() const;
	UPath Path() const;
	bool IsConstructed() const;
	const std::string& GetName() const;
	std::string TypeName() const;

	/// Read json from disk
	nlohmann::json ReadJson() const;
	/// Write the object to json
	nlohmann::json WriteJson() const;

	void SetName(const std::string& name);

	/// Cleanup and delete the object immediately
	void Delete();

	/// Serialize and write to the object's path
	virtual void Serialize() const;
	/// Read from the object's path and deserialize
	virtual void Deserialize();	

	virtual std::string ToString() const;

public:
	static UPtr<KObject> Deserialize(const nlohmann::json& json);

protected:
	template <typename TObj, typename ...Args>
	void _RegisterDelegate(const UPtr<TObj>& owner, UEvent<Args...>& event, const std::function<UDelegate<Args...>()>& makeDelegateFunc)
	{
		event._AddListener(makeDelegateFunc());

		unregisterDelegates_.push_back(
			[owner, event, makeDelegateFunc]()
			{
				if (owner)
				{
					event._RemoveListener(makeDelegateFunc());
				}
			}
		);
	}

protected:
	UPtrOwnerBase* const ptrOwner_;

	std::vector<std::function<void()>> unregisterDelegates_;

private:
	SERIALIZE UGuid guid_;
	SERIALIZE std::string type_;
	SERIALIZE std::string name_;
	bool isConstructed_;

#if defined(_DEBUG)
public:
	static void CheckDebugRegistry();

private:
	static std::unordered_set<UPtr<KObject>> debugRegistry_;
#endif
};

template <std::derived_from<KObject> T>
UPtr<T> Create()
{
	auto* ptrOwner{ new UPtrOwner<T>() };
	auto* object{ new T(ptrOwner) };
	object->OnConstructed();
	UPtr<T> ptr{ ptrOwner };
	return ptr;
}

template <std::derived_from<KObject> T>
struct USerialize<UPtr<T>>
{
	void operator()(nlohmann::json& json, const UPtr<T>& v) const
	{
		if (v)
		{
			USerialize<UGuid>{}(json, v->Guid());
			v->Serialize();
		}
	}
};

template <std::derived_from<KObject> T>
struct UDeserialize<UPtr<T>>
{
	void operator()(const nlohmann::json& json, UPtr<T>& v) const
	{
		v = TryCast<T>(KObject::Deserialize(json));
	}
};
