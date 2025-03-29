#pragma once
#include "base_class.h"
#include <string>
#include "Guid.h"
#include <nlohmann/json_fwd.hpp>
class OObject
{
public:
	const UGuid& GetGuid() const;
	const std::string& GetName() const;

	void SetName(const std::string& name);

	const nlohmann::json Serialize() const;
	void Deserialize(const nlohmann::json& data);

private:
	UGuid _guid;
	std::string _name;
};

