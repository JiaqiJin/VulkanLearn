#include "Buffer.h"
#include "Device.h"
#include "DeviceMemory.h"

#include <stdexcept>

namespace Rendering
{
	Buffer::Buffer(const Device& device, VkDeviceSize size, VkBufferUsageFlags usage)
		: m_device(device)
	{
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(m_device.getHandle(), &bufferInfo, nullptr, &m_handle.get()) != VK_SUCCESS)
			throw std::runtime_error("failed to create buffer!");
	}

	Buffer::~Buffer()
	{
		vkDestroyBuffer(m_device.getHandle(), m_handle, nullptr);
	}

	void Buffer::bindMemory(const DeviceMemory& memory) const
	{
		vkBindBufferMemory(m_device.getHandle(), m_handle, memory.getHandle(), 0);
	}

	VkMemoryRequirements Buffer::getMemoryRequirements() const
	{
		VkMemoryRequirements memoryRequirements;
		vkGetBufferMemoryRequirements(m_device.getHandle(), m_handle, &memoryRequirements);

		return memoryRequirements;
	}


}