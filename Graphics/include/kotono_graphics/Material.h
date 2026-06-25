#pragma once
#include <kotono_common/Path.h>
#include <kotono_common/types.h>
class UMaterial final
{
public:
	UMaterial(const UPath& path);

	u32 GetIndex() const;

private:
	const UPath path_;

	u32 index_;
};