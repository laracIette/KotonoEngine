#pragma once
#include <kotono_common/Path.h>
#include <kotono_common/types.h>
#include <vector>
class UFont final
{
public:
	UFont(UPath const& path);

	auto Path() const -> UPath const&;
	auto GetSize() const -> f32;
	auto GetSpacing() const -> f32;

	void SetSize(f32 size);
	void SetSpacing(f32 spacing);

	auto GetCharacterPath(char character) const -> UPath;
	auto GetTextPaths(std::string_view text) const -> std::vector<UPath>;

private:
	UPath path_;
	// todo: unused yet, for sdf
	f32 size_;
	f32 spacing_;

	std::vector<UPath> characterPaths_;
};

