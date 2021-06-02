#include "Buffer.h"
#include "Device.h"

#include "../Common/Macro.h"
#include "../Common/Logger.h"

namespace RHI
{
	Buffer::Buffer(const std::shared_ptr<Device>& pDevice, const VkBufferCreateInfo& info, uint32_t memoryPropertyFlag)
		: BufferBase(pDevice, info)
	{

	}

	Buffer::~Buffer()
	{
		if (m_buffer)
			vkDestroyBuffer(m_pDevice->GetDeviceHandle(), m_buffer, nullptr);
	}
}