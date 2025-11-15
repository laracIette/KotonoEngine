#pragma once
#include <functional>
#include <string_view>

template <class T>
class UPtr;
class KObject;

using ObjectFactoryFunc = std::function<UPtr<KObject>()>;
extern const std::unordered_map<std::string_view, ObjectFactoryFunc> ObjectFactories;