#include "CommandPool.h"
#include "Device.h"
#include "SwapChainSupport.h"
#include "CommandBuffers.h"

namespace Rendering
{
	CommandPool::CommandPool(const Device& device, const SwapChainSupportDetails& detail)
		:m_device(device)
	{
		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = detail.getQueueFamilyIndices().getGraphicsQueueFamily().getIndex();
		poolInfo.flags = 0;

		if (vkCreateCommandPool(m_device.getHandle(), &poolInfo, nullptr, &m_handle.get()) != VK_SUCCESS) 
			throw std::runtime_error("failed to create command pool!");
	}

	CommandPool::~CommandPool()
	{
		vkDestroyCommandPool(m_device.getHandle(), m_handle, nullptr);
	}

	CommandBuffer CommandPool::createCommandBuffer() const
	{
		return createCommandBuffers(1)[0];
	}

	std::vector<CommandBuffer> CommandPool::createCommandBuffers(std::size_t size) const
	{
		std::shared_ptr<CommandBuffersContainer> container = std::make_shared<CommandBuffersContainer>(m_device, *this, size);

		std::vector<CommandBuffer> commandBuffers;
		commandBuffers.reserve(size);
		for (size_t i = 0; i < size; i++)
		{
			commandBuffers.emplace_back(container, i);
		}
		return commandBuffers;
	}

}