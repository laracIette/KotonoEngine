#pragma once
#include "base_class.h"
#include "generated/Object.generated.h"
#include "Guid.h"
#include "serialize.h"
#include <filesystem>
#include <kotono_common/Event.h>
#include <kotono_common/Ptr.h>
#include <nlohmann/json_fwd.hpp>
#include <string>

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
	const std::string& GetName() const;
	std::string TypeName() const;

	// Read json from disk
	nlohmann::json ReadJson() const;
	// Write the object to json
	nlohmann::json WriteJson() const;

	void SetName(const std::string& name);

	// Delete the object immediately
	void Delete() const;

	// Serialize and write to the object's path
	virtual void Serialize() const;
	// Read from the object's path and deserialize
	virtual void Deserialize();	

	virtual std::string ToString() const;

protected:
	template <std::derived_from<KObject> T>
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

	union
	{
		size_t initIndex_;
		size_t objectIndex_;
	};
	bool isInit_;

};
