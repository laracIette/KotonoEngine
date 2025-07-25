#pragma once
#include <unordered_map>
#include <filesystem>

class KtModel;

class KtModelManager final
{
public:
	void Cleanup(); // todo: make private and friend

	KtModel* Get(const std::filesystem::path& path);

private:
	std::unordered_map<std::filesystem::path, KtModel*> models_;

	KtModel* Create(const std::filesystem::path& path);
};

