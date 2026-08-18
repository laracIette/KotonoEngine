#pragma once
#include <kotono_common/Asset.h>
#include <kotono_common/Path.h>
#include <kotono_common/types.h>
class AMaterial final : public AAsset
{
public:
	AMaterial(const UPath& path);

	u32 GetIndex() const;

private:
	u32 index_;
};