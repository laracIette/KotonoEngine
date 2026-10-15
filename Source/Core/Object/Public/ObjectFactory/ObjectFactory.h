#pragma once
#include "Ptr/ObjectPtr.h"
#include "Guid/Guid.h"
#include <Containers/Map.h>
#include <string_view>

class KObject;

class SObjectFactory final
{
public:
	using ObjectFactoryFunc = UObjectPtr<KObject>(*)();

public:
	static void Register(std::string_view className, ObjectFactoryFunc function);
	static auto Get(UGuid const& guid) -> UObjectPtr<KObject>;

private:
	static auto GetFactory(std::string_view typeName) -> UObjectPtr<KObject>;

private:
	static UMap<std::string_view, ObjectFactoryFunc> objectFactories_;
	static UMap<UGuid, UObjectPtr<KObject>> registry_;
};
