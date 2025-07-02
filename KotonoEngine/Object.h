#pragma once
#include "base_class.h"
#include <string>
#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include "Guid.h"
#include <kotono_framework/Event.h>
#include <unordered_set>
#include <unordered_map>

class KObject
{
public:
	virtual void Construct();
	virtual void Init();
	virtual void Update();
	virtual void Cleanup();

	const UGuid& GetGuid() const;
	const std::filesystem::path& GetPath() const;
	const std::string& GetName() const;
	const bool GetIsDelete() const;
	const std::string GetTypeName() const;

	void SetPath(const std::filesystem::path& path);
	void SetName(const std::string& name);
	void SetIsDelete(const bool isDelete);

	// Serialize and write to the object's path
	void Serialize() const;
	// Read from the object's path and deserialize
	void Deserialize();

	// Serialize to json
	virtual void SerializeTo(nlohmann::json& json) const;
	// Deserialize from json
	virtual void DeserializeFrom(const nlohmann::json& json);

	template <class T, typename... Args>
		requires std::is_base_of_v<KObject, T>
	T* AddObject(Args... args)
	{
		T* component = new T(args...);
		AddObject(static_cast<KObject*>(component));
		return component;
	}

	template <typename... Args>
	void ListenEvent(KtEvent<Args...>& event, const KtDelegate<Args...>& delegate)
	{
		event.AddListener(delegate);
		listenedEvents_[&event] += 1;
	}

	template <typename... Args>
	void UnlistenEvent(KtEvent<Args...>& event, const KtDelegate<Args...>& delegate)
	{
		event.RemoveListener(delegate);
		listenedEvents_[&event] -= 1;
	}

	template <typename... Args>
	void UnlistenEvent(KtEvent<Args...>& event)
	{
		event.RemoveListener(this);
		listenedEvents_.erase(&event);
	}

	void Repeat(const KtDelegate<>& delegate, float frequency);

private:
	UGuid _guid;
	std::filesystem::path _path;
	std::string _name;
	bool _isDelete;

	std::unordered_set<KObject*> objects_;
	std::unordered_map<KtEventBase*, uint32_t> listenedEvents_;

	void AddObject(KObject* object);
	void UnlistenEvents();
};

