#pragma once

#include <vulkan/vulkan.h>
#include "UniqueHandle.h"
#include "../Objects/Object.h"

namespace Rendering
{
	class Device;
	class DeviceMemory;

	class Buffer : public Object
	{
	public:
		Buffer(const Application& app, VkDeviceSize size, VkBufferUsageFlags usage);
		~Buffer();

		Buffer(const Buffer&) = default;
		Buffer(Buffer&&) = default;
		Buffer& operator=(const Buffer&) = default;
		Buffer& operator=(Buffer&&) = default;

		VkMemoryRequirements getMemoryRequirements() const;
		void bindMemory(DeviceMemory const& memory) const;

		VkBuffer getHandle() const { return m_handle; }
		VkDeviceSize getSize() const { return m_size; }

		static void copy(const Buffer& source, const Buffer& destination);

	private:
		UniqueHandle<VkBuffer> m_handle;
		VkDeviceSize m_size;
		//const Device& m_device;
	};
}