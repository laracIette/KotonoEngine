#pragma once
#include <filesystem>
#include <vector>

class KtTexture;

class KtFont final
{
public:
	KtFont(const std::filesystem::path& path);

	const std::filesystem::path& Path() const;
	float GetSize() const;
	float GetSpacing() const;

	void SetSize(const float size);
	void SetSpacing(const float spacing);

	std::vector<std::filesystem::path> GetTextPaths(const std::string_view text) const;
	std::vector<KtTexture*> GetTextTextures(const std::string_view text) const;

private:
	const std::filesystem::path path_;
	// todo: unused yet, for sdf
	float size_;
	float spacing_;
};

