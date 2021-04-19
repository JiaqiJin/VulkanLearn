#pragma once

#include <vulkan/vulkan.h>
#include "UniqueHandle.h"

namespace Rendering
{
	class Device;
	class DeviceMemory;

	class Buffer
	{
	public:
		Buffer(const Device& device, VkDeviceSize size, VkBufferUsageFlags usage);
		~Buffer();

		Buffer(const Buffer&) = default;
		Buffer(Buffer&&) = default;
		Buffer& operator=(const Buffer&) = default;
		Buffer& operator=(Buffer&&) = default;

		VkMemoryRequirements getMemoryRequirements() const;
		void bindMemory(const DeviceMemory& memory) const;

		VkBuffer getHandle() const { return m_handle; }

	private:
		UniqueHandle<VkBuffer> m_handle;
		const Device& m_device;
	};
}