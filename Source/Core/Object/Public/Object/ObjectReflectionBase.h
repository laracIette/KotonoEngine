#pragma once
#include "ObjectBase.h"

#include "ObjectFactory/ObjectFactory.h"
#include "Ptr/ObjectPtr.h"
#include "VariableInfo.h"
#include <nlohmann/json_fwd.hpp>
#include <vector>

#define SERIALIZE
#define ABSTRACT

class KObjectReflectionBase_Class
{
public:
	virtual ~KObjectReflectionBase_Class() = default;

	virtual auto Instantiate() const -> UObjectPtr<KObjectBase> = 0;

	template <std::derived_from<KObjectBase> T>
	auto Instantiate() const -> UObjectPtr<T>
	{
		return TryCast<T>(Instantiate());
	}
};

class KObjectReflectionBase : public KObjectBase
{
public:
	KObjectReflectionBase();
	~KObjectReflectionBase() override;
	
	virtual void SerializeTo(nlohmann::json& json) const;
	virtual void DeserializeFrom(nlohmann::json const& json);
	virtual void PopulateMemberVariables(std::vector<UVariableInfo>& variables) const;
	
	auto GetMemberVariables() const -> std::vector<UVariableInfo>;
	
	virtual auto GetClass() const -> UPtr<KObjectReflectionBase_Class const> = 0;
	
protected:
	UObjectPtr<KObjectBase> Pointer;
};
