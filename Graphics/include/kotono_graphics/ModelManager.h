#pragma once
#include <unordered_map>
#include <filesystem>

class KtModel;

class KtModelManager final
{
	friend class SEngine;

private:
	void Cleanup();

public:
	KtModel* Get(const std::filesystem::path& path);

private:
	std::unordered_map<std::filesystem::path, KtModel*> models_;

	KtModel* Create(const std::filesystem::path& path);
};

inline KtModelManager ModelManager;
