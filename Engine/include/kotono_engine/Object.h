#pragma once
#include "generated/Object.generated.h"
#include "base_class.h"
#include <string>
#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include "Guid.h"
#include <kotono_common/Event.h>
#include "Ptr.h"
#include "serialize.h"

class UPtrOwnerBase;

class KObject
{
	GENERATED_KOBJECT()

	friend class SObjectManager;
	friend class SObjectFactory;
	friend class SGame;

public:
	KObject(UPtrOwnerBase* ptrOwner);
	virtual ~KObject() = default;

protected:
	virtual void Init();
	virtual void Cleanup();

public:
	const UGuid& Guid() const;
	const std::type_info& Type() const;
	const std::filesystem::path Path() const;
	bool IsConstructed() const;
	bool IsDelete() const;
	const std::string& GetName() const;
	std::string TypeName() const;
	KtEvent<>& GetEventCleanup();

	void SetName(const std::string& name);

	// Stages the deletion at the end of the update
	void Delete();
	//void DelayDelete(const UDuration& delay);

	// Serialize and write to the object's path
	void Serialize() const;
	// Read from the object's path and deserialize
	void Deserialize();	

	virtual std::string ToString() const;

protected:
	//void Delay(const KtDelegate<>& delegate, const UDuration& delay) const;
	//void Delay(KtDelegate<>&& delegate, const UDuration& delay) const;

	template <class T>
		requires std::is_base_of_v<KObject, T>
	UPtr<T> Ptr() const
	{
		return static_cast<UPtrOwner<T>*>(ptrOwner_);
	}

private:
	UPtrOwnerBase* const ptrOwner_;
	SERIALIZE UGuid guid_;
	SERIALIZE std::string type_;
	SERIALIZE std::string name_;
	bool isConstructed_;
	bool isDelete_;
	KtEvent<> eventCleanup_;

	union
	{
		size_t initIndex_;
		size_t objectIndex_;
	};
	bool isInit_;

};
