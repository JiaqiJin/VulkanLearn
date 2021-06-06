#include "VertexBuffer.h"

#include "../Common/Macro.h"
#include "../Common/Logger.h"

namespace RHI
{
	VertexBuffer::VertexBuffer(const std::shared_ptr<Device> pDevice, uint32_t numBytes)
		: Buffer(pDevice, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
	{
		VkBufferCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		info.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		info.size = numBytes;

		if(!Buffer::Init(info, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT))
			K_ERROR("Error Initialize Vertex Buffer");
	}
}