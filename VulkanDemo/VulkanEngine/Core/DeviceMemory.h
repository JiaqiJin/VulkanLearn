#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "UniqueHandle.h"

namespace Rendering
{
	class Device;
	class PhysicalDevice;

	class DeviceMemory
	{
	public:
		DeviceMemory(const Device& device, const PhysicalDevice& physicalDevice, VkMemoryRequirements memoryRequirements, VkMemoryPropertyFlags memoryProperties);
		~DeviceMemory();

		DeviceMemory(const DeviceMemory&) = default;
		DeviceMemory(DeviceMemory&&) = default;
		DeviceMemory& operator=(const DeviceMemory&) = default;
		DeviceMemory& operator=(DeviceMemory&&) = default;

		void copyFrom(const void* sourcePointer, std::size_t sourceSize);

		VkDeviceMemory getHandle() const { return m_handle; };

		template<typename T>
		void copyFrom(std::vector<T> const& source)
		{
			copyFrom(source.data(), sizeof(T) * source.size());
		}

	private:
		UniqueHandle<VkDeviceMemory> m_handle;

		const Device& m_device;
	};
}
