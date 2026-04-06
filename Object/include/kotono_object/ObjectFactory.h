#pragma once
#include <functional>
#include <string_view>
#include "Guid.h"

template <class T>
class UPtr;
class KObject;

class SObjectFactory final
{
public:
	using ObjectFactoryFunc = std::function<UPtr<KObject>()>;

public:
	UPtr<KObject> Get(const UGuid& guid);

	void _Register(const std::string_view className, const ObjectFactoryFunc& function);

private:
	std::unordered_map<std::string_view, ObjectFactoryFunc> objectFactories_;
	std::unordered_map<UGuid, UPtr<KObject>> registry_;

	UPtr<KObject> GetFactory(const std::string_view typeName) const;
};

inline SObjectFactory ObjectFactory;

struct UAutoRegister final
{
	UAutoRegister(const std::string_view className, const SObjectFactory::ObjectFactoryFunc& creator);
};
