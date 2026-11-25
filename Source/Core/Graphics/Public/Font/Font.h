#pragma once

#include "Containers/List.h"
#include "Path/Path.h"
#include "types.h"

class UFont final
{
public:
	explicit UFont(UPath const& path);

	auto Path() const -> UPath const&;
	auto GetSize() const -> f32;
	auto GetSpacing() const -> f32;

	void SetSize(f32 size);
	void SetSpacing(f32 spacing);

	auto GetCharacterPath(char character) const -> UPath;
	auto GetTextPaths(std::string_view text) const -> UList<UPath>;

private:
	UPath path_;
	// todo: unused yet, for sdf
	f32 size_;
	f32 spacing_;

	UList<UPath> characterPaths_;
};

