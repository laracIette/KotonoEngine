#include "Texture.h"

#include <kotono_platform/AllocatedBuffer.h>
#include <kotono_platform/Device.h>
#include <stbimage/stb_image.h>

ATexture::ATexture(UPath const& path) 
	: AAsset(path)
	, index_{}
{
}

void ATexture::Init(UDevice& device)
{
	CreateImage(device);
}

void ATexture::Cleanup(UDevice& device) const
{
	device.CleanupAllocatedImage(allocatedImage_);
}

glm::uvec2 const& ATexture::GetSize() const
{
	return size_;
}

u32 ATexture::GetIndex() const
{
	return index_;
}

void ATexture::SetIndex(u32 index)
{
	index_ = index;
}

VkImageView ATexture::GetImageView() const
{
	return allocatedImage_.imageView;
}

void ATexture::CreateImage(UDevice& device)
{
	i32 texWidth, texHeight, texChannels;
	stbi_uc* pixels{ stbi_load(GetPath().ToPath().string().c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha)};
	if (texWidth == 0 || texHeight == 0)
	{
		throw std::runtime_error("Texture has zero width or height!");
	}
	if (!pixels)
	{
		throw std::runtime_error("failed to load texture image!");
	}

	VkDeviceSize const imageSize{ static_cast<VkDeviceSize>(texWidth) * texHeight * 4 };
	u32 const mipLevels{ static_cast<u32>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1 };

	auto const stagingBuffer{ device.CreateAllocatedBuffer(
		  imageSize
		, VK_BUFFER_USAGE_TRANSFER_SRC_BIT
		, VMA_ALLOCATION_CREATE_MAPPED_BIT
		| VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
	) };

	std::memcpy(stagingBuffer.allocationInfo.pMappedData, pixels, static_cast<size>(imageSize));

	stbi_image_free(pixels);

	allocatedImage_ = device.CreateAllocatedImage(UAllocatedImageCreateInfo::CreateSampled2D(
		static_cast<u32>(texWidth),
		static_cast<u32>(texHeight),
		mipLevels,
		VK_FORMAT_R8G8B8A8_SRGB,
		VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
		VK_IMAGE_ASPECT_COLOR_BIT
	));

	device.TransitionImageLayout(
		allocatedImage_,
		VK_FORMAT_R8G8B8A8_SRGB,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		mipLevels
	);

	device.CopyBufferToImage(
		stagingBuffer,
		allocatedImage_,
		static_cast<u32>(texWidth),
		static_cast<u32>(texHeight)
	);

	device.StageBufferForDeletion(stagingBuffer);

	device.GenerateMipmaps(allocatedImage_, VK_FORMAT_R8G8B8A8_SRGB, texWidth, texHeight, mipLevels);

	size_ = { static_cast<u32>(texWidth), static_cast<u32>(texHeight) };
}
