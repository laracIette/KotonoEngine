#include "Texture.h"
#include "PipelineResourceManager.h"
#include <kotono_common/log.h>
#include <kotono_platform/Context.h>
#include <stbimage/stb_image.h>

UTexture::UTexture(const UPath& path) 
	: path_{ path }
	, index_{}
{
	CreateImage();
	CreateImageView();

	index_ = PipelineResourceManager.RegisterTexture(imageView_);
}

UTexture::~UTexture()
{
	PipelineResourceManager.UnregisterTexture(index_);

	vkDestroyImageView(Context.GetDevice(), imageView_, nullptr);
	vmaDestroyImage(Context.GetAllocator(), image_, allocation_);
	KT_LOG(ELogImportanceLevel::Low, "Graphics", "cleaned up {0}", Path().ToString());
}

const UPath& UTexture::Path() const
{
	return path_;
}

const glm::uvec2& UTexture::GetSize() const
{
	return size_;
}

u32 UTexture::GetIndex() const
{
	return index_;
}

void UTexture::CreateImage()
{
	int texWidth, texHeight, texChannels;
	stbi_uc* pixels{ stbi_load(path_.ToPath().string().c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha) };
	if (texWidth == 0 || texHeight == 0)
	{
		throw std::runtime_error("Texture has zero width or height!");
	}
	const VkDeviceSize imageSize{ static_cast<VkDeviceSize>(texWidth) * texHeight * 4 };

	mipLevels_ = static_cast<u32>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;

	if (!pixels)
	{
		throw std::runtime_error("failed to load texture image!");
	}

	Context.CreateBuffer(
		imageSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
		stagingBuffer_
	);

	std::memcpy(stagingBuffer_.allocationInfo.pMappedData, pixels, static_cast<size>(imageSize));

	stbi_image_free(pixels);

	Context.CreateImage(
		texWidth,
		texHeight,
		mipLevels_,
		VK_SAMPLE_COUNT_1_BIT,
		VK_FORMAT_R8G8B8A8_SRGB,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		image_,
		allocation_
	);

	Context.TransitionImageLayout(
		image_,
		VK_FORMAT_R8G8B8A8_SRGB,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		mipLevels_
	);

	Context.CopyBufferToImage(
		stagingBuffer_.buffer,
		image_,
		static_cast<u32>(texWidth),
		static_cast<u32>(texHeight)
	);

	Context.GetEventExecuteSingleTimeCommands().AddListener(this, &UTexture::DestroyStagingBuffer);

	Context.GenerateMipmaps(image_, VK_FORMAT_R8G8B8A8_SRGB, texWidth, texHeight, mipLevels_);

	size_ = glm::uvec2(static_cast<u32>(texWidth), static_cast<u32>(texHeight));
}

void UTexture::CreateImageView()
{
	imageView_ = Context.CreateImageView(image_, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, mipLevels_);
}

void UTexture::DestroyStagingBuffer() const
{
	vmaDestroyBuffer(Context.GetAllocator(), stagingBuffer_.buffer, stagingBuffer_.allocation);
}
