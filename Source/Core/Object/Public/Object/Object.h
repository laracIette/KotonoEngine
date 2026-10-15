#pragma once
#include "ObjectReflectionBase.h"

#include "Ptr/ObjectPtr.h"

#include "check.h"
#include "properties.h"
#include "Guid/Guid.h"
#include <concepts>
#include <functional>
#include <format>
#include <Event/Event.h>
#include <Path/Path.h>
#include <serialize_base/serialize_base.h>
#include <optional>
#include <source_location>
#include <string>

#ifndef NDEBUG
#include <unordered_set>
#endif

#include "Object.generated.h"

using VoidCallback = std::function<void()>;

class KObject;
using ObjectPtr = UObjectPtr<KObject>;

ABSTRACT class KObject : public KObjectReflectionBase
{
	GENERATED()

	friend class SObjectFactory;
	
	template <std::derived_from<KObject> T>
	friend struct UCreate;

public:
	KObject();
	~KObject() override;
	
	auto GetTypeName() const -> std::string;
	auto GetMemberVariablePointer(size offset) const -> void*;

	/// Serialize and write to the object's path
	virtual void Serialize() const;
	/// Read from the object's path and deserialize
	virtual void Deserialize();	

	virtual auto ToString() const -> std::string;

	static auto Deserialize(nlohmann::json const& json) -> ObjectPtr;
	
private:
	auto GetAssetPath() const -> UPath;
	auto GetTempPath() const -> UPath;

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
		std::apply([ptr, this](auto&&... unpackedArgs) {
			(ptr->*fn)(std::forward<decltype(unpackedArgs)>(unpackedArgs)...);
		}, args);
	}
};

template <std::derived_from<KObject> T, typename MemFn, typename... Args>
	requires std::is_member_function_pointer_v<MemFn>
		&& (std::is_invocable_v<MemFn, T*, Args...> || std::is_invocable_v<MemFn, T*>)
UObjectPtr<T> const& operator|(UObjectPtr<T> const& object, _MemberAction<MemFn, Args...> const& action)
{
	Check(Abort, object, "object is null!");
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
	constexpr UCreate(std::optional<std::string> const& name = std::nullopt, std::source_location const& loc = std::source_location::current())
		: name_{ name }
		, loc_{ loc }
	{}
#else
	constexpr UCreate(std::optional<std::string> const& name = std::nullopt)
		: name_{ name }
	{}
#endif

	template <typename ...Args>
		requires std::constructible_from<T, Args...>
	UObjectPtr<T> operator()(Args&&... args) const
	{
		T* object{ new T{ std::forward<Args>(args)... } };
		object->type_ = object->GetTypeName();
			
		if (name_.has_value())
		{
			object->SetName(name_.value());
		}
		else
		{
			object->SetName(std::format("{0}_{1}", object->type_, object->GetGuid().ToString()));
		}

#	ifndef NDEBUG
		object->sourceFile = loc_.file_name();
		object->sourceFunc = loc_.function_name();
		object->sourceLine = loc_.line();
#	endif

		return object->Ptr();
	}

private:
	std::optional<std::string> name_;

#ifndef NDEBUG
	std::source_location loc_;
#endif
};

template <std::derived_from<KObject> T>
struct USerialize<UObjectPtr<T>> final
{
	void operator()(nlohmann::json& json, const UObjectPtr<T>& v) const
	{
		if (v)
		{
			USerialize<UGuid>{}(json, v->GetGuid());
			v->Serialize();
		}
	}
};

template <std::derived_from<KObject> T>
struct UDeserialize<UObjectPtr<T>> final
{
	void operator()(const nlohmann::json& json, UObjectPtr<T>& v) const
	{
		v = TryCast<T>(KObject::Deserialize(json));
	}
};

template <std::derived_from<KObject> T, typename CharT>
struct std::formatter<UObjectPtr<T>, CharT> : std::formatter<std::string, CharT>
{
	auto format(UObjectPtr<T> const& ptr, auto& ctx) const 
	{
		return std::format_to(ctx.out(), "{0}", ptr ? ptr->ToString() : "nullptr");
	}
};

template <std::derived_from<KObject> T>
struct UAutoDelete final
{
public:
	explicit UAutoDelete(std::span<UObjectPtr<T> const> values) 
		: values_(values.begin(), values.end())
	{}

	~UAutoDelete()
	{
		for (auto const& object : values_)
		{
			Delete(object);
		}
	}

private:
	std::vector<UObjectPtr<T>> values_;
};
