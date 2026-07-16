#pragma once
#include "Path.h"
template <typename T>
class SAssetManager;
template <typename T>
class UAsset final
{
public:
	using PointerType = T;

private:
	friend class SAssetManager<PointerType>;

	friend std::hash<UAsset>;

	UAsset(const UPath& path, PointerType* pointer) : path_{ path }, pointer_{ pointer } {}

public:
	UAsset() : UAsset{ {}, nullptr } {}

	UAsset(const UAsset& asset) : UAsset{ asset.path_, asset.pointer_ } {}

	UAsset& operator=(const UAsset& other)
	{
		if (this != &other)
		{
			path_ = other.path_;
			pointer_ = other.pointer_;
		}
		return *this;
	}

	constexpr const UPath& Path() const noexcept
	{
		return path_;
	}

	constexpr PointerType* Get() const noexcept
	{
		return pointer_;
	}

	constexpr PointerType* operator->() const noexcept
	{
		return Get();
	}

	constexpr PointerType& operator*() const noexcept
	{
		return *Get();
	}

	constexpr operator bool() const noexcept
	{
		return pointer_;
	}

	constexpr bool operator==(const UAsset& other) const noexcept
	{
		return pointer_ == other.pointer_;
	}
	
private:
	UPath path_;
	PointerType* pointer_;
};

template <typename T>
struct std::hash<UAsset<T>>
{
	::size operator()(const UAsset<T>& asset) const noexcept
	{
		return std::hash<void*>{}(asset.pointer_);
	}
};
