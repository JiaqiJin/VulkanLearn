#include "DeviceMemory.h"
#include "Device.h"
#include "PhysicalDevice.h"

#include <stdexcept>

namespace Rendering
{
	uint32_t findMemoryType(const PhysicalDevice& physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties)
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice.getHandle(), &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
		{
			if ((typeFilter & (1 << i)) == 0)
				continue;

			if ((memProperties.memoryTypes[i].propertyFlags & properties) != properties)
				continue;

			return i;
		}

		throw std::runtime_error("failed to find suitable memory type!");
	}

	DeviceMemory::DeviceMemory(const Device& device, const PhysicalDevice& physicalDevice, VkMemoryRequirements memoryRequirements, VkMemoryPropertyFlags memoryProperties)
		: m_device(device)
	{
		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memoryRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(physicalDevice, memoryRequirements.memoryTypeBits, memoryProperties);

		if (vkAllocateMemory(m_device.getHandle(), &allocInfo, nullptr, &m_handle.get()) != VK_SUCCESS)
			throw std::runtime_error("failed to allocate image memory!");
	}

	DeviceMemory::~DeviceMemory()
	{
		vkFreeMemory(m_device.getHandle(), m_handle, nullptr);
	}

	void DeviceMemory::copyFrom(const void* sourcePointer, std::size_t sourceSize)
	{
		void* data;
		vkMapMemory(m_device.getHandle(), m_handle, 0, sourceSize, 0, &data);
		memcpy(data, sourcePointer, sourceSize);
		vkUnmapMemory(m_device.getHandle(), m_handle);
	}

}


