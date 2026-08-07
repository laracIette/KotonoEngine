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

#define ReadonlyProperty(Type, Name, PropertyName) private:			\
	Type Name;														\
public:																\
	const Type& Get##PropertyName() const noexcept { return Name; } \
private:

#define WritableProperty(Type, Name, PropertyName) private:					\
	Type Name;																\
public:																		\
	const Type& Get##PropertyName() const noexcept { return Name; }			\
	void Set##PropertyName(const Type& value) noexcept { Name = value; }	\
private:

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
	const std::type_info& Type() const;
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

public:
	static UPtr<KObject> Deserialize(const nlohmann::json& json);

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
	static std::unordered_set<UPtr<KObject>> debugRegistry_;
#endif
};

template <std::derived_from<KObject> T>
struct UCreate final
{
public:
#ifndef NDEBUG
	constexpr UCreate(const std::string& name = "", const std::source_location& loc = std::source_location::current())
		: name_{ name }
		, loc_{ loc }
	{}
#else
	constexpr UCreate(const std::string& name = "")
		: name_{ name }
	{}
#endif

	template <typename ...Args>
		requires std::constructible_from<T, Args...>
	UPtr<T> operator()(Args&&... args) const
	{
		T* object{ new T{ std::forward<Args>(args)... } };
		object->PostConstruct();

		if (!name_.empty())
		{
			object->SetName(name_);
		}

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

struct UAutoDelete final
{
public:
	template <std::derived_from<KObject> T>
	UAutoDelete(const UPool<UPtr<T>> pool) : pool_(pool) {}

	~UAutoDelete()
	{
		for (auto& object : pool_)
		{
			if (object)
			{
				object->Delete();
			}
		}
	}

private:
	UPool<UPtr<KObject>> pool_;
};
