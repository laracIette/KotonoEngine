#pragma once
#include "generated/Object.generated.h"

#include "Guid.h"
#include "ObjectFactory.h"
#include "Ptr.h"
#include "VariableInfo.h"
#include <concepts>
#include <functional>
#include <kotono_common/Asset.h>
#include <kotono_common/Event.h>
#include <kotono_common/log.h>
#include <kotono_io/serialize_base.h>
#include <nlohmann/json_fwd.hpp>
#include <source_location>
#include <string>
#include <unordered_set>

using VoidCallback = std::function<void()>;

#define PROP_ACCESS_ const&
#define PROP_ACCESS_Value 
#define PROP_ACCESS_Reference &
#define PROP_ACCESS_ConstReference const&

#define FUNC_ACCESS_ const
#define FUNC_ACCESS_Value const
#define FUNC_ACCESS_Reference 
#define FUNC_ACCESS_ConstReference const

#define MACRO_CONCAT(a, b) a##b

#define GET_PROP_ACCESS(...) MACRO_CONCAT(PROP_ACCESS_, __VA_ARGS__)
#define GET_FUNC_ACCESS(...) MACRO_CONCAT(FUNC_ACCESS_, __VA_ARGS__)

#define ReadonlyProperty(Type, Name, PropertyName, ...) private:						\
	Type Name;																			\
public:																					\
	Type GET_PROP_ACCESS(__VA_ARGS__) Get##PropertyName() GET_FUNC_ACCESS(__VA_ARGS__) { return Name; } \
private:

#define WritableProperty(Type, Name, PropertyName, ...) private:						\
	Type Name;																			\
public:																					\
	Type GET_PROP_ACCESS(__VA_ARGS__) Get##PropertyName() GET_FUNC_ACCESS(__VA_ARGS__) { return Name; } \
	void Set##PropertyName(Type const& value) { Name = value; }				\
private:

class KObject;
using ObjectPtr = UPtr<KObject>;

class UPath;

class KObject
{
	GENERATED_KOBJECT()

	friend class SObjectFactory;

public:
	KObject();
	virtual ~KObject();

	virtual void PostConstruct();

public:
	std::string TypeName() const;
	UPath InstancePath() const;

	/// Read json from disk
	nlohmann::json ReadJson() const;
	/// Write the object to json
	nlohmann::json WriteJson() const;

	/// Cleanup and delete the object immediately
	void Delete();

	/// Serialize and write to the object's path
	virtual void Serialize() const;
	/// Read from the object's path and deserialize
	virtual void Deserialize();	

	virtual std::string ToString() const;

	operator std::string() const;

public:
	static ObjectPtr Deserialize(const nlohmann::json& json);

protected:
	UPtrOwner* const ptrOwner_;

private:
	SERIALIZE std::string type_;
	SERIALIZE ReadonlyProperty(UGuid, guid_, Guid);
	SERIALIZE WritableProperty(std::string, name_, Name);

#ifndef NDEBUG
public:
	static void CheckDebugRegistry();

public:
	std::string sourceFile;
	std::string sourceFunc;
	u32 sourceLine;

private:
	static std::unordered_set<ObjectPtr> debugRegistry_;
#endif
};



template <typename MemFn, typename... Args>
struct _MemberAction
{
	MemFn fn;
	std::tuple<Args...> args;

	template <typename T>
	void Apply(T* ptr) const
	{
		std::apply([ptr, this](auto&&... unpacked_args) {
			(ptr->*fn)(std::forward<decltype(unpacked_args)>(unpacked_args)...);
		}, args);
	}
};

template <std::derived_from<KObject> T, typename MemFn, typename... Args>
	requires std::is_member_function_pointer_v<MemFn>
		&& (std::is_invocable_v<MemFn, T*, Args...> || std::is_invocable_v<MemFn, T*>)
UPtr<T> const& operator|(UPtr<T> const& object, _MemberAction<MemFn, Args...> const& action)
{
	action.Apply(object.Get());
	return object;
}

template <typename MemFn, typename... Args>
auto _Make(MemFn fn, Args&&... args)
{
	return _MemberAction<MemFn, std::decay_t<Args>...>{
		fn, std::make_tuple(std::forward<Args>(args)...)
	};
}

#define Apply _Make





template <std::derived_from<KObject> T>
struct UCreate final
{
public:
#ifndef NDEBUG
	constexpr UCreate(std::string_view name = "", std::source_location const& loc = std::source_location::current())
		: name_{ name }
		, loc_{ loc }
	{}
#else
	constexpr UCreate(std::string_view name = "")
		: name_{ name }
	{}
#endif

	template <typename ...Args>
		requires std::constructible_from<T, Args...>
	UPtr<T> operator()(Args&&... args) const
	{
		T* object{ new T{ std::forward<Args>(args)... } };

		if (!name_.empty())
		{
			object->SetName(name_);
		}

		object->PostConstruct();

#	ifndef NDEBUG
		object->sourceFile = loc_.file_name();
		object->sourceFunc = loc_.function_name();
		object->sourceLine = loc_.line();
#	endif

		return object->Ptr();
	}

private:
	const std::string name_;

#ifndef NDEBUG
	const std::source_location loc_;
#endif
};

template <std::derived_from<KObject> T>
struct USerialize<UPtr<T>> final
{
	void operator()(nlohmann::json& json, const UPtr<T>& v) const
	{
		if (v)
		{
			USerialize<UGuid>{}(json, v->GetGuid());
			v->Serialize();
		}
	}
};

template <std::derived_from<KObject> T>
struct UDeserialize<UPtr<T>> final
{
	void operator()(const nlohmann::json& json, UPtr<T>& v) const
	{
		v = TryCast<T>(KObject::Deserialize(json));
	}
};

template <std::derived_from<KObject> T>
struct UAutoDelete final
{
public:
	explicit UAutoDelete(std::span<UPtr<T> const> values) 
		: values_(values.begin(), values.end())
	{}

	~UAutoDelete()
	{
		for (auto const& object : values_)
		{
			if (object)
			{
				object->Delete();
			}
		}
	}

private:
	std::vector<UPtr<T>> values_;
};
