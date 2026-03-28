#pragma once
#include "generated/Object.generated.h"
#include "Guid.h"
#include "Ptr.h"
#include "serialize.h"
#include "VariableInfo.h"
#include <kotono_common/Event.h>
#include <nlohmann/json_fwd.hpp>
#include <string>

#define RegisterDelegate(Owner, Event, Instance, Function)				\
static_assert((#Owner != "Ptr()" && #Owner != "this->Ptr()") && "Please use the event's AddListener when registering and event owned by this object.");\
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

	friend class SObjectManager;
	friend class SObjectFactory;

public:
	KObject(UPtrOwnerBase* ptrOwner);
	virtual ~KObject() = default;

protected:
	/// This only occurs right before the object gets destructed
	virtual void Cleanup();

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

	/// Delete the object immediately
	void Delete() const;

	/// Serialize and write to the object's path
	virtual void Serialize() const;
	/// Read from the object's path and deserialize
	virtual void Deserialize();	

	virtual std::string ToString() const;

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
	size objectIndex_;
};
