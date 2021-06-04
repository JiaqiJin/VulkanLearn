#include "StagingBuffer.h"
#include "GlobalDeviceObjects.h"

namespace RHI
{
	StagingBuffer::StagingBuffer(const std::shared_ptr<Device>& pDevice, uint32_t numBytes)
		: Buffer(pDevice, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
	{
		VkBufferCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		info.size = numBytes;

		m_info = info;
	}

	StagingBuffer::StagingBuffer(const std::shared_ptr<Device>& pDevice, uint32_t numBytes, bool ReadableStagingBuffer)
		: Buffer(pDevice, VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_HOST_READ_BIT)
	{

	}

}