#pragma once
#include <kotono_common/Path.h>
#include <kotono_common/types.h>
#include <vector>
class UFont final
{
public:
	UFont(UPath const& path);

	UPath const& Path() const;
	f32 GetSize() const;
	f32 GetSpacing() const;

	void SetSize(f32 size);
	void SetSpacing(f32 spacing);

	std::vector<UPath> GetTextPaths(std::string_view text) const;

private:
	UPath const path_;
	// todo: unused yet, for sdf
	f32 size_;
	f32 spacing_;
};

