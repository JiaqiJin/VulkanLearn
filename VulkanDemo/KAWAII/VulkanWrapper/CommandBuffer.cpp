#include "CommandBuffer.h"
#include "Device.h"

namespace RHI
{
	CommandBuffer::CommandBuffer(const std::shared_ptr<Device> pDevice, const std::shared_ptr<CommandPool> pCmdPool, CBLevel level)
		: m_pDevice(pDevice), m_pCommandPool(pCmdPool)
	{

	}

	CommandBuffer::~CommandBuffer()
	{

	}
}