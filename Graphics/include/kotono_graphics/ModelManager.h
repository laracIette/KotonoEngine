#pragma once
#include <unordered_map>
#include <kotono_common/Path.h>

class KtModel;

class KtModelManager final
{
	friend class SCore;

private:
	void Cleanup();

public:
	KtModel* Get(const UPath& path);

private:
	KtModel* Create(const UPath& path);

private:
	std::unordered_map<UPath, KtModel*> models_;
};

inline KtModelManager ModelManager;
