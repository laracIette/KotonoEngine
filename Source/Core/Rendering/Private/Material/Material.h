#pragma once
#include <Asset/Asset.h>

class AMaterial final : public AAsset
{
public:
	struct Data
	{
		UPath albedo;
		UPath normal;
		UPath orm;
		UPath emissive;
		u32 materialType;
		UPath sampler;
	};

public:
	AMaterial(UPath const& path);

	Data const& GetData() const { return data_; }

private:
	Data data_;
};