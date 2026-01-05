#pragma once
#include "generated/Object.generated.h"
#include "Guid.h"
#include "Ptr.h"
#include "serialize.h"
#include "VariableInfo.h"
#include <nlohmann/json_fwd.hpp>
#include <string>

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
	virtual void Cleanup();

public:
	const UGuid& Guid() const;
	const std::type_info& Type() const;
	UPath Path() const;
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
	UPtrOwnerBase* const ptrOwner_;

private:
	SERIALIZE UGuid guid_;
	SERIALIZE std::string type_;
	SERIALIZE std::string name_;
	bool isConstructed_;
	size objectIndex_;
};
