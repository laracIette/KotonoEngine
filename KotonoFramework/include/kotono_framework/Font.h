#pragma once
#include <vector>
#include <filesystem>

class KtImageTexture;

class KtFont final
{
public:
	KtFont(const std::filesystem::path& path);

	const std::filesystem::path& GetPath() const;
	const float GetSize() const;
	const float GetSpacing() const;

	void SetSize(const float size);
	void SetSpacing(const float spacing);

	const std::vector<KtImageTexture*> GetTextTextures(const std::string_view text) const;

private:
	const std::filesystem::path path_;
	// todo: unused yet, for sdf
	float size_;
	float spacing_;
};

