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

	VkMemoryRequirements Buffer::GetMemoryReqirments() const
	{
		VkMemoryRequirements reqs;
		vkGetBufferMemoryRequirements(m_pDevice->GetDeviceHandle(), m_buffer, &reqs);
		return reqs;
	}

	void Buffer::BindMemory(VkDeviceMemory memory, uint32_t offset) const
	{
		CHECK_VK_ERROR(vkBindBufferMemory(m_pDevice->GetDeviceHandle(), m_buffer, memory, offset));
	}

	Buffer::~Buffer()
	{
		if (m_buffer)
			vkDestroyBuffer(m_pDevice->GetDeviceHandle(), m_buffer, nullptr);
	}
}