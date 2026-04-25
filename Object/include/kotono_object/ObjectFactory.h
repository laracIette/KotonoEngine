#pragma once
#include <functional>
#include <string_view>
#include "Guid.h"

template <class T>
class UPtr;
class KObject;
/// This class uses the singleton pattern because of the UAutoRegister, 
/// get the instance using SObjectFactory::Get()
class SObjectFactory final
{
private:
	friend struct UAutoRegister;

public:
	using ObjectFactoryFunc = std::function<UPtr<KObject>()>;

	static SObjectFactory& Get();

public:
	UPtr<KObject> Get(const UGuid& guid);

private:
	void Register(const std::string_view className, const ObjectFactoryFunc& function);
	UPtr<KObject> GetFactory(const std::string_view typeName) const;

private:
	std::unordered_map<std::string_view, ObjectFactoryFunc> objectFactories_;
	std::unordered_map<UGuid, UPtr<KObject>> registry_;
};

struct UAutoRegister final
{
	UAutoRegister(const std::string_view className, const SObjectFactory::ObjectFactoryFunc& creator);
};
