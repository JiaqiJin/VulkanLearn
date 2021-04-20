#include "CommandBuffers.h"
#include "Device.h"
#include "CommandPool.h"
#include "Semaphore.h"
#include "Fence.h"
#include "Queue.h"

#include <stdexcept>

namespace Rendering
{
	CommandBuffersContainer::CommandBuffersContainer(const Device& device, const CommandPool& commandPool, size_t size)
		: m_device(device)
		, m_commandPool(commandPool)
	{
		VkCommandBufferAllocateInfo commandBufferAllocateInfo{};
		commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		commandBufferAllocateInfo.commandPool = m_commandPool.getHandle();
		commandBufferAllocateInfo.commandBufferCount = static_cast<uint32_t>(size);

		m_handles.resize(size);
		vkAllocateCommandBuffers(m_device.getHandle(), &commandBufferAllocateInfo, m_handles.data());
	}

	CommandBuffersContainer::~CommandBuffersContainer()
	{
		vkFreeCommandBuffers(m_device.getHandle(), m_commandPool.getHandle(), static_cast<uint32_t>(m_handles.size()), m_handles.data());
	}

	//  ----------------------- Command Buffer ---------------------------
	CommandBuffer::CommandBuffer(const std::shared_ptr<CommandBuffersContainer>& container, std::size_t index)
		: m_container(container)
		, m_index(index)
	{

	}

	const VkCommandBuffer& CommandBuffer::getHandle() const
	{
		return m_container->getHandle(m_index);
	}

	void CommandBuffer::reset() const
	{
		vkResetCommandBuffer(getHandle(), 0);
	}

	void CommandBuffer::begin(bool oneTime) const
	{
		VkCommandBufferUsageFlags flags = 0;
		if(oneTime)
			flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		VkCommandBufferBeginInfo commandBufferBeginInfo{};
		commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		commandBufferBeginInfo.flags = flags;
		commandBufferBeginInfo.pInheritanceInfo = nullptr;

		if (vkBeginCommandBuffer(getHandle(), &commandBufferBeginInfo) != VK_SUCCESS)
			throw std::runtime_error("failed to begin recording command buffer!");
	}

	void CommandBuffer::end() const
	{
		if (vkEndCommandBuffer(getHandle()) != VK_SUCCESS)
			throw std::runtime_error("failed to record command buffer!");
	}

	// Queue submission and synchronization
	void CommandBuffer::submit(const Queue& queue, const Semaphore* signalSemaphore, const Semaphore* waitSemaphore,
		const Fence* signalFence) const
	{
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		std::vector<VkSemaphore> signalSemaphores;
		if (signalSemaphore)
			signalSemaphores.push_back(signalSemaphore->getHandle());
		submitInfo.signalSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size());;
		submitInfo.pSignalSemaphores = signalSemaphores.data();

		std::vector<VkSemaphore> waitSemaphores;
		std::vector<VkPipelineStageFlags> waitStages;
		if (waitSemaphore)
		{
			waitSemaphores.push_back(waitSemaphore->getHandle());
			waitStages.push_back(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
		}
		submitInfo.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size());
		submitInfo.pWaitSemaphores = waitSemaphores.data();
		submitInfo.pWaitDstStageMask = waitStages.data();

		VkFence signalFenceHandle = signalFence ? signalFence->getHandle() : VK_NULL_HANDLE;
		// submit info to the graphic queue
		if (vkQueueSubmit(queue.getHandle(), 1, &submitInfo, signalFenceHandle) != VK_SUCCESS)
			throw std::runtime_error("failed to submit draw command buffer!");
	}

}