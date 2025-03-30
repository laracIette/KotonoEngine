#pragma once
#include "base_class.h"
#include <string>
#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include "Guid.h"
class OObject
{
public:
	virtual void Init();;
	virtual void Update();;
	virtual void Cleanup();;

	const UGuid& GetGuid() const;
	const std::filesystem::path& GetPath() const;
	const std::string& GetName() const;
	const bool GetIsDelete() const;

	void SetPath(const std::filesystem::path& path);
	void SetName(const std::string& name);
	void SetIsDelete(const bool isDelete);

	void Serialize() const;
	void Deserialize();

protected:
	virtual void Serialize(nlohmann::json& json) const;
	virtual void Deserialize(const nlohmann::json& json);

private:
	UGuid _guid;
	std::filesystem::path _path;
	std::string _name;
	bool _isDelete;
};

