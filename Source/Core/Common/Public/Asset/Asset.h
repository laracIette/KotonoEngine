#pragma once
#include "Path.h"
class AAsset
{
public:
	AAsset(UPath const& path);
	virtual ~AAsset() = default;

	UPath const& GetPath() const;
	
private:
	UPath path_;
};
