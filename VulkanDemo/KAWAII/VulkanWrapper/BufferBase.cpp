#include "BufferBase.h"
#include "Device.h"

namespace RHI
{
	BufferBase::BufferBase(const std::shared_ptr<Device>& pDevice, const VkBufferCreateInfo& info)
		: VKGPUSyncRes(pDevice), m_info(info)
	{

	}
}