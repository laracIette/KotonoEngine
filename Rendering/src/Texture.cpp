#include "Texture.h"

#include <kotono_common/log.h>
#include <kotono_platform/Context.h>
#include <stbimage/stb_image.h>

ATexture::ATexture(UPath const& path) 
	: AAsset(path)
	, index_{}
{
	CreateImage();
}

ATexture::~ATexture()
{
	vkDestroyImageView(Context.GetDevice(), allocatedImage_.imageView, nullptr);
	vmaDestroyImage(Context.GetAllocator(), allocatedImage_.image, allocatedImage_.allocation);
	KT_LOG(ELogImportanceLevel::Low, "Graphics", "cleaned up {0}", GetPath().ToString());
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

void ATexture::CreateImage()
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

	Context.CreateBuffer(stagingBuffer_
		, imageSize
		, VK_BUFFER_USAGE_TRANSFER_SRC_BIT
		, VMA_ALLOCATION_CREATE_MAPPED_BIT
		| VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
	);

	std::memcpy(stagingBuffer_.allocationInfo.pMappedData, pixels, static_cast<size>(imageSize));

	stbi_image_free(pixels);

	Context.CreateImageAndImageView(allocatedImage_, UAllocatedImageCreateInfo::CreateSampled2D(
		static_cast<u32>(texWidth),
		static_cast<u32>(texHeight),
		mipLevels,
		VK_FORMAT_R8G8B8A8_SRGB,
		VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
		VK_IMAGE_ASPECT_COLOR_BIT
	));

	Context.TransitionImageLayout(
		allocatedImage_.image,
		VK_FORMAT_R8G8B8A8_SRGB,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		mipLevels
	);

	Context.CopyBufferToImage(
		stagingBuffer_.buffer,
		allocatedImage_.image,
		static_cast<u32>(texWidth),
		static_cast<u32>(texHeight)
	);

	Context.GetEventExecuteSingleTimeCommands().AddListener(this, &ATexture::DestroyStagingBuffer);

	Context.GenerateMipmaps(allocatedImage_.image, VK_FORMAT_R8G8B8A8_SRGB, texWidth, texHeight, mipLevels);

	size_ = { static_cast<u32>(texWidth), static_cast<u32>(texHeight) };
}

void ATexture::DestroyStagingBuffer() const
{
	vmaDestroyBuffer(Context.GetAllocator(), stagingBuffer_.buffer, stagingBuffer_.allocation);
}
