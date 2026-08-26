#pragma once
#include "vk_utils.h"
#include <span>
#include <stdexcept>
#include <type_traits>
#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>
struct UAllocatedBuffer final
{
	VkBuffer buffer;
	VmaAllocation allocation;
	VmaAllocationInfo allocationInfo;
	VkDeviceAddress bda;

	void Create(
		  VkDevice device
		, VmaAllocator allocator
		, VkDeviceSize size
		, VkBufferUsageFlags usage
		, VmaAllocationCreateFlags allocFlags
		, VmaMemoryUsage memUsage = VMA_MEMORY_USAGE_AUTO
	)
	{
		VkBufferCreateInfo const bufInfo{
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.size = size,
			.usage = usage,
		};

		VmaAllocationCreateInfo const allocInfo{
			.flags = allocFlags,
			.usage = memUsage,
		};

		VK_CHECK_THROW(
			vmaCreateBuffer(
				  allocator
				, &bufInfo
				, &allocInfo
				, &buffer
				, &allocation
				, &allocationInfo
			),
			"failed to create buffer with VMA!"
		);

		if ((allocFlags & VMA_ALLOCATION_CREATE_MAPPED_BIT) && !allocationInfo.pMappedData)
		{
			throw std::runtime_error("Staging buffer was not mapped as expected!");
		}

		if (usage & VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT)
		{
			VkBufferDeviceAddressInfo const addrInfo{
				.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
				.buffer = buffer,
			};
			bda = vkGetBufferDeviceAddress(device, &addrInfo);
		}
	}

	void Cleanup(VmaAllocator allocator) const
	{
		vmaDestroyBuffer(allocator, buffer, allocation);
	}

	template <typename T>
		requires std::is_trivially_copyable_v<T>
	void UpdateMappedData(std::span<T const> data) const
	{
		std::memcpy(allocationInfo.pMappedData
		   , data.data()
		   , data.size_bytes()
		);
	}
};

