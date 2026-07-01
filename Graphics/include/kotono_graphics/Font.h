#pragma once
#include <filesystem>
#include <vector>
#include <kotono_common/Path.h>

template <typename T>
class UAsset;
class UTexture;

class KtFont final
{
public:
	KtFont(const UPath& path);

	const UPath& Path() const;
	float GetSize() const;
	float GetSpacing() const;

	void SetSize(const float size);
	void SetSpacing(const float spacing);

	std::vector<UPath> GetTextPaths(const std::string_view text) const;
	std::vector<UAsset<UTexture>> GetTextTextures(const std::string_view text) const;

private:
	const UPath path_;
	// todo: unused yet, for sdf
	float size_;
	float spacing_;
};

