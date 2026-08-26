#pragma once
#include <kotono_common/Asset.h>
#include <kotono_common/Path.h>
#include <kotono_common/types.h>
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

	Data GetData() const;
};