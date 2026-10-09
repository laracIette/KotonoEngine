#pragma once
#include "Ptr.h"
#include "Guid/Guid.h"
#include <Containers/Map.h>
#include <string_view>

class KObject;

class SObjectFactory final
{
public:
	using ObjectFactoryFunc = UPtr<KObject>(*)();

public:
	static void Register(std::string_view className, ObjectFactoryFunc function);
	static auto Get(UGuid const& guid) -> UPtr<KObject>;

private:
	static auto GetFactory(std::string_view typeName) -> UPtr<KObject>;

private:
	static UMap<std::string_view, ObjectFactoryFunc> objectFactories_;
	static UMap<UGuid, UPtr<KObject>> registry_;
};
