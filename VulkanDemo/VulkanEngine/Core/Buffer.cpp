#include "Buffer.h"
#include "Device.h"
#include "DeviceMemory.h"
#include "ScopedOneTimeCommandBuffer.h"

#include <stdexcept>

namespace Rendering
{
	Buffer::Buffer(const Application& app, VkDeviceSize size, VkBufferUsageFlags usage)
		: Object(app)
	{
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(getDevice().getHandle(), &bufferInfo, nullptr, &m_handle.get()) != VK_SUCCESS)
			throw std::runtime_error("failed to create buffer!");
	}

	Buffer::~Buffer()
	{
		vkDestroyBuffer(getDevice().getHandle(), m_handle, nullptr);
	}

	void Buffer::bindMemory(const DeviceMemory& memory) const
	{
		vkBindBufferMemory(getDevice().getHandle(), m_handle, memory.getHandle(), 0);
	}

	VkMemoryRequirements Buffer::getMemoryRequirements() const
	{
		VkMemoryRequirements memoryRequirements;
		vkGetBufferMemoryRequirements(getDevice().getHandle(), m_handle, &memoryRequirements);

		return memoryRequirements;
	}

	void Buffer::copy(const Buffer& source, const Buffer& destination)
	{
		if (source.getSize() != destination.getSize())
			throw std::runtime_error("Copy operation between buffers of different sizes");

		Application const& app = source.getApp();

		ScopedOneTimeCommandBuffer commandBuffer{ app };

		VkBufferCopy copyRegion{};
		copyRegion.size = source.getSize();
		vkCmdCopyBuffer(commandBuffer.getHandle(), source.getHandle(), destination.getHandle(), 1, &copyRegion);
	}

}