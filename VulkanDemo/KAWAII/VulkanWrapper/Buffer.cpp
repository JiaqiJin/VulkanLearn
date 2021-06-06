#include "Buffer.h"
#include "Device.h"
#include "GlobalDeviceObjects.h"

#include "../Common/Macro.h"
#include "../Common/Logger.h"

namespace RHI
{
	Buffer::Buffer(const std::shared_ptr<Device>& pDevice, uint32_t memoryPropertyFlag)
		: BufferBase(pDevice)
	{
		if (!Init(m_info, memoryPropertyFlag))
			K_ERROR("Error Initialize Buffer");
	}

	Buffer::Buffer(const std::shared_ptr<Device>& pDevice, const VkBufferCreateInfo& info, uint32_t memoryPropertyFlag)
		: BufferBase(pDevice)
	{
		if (!Init(m_info, memoryPropertyFlag))
			K_ERROR("Error Initialize Buffer");
	}

	bool Buffer::Init(const VkBufferCreateInfo& info, uint32_t memoryPropertyFlag)
	{
		if (!BufferBase::Init(info))
			return false;

		CHECK_VK_ERROR(vkCreateBuffer(m_pDevice->GetDeviceHandle(), &m_info, nullptr, &m_buffer));
		m_pMemKey = DeviceMemMgr()->AllocateBufferMemChunk(this, memoryPropertyFlag);

		m_isHostVisible = memoryPropertyFlag & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
	}

	std::shared_ptr<Buffer> Buffer::Create(const std::shared_ptr<Device>& pDevice, const VkBufferCreateInfo& info, uint32_t memoryPropertyFlag)
	{
		std::shared_ptr<Buffer> pBuffer = std::make_shared<Buffer>(pDevice, info, memoryPropertyFlag);
		if (pBuffer.get() && pBuffer->Init(info, memoryPropertyFlag))
			return pBuffer;
		return nullptr;
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