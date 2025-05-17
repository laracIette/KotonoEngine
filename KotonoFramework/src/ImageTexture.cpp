#include "ImageTexture.h"
#include <stbimage/stb_image.h>
#include "Framework.h"
#include "Context.h"
#include "log.h"

KtImageTexture::KtImageTexture(const std::filesystem::path& path) :
	path_(path)
{
}

const std::filesystem::path& KtImageTexture::GetPath() const
{
	return path_;
}

const glm::uvec2& KtImageTexture::GetSize() const
{
	return size_;
}

const VkDescriptorImageInfo& KtImageTexture::GetDescriptorImageInfo() const
{
	return imageInfo_;
}

void KtImageTexture::Init()
{
	CreateTextureImage();
	CreateTextureImageView();
	CreateTextureSampler();

	imageInfo_.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageInfo_.imageView = imageView_;
	imageInfo_.sampler = sampler_;
}

void KtImageTexture::Cleanup() const
{
	KT_DEBUG_LOG("cleaning up image texture");
	vkDestroySampler(Framework.GetContext().GetDevice(), sampler_, nullptr);
	vkDestroyImageView(Framework.GetContext().GetDevice(), imageView_, nullptr);
	vmaDestroyImage(Framework.GetContext().GetAllocator(), image_, allocation_);
	KT_DEBUG_LOG("cleaned up image texture");
}

void KtImageTexture::CreateTextureImage()
{
	int texWidth, texHeight, texChannels;
	stbi_uc* pixels = stbi_load(path_.string().c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	if (texWidth == 0 || texHeight == 0)
	{
		throw std::runtime_error("Texture has zero width or height!");
	}
	VkDeviceSize imageSize = static_cast<VkDeviceSize>(texWidth) * texHeight * 4;

	mipLevels_ = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;

	if (!pixels)
	{
		throw std::runtime_error("failed to load texture image!");
	}

	KtAllocatedBuffer stagingBuffer;
	Framework.GetContext().CreateBuffer(
		imageSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
		stagingBuffer
	);

	memcpy(stagingBuffer.AllocationInfo.pMappedData, pixels, static_cast<size_t>(imageSize));

	stbi_image_free(pixels);

	Framework.GetContext().CreateImage(
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

	Framework.GetContext().TransitionImageLayout(
		image_,
		VK_FORMAT_R8G8B8A8_SRGB,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		mipLevels_
	);
	Framework.GetContext().CopyBufferToImage(
		stagingBuffer.Buffer,
		image_,
		static_cast<uint32_t>(texWidth),
		static_cast<uint32_t>(texHeight)
	);

	vmaDestroyBuffer(Framework.GetContext().GetAllocator(), stagingBuffer.Buffer, stagingBuffer.Allocation);

	Framework.GetContext().GenerateMipmaps(image_, VK_FORMAT_R8G8B8A8_SRGB, texWidth, texHeight, mipLevels_);

	size_ = glm::uvec2(static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
}

void KtImageTexture::CreateTextureImageView()
{
	imageView_ = Framework.GetContext().CreateImageView(image_, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, mipLevels_);
}

void KtImageTexture::CreateTextureSampler()
{
	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

	VkPhysicalDeviceProperties properties{};
	vkGetPhysicalDeviceProperties(Framework.GetContext().GetPhysicalDevice(), &properties);
	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = static_cast<float>(mipLevels_);
	samplerInfo.mipLodBias = 0.0f; // Optional

	if (vkCreateSampler(Framework.GetContext().GetDevice(), &samplerInfo, nullptr, &sampler_) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create texture sampler!");
	}
}
