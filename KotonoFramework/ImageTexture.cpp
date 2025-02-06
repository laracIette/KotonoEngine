#include "ImageTexture.h"
#include "Framework.h"

KtImageTexture::KtImageTexture(const std::filesystem::path& path) :
	Path(path)
{
	CreateTextureImage();
	CreateTextureImageView();
	CreateTextureSampler();
}

KtImageTexture::~KtImageTexture()
{
	vkDestroySampler(Framework.GetWindow().GetContext().GetDevice(), Sampler, nullptr); 
	vkDestroyImageView(Framework.GetWindow().GetContext().GetDevice(), ImageView, nullptr);
	vmaDestroyImage(Framework.GetWindow().GetContext().GetAllocator(), Image, Allocation);
}

void KtImageTexture::CreateTextureImage()
{
	int texWidth, texHeight, texChannels;
	stbi_uc* pixels = stbi_load(Path.string().c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	if (texWidth == 0 || texHeight == 0)
	{
		throw std::runtime_error("Texture has zero width or height!");
	}
	VkDeviceSize imageSize = texWidth * texHeight * 4;

	MipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;

	if (!pixels)
	{
		throw std::runtime_error("failed to load texture image!");
	}

	VkBuffer stagingBuffer;
	VmaAllocation stagingBufferAllocation;
	VmaAllocationInfo stagingBufferAllocInfo;
	Framework.GetWindow().GetContext().CreateBuffer(
		imageSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
		stagingBuffer,
		stagingBufferAllocation,
		stagingBufferAllocInfo
	);

	memcpy(stagingBufferAllocInfo.pMappedData, pixels, static_cast<size_t>(imageSize));

	stbi_image_free(pixels);

	Framework.GetWindow().GetContext().CreateImage(
		texWidth,
		texHeight,
		MipLevels,
		VK_SAMPLE_COUNT_1_BIT,
		VK_FORMAT_R8G8B8A8_SRGB,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		Image,
		Allocation
	);

	Framework.GetWindow().GetContext().TransitionImageLayout(
		Image,
		VK_FORMAT_R8G8B8A8_SRGB,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		MipLevels
	);
	Framework.GetWindow().GetContext().CopyBufferToImage(
		stagingBuffer,
		Image,
		static_cast<uint32_t>(texWidth),
		static_cast<uint32_t>(texHeight)
	);

	vmaDestroyBuffer(Framework.GetWindow().GetContext().GetAllocator(), stagingBuffer, stagingBufferAllocation);

	Framework.GetWindow().GetContext().GenerateMipmaps(Image, VK_FORMAT_R8G8B8A8_SRGB, texWidth, texHeight, MipLevels);

	Size = { static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight) };
}

void KtImageTexture::CreateTextureImageView()
{
	ImageView = Framework.GetWindow().GetContext().CreateImageView(Image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, MipLevels);
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
	vkGetPhysicalDeviceProperties(Framework.GetWindow().GetContext().GetPhysicalDevice(), &properties);
	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = static_cast<float>(MipLevels);
	samplerInfo.mipLodBias = 0.0f; // Optional

	if (vkCreateSampler(Framework.GetWindow().GetContext().GetDevice(), &samplerInfo, nullptr, &Sampler) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create texture sampler!");
	}
}
