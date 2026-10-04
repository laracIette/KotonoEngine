#pragma once
#include "Guid.h"
#include <functional>
#include <string_view>
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

public:
	static auto Get(UGuid const& guid) -> UPtr<KObject>;

private:
	static void Register(std::string_view className, ObjectFactoryFunc const& function);
	static auto GetFactory(std::string_view typeName) -> UPtr<KObject>;

private:
	static std::unordered_map<std::string_view, ObjectFactoryFunc> objectFactories_;
	static std::unordered_map<UGuid, UPtr<KObject>> registry_;
};

struct UAutoRegister final
{
	UAutoRegister(std::string_view className, SObjectFactory::ObjectFactoryFunc const& creator);
};
