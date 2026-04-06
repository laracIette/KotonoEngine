#pragma once
#include "Path.h"

template <typename T>
class UAssetManager;

template <typename T>
class UAsset final
{
public:
	using PointerType = T;

private:
	friend class UAssetManager<PointerType>;

	UAsset(const UPath& path, PointerType* pointer) : path_(path), pointer_(pointer) {}

public:
	UAsset() : path_(), pointer_(nullptr) {}

	UAsset(const UAsset& asset) : path_(asset.path_), pointer_(asset.pointer_) {}

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
