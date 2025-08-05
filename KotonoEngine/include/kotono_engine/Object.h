#pragma once
#include "base_class.h"
#include <string>
#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include "Guid.h"
#include <kotono_framework/Event.h>

class KObject
{
	friend class SObjectManager;

public:
	KObject();
	virtual ~KObject() = default;

protected:
	virtual void Init();
	virtual void Update();
	virtual void Cleanup();

public:
	const UGuid& GetGuid() const;
	const bool GetIsConstructed() const;
	const std::filesystem::path& GetPath() const;
	const std::string& GetName() const;
	const bool GetIsDelete() const;
	const std::string GetTypeName() const;
	KtEvent<>& GetEventCleanup();

	void SetPath(const std::filesystem::path& path);
	void SetName(const std::string& name);
	// Stages the deletion at the end of the update
	void Delete();

	// Serialize and write to the object's path
	void Serialize() const;
	// Read from the object's path and deserialize
	void Deserialize();

	// Serialize to json
	virtual void SerializeTo(nlohmann::json& json) const;
	// Deserialize from json
	virtual void DeserializeFrom(const nlohmann::json& json);

protected:
	void Delay(const KtDelegate<>& delegate, const float delay) const;
	void Delay(KtDelegate<>&& delegate, const float delay) const;

private:
	UGuid guid_;
	bool isConstructed_;
	std::filesystem::path path_;
	std::string name_;
	bool isDelete_;
	KtEvent<> eventCleanup_;
	size_t objectIndex_;
};

