#pragma once
#include <functional>
#include <string_view>
#include "Guid.h"

template <class T>
class UPtr;
class KObject;

class SObjectFactory final
{
private:
	using ObjectFactoryFunc = std::function<UPtr<KObject>()>;

public:
	SObjectFactory(); 

	UPtr<KObject> Get(const UGuid& guid);

private:
	const std::unordered_map<std::string_view, ObjectFactoryFunc> objectFactories_;
	std::unordered_map<UGuid, UPtr<KObject>> registry_;

	UPtr<KObject> GetFactory(const std::string_view typeName) const;
};

inline SObjectFactory ObjectFactory;
