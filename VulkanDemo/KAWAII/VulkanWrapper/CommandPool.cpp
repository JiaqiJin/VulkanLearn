#include "CommandPool.h"
#include "Fence.h"
#include "Device.h"

#include "../Common/Macro.h"
#include "../Common/Logger.h"

namespace RHI
{
	CommandPool::CommandPool(const std::shared_ptr<Device> pDevice, 
		PhysicalDevice::QueueFamily queueFamily,
		CBPersistancy persistancy)
		: m_pDevice(pDevice)
	{
		if(!Init(queueFamily, persistancy))
			K_ERROR("Error Initialize Command Pool");
	}

	bool CommandPool::Init(PhysicalDevice::QueueFamily queueFamily, CBPersistancy persistancy)
	{
		m_info = {};
		m_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		m_info.queueFamilyIndex = m_pDevice->GetPhysicalDevice()->GetQueueFamilyIndex(queueFamily);
		if (persistancy == CBPersistancy::TRANSIENT)
		{
			// Hint that command buffers are rerecorded with new commands very often (may change memory allocation behavior)
			m_info.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
		}
		CHECK_VK_ERROR(vkCreateCommandPool(m_pDevice->GetDeviceHandle(), &m_info, nullptr, &m_commandPool));

		return true;
	}

	std::shared_ptr<CommandBuffer> CommandPool::AllocateCommandBuffer(CommandBuffer::CBLevel level)
	{
		std::shared_ptr<CommandBuffer> commandBuffer = std::make_shared<CommandBuffer>(m_pDevice, shared_from_this(), level);

		return commandBuffer;
	}

	std::vector<std::shared_ptr<CommandBuffer>> CommandPool::AllocateCommandBuffers(CommandBuffer::CBLevel level, uint32_t count)
	{
		std::vector<std::shared_ptr<CommandBuffer>> cmdBuffers;
		for (uint32_t i = 0; i < count; i++)
			cmdBuffers.push_back(AllocateCommandBuffer(level));

		return cmdBuffers;
	}

	void CommandPool::Reset(const std::shared_ptr<Fence>& pFence)
	{
		//Hold this fence pointer until the end of this function
		std::shared_ptr<Fence> pFenceHolder = pFence;

		if (pFenceHolder.get())
		{
			VkFence fence = pFenceHolder->GetDeviceHandle();
			CHECK_VK_ERROR(vkWaitForFences(m_pDevice->GetDeviceHandle(), 1, &fence, VK_TRUE, UINT64_MAX));
		}

		vkResetCommandPool(m_pDevice->GetDeviceHandle(), m_commandPool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
	}

	CommandPool::~CommandPool()
	{
		vkDestroyCommandPool(m_pDevice->GetDeviceHandle(), m_commandPool, nullptr);
	}
}