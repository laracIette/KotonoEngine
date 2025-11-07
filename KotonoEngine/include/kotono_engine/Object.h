#pragma once
#include "generated/Object.generated.h"
#include "base_class.h"
#include <string>
#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include "Guid.h"
#include <kotono_framework/Event.h>
#include "Duration.h"
#include "Ptr.h"
#include "serialize.h"

class UPtrOwnerBase;

class KObject
{
	GENERATED_KOBJECT()

	friend class SObjectManager;

public:
	KObject(UPtrOwnerBase* ptrOwner);
	virtual ~KObject() = default;

protected:
	virtual void Init();
	virtual void Update();
	virtual void Cleanup();

public:
	const UGuid& Guid() const;
	const std::type_info& Type() const;
	bool IsConstructed() const;
	bool IsDelete() const;
	bool GetCanUpdate() const;
	const std::filesystem::path& Path() const;
	const std::string& GetName() const;
	std::string GetTypeName() const;
	KtEvent<>& GetEventCleanup();

	void SetPath(const std::filesystem::path& path);
	void SetName(const std::string& name);
	void SetCanUpdate(const bool canUpdate);
	// Stages the deletion at the end of the update
	void Delete();
	void DelayDelete(const UDuration& delay);

	// Serialize and write to the object's path
	void Serialize() const;
	// Read from the object's path and deserialize
	void Deserialize();

	// Serialize to json
	//virtual void SerializeTo(nlohmann::json& json) const;
	// Deserialize from json
	virtual void DeserializeFrom(const nlohmann::json& json);

	virtual std::string ToString() const;

protected:
	UPtrOwnerBase* const ptrOwner_;

	void Delay(const KtDelegate<>& delegate, const UDuration& delay) const;
	void Delay(KtDelegate<>&& delegate, const UDuration& delay) const;

	template <class T>
		requires std::is_base_of_v<KObject, T>
	UPtr<T> Ptr() const
	{
		return static_cast<UPtrOwner<T>*>(ptrOwner_);
	}

private:
	UGuid guid_;
	bool isConstructed_;
	bool isDelete_;
	bool canUpdate_;
	std::filesystem::path path_;
	std::string name_;
	KtEvent<> eventCleanup_;

	union
	{
		size_t initIndex_;
		size_t objectIndex_;
	};
	bool isInit_;

};

