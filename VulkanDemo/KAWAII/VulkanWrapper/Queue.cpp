#include "Queue.h"
#include "Device.h"
#include "Fence.h"
#include "Semaphore.h"
#include "CommandBuffer.h"

#include "../Common/Logger.h"
#include "../Common/Macro.h"

namespace RHI
{
	Queue::Queue(const std::shared_ptr<Device>& pDevice, PhysicalDevice::QueueFamily queueFamily)
		: m_device(pDevice), m_queueFamily(queueFamily)
	{
		if (!Init())
			K_ERROR("Error Initialize Queue Device");
	}

	bool Queue::Init()
	{
		// 1st Queue in a queue family
		vkGetDeviceQueue(m_device->GetDeviceHandle(), m_device->GetPhysicalDevice()->GetQueueFamilyIndex(m_queueFamily), 0, &m_queue);
		return true;
	}

	void Queue::SubmitCommandBuffer(const std::shared_ptr<CommandBuffer>& pCmdBuffer, const std::shared_ptr<Fence>& pFence, bool waitUtilQueueIdle)
	{
		std::vector<std::shared_ptr<CommandBuffer>> v = { pCmdBuffer };
		SubmitCommandBuffers(v, pFence, waitUtilQueueIdle);
	}

	void Queue::SubmitCommandBuffers(const std::vector<std::shared_ptr<CommandBuffer>>& cmdBuffers, const std::shared_ptr<Fence>& pFence, bool waitUtilQueueIdle)
	{
		SubmitCommandBuffers(cmdBuffers, std::vector<std::shared_ptr<Semaphore>>(), 
			std::vector<VkPipelineStageFlags>(), pFence, waitUtilQueueIdle);
	}

	void Queue::SubmitCommandBuffer(
		const std::shared_ptr<CommandBuffer>& pCmdBuffer,
		const std::vector<std::shared_ptr<Semaphore>>& waitSemaphores,
		const std::vector<VkPipelineStageFlags>& waitStages,
		const std::shared_ptr<Fence>& pFence,
		bool waitUtilQueueIdle)
	{
		std::vector<std::shared_ptr<CommandBuffer>> v = { pCmdBuffer };
		SubmitCommandBuffers(v, waitSemaphores, waitStages, pFence, waitUtilQueueIdle);
	}

	void Queue::SubmitCommandBuffers(
		const std::vector<std::shared_ptr<CommandBuffer>>& cmdBuffers,
		const std::vector<std::shared_ptr<Semaphore>>& waitSemaphores,
		const std::vector<VkPipelineStageFlags>& waitStages,
		const std::shared_ptr<Fence>& pFence,
		bool waitUtilQueueIdle)
	{
		SubmitCommandBuffers(cmdBuffers, std::vector<std::shared_ptr<Semaphore>>(), std::vector<VkPipelineStageFlags>(), 
			std::vector<std::shared_ptr<Semaphore>>(), pFence, waitUtilQueueIdle);
	}

	void Queue::SubmitCommandBuffer(
		const std::shared_ptr<CommandBuffer>& pCmdBuffer,
		const std::vector<std::shared_ptr<Semaphore>>& waitSemaphores,
		const std::vector<VkPipelineStageFlags>& waitStages,
		const std::vector<std::shared_ptr<Semaphore>>& signalSemaphores,
		const std::shared_ptr<Fence>& pFence,
		bool waitUtilQueueIdle)
	{
		std::vector<std::shared_ptr<CommandBuffer>> v = { pCmdBuffer };
		SubmitCommandBuffers(v, waitSemaphores, waitStages, signalSemaphores, pFence, waitUtilQueueIdle);
	}

	void Queue::SubmitCommandBuffers(
		const std::vector<std::shared_ptr<CommandBuffer>>& cmdBuffers,
		const std::vector<std::shared_ptr<Semaphore>>& waitSemaphores,
		const std::vector<VkPipelineStageFlags>& waitStages,
		const std::vector<std::shared_ptr<Semaphore>>& signalSemaphores,
		const std::shared_ptr<Fence>& pFence,
		bool waitUtilQueueIdle)
	{
		std::vector<VkCommandBuffer> deviceCmdBuffers;
		for (uint32_t i = 0; i < cmdBuffers.size(); i++)
			deviceCmdBuffers.push_back(cmdBuffers[i]->GetDeviceHandle());

		std::vector<VkSemaphore> deviceWaitSemaphores;
		for (uint32_t i = 0; i < waitSemaphores.size(); i++)
			deviceWaitSemaphores.push_back(waitSemaphores[i]->GetSemaphore());

		std::vector<VkSemaphore> deviceSignalSemaphores;
		for (uint32_t i = 0; i < signalSemaphores.size(); i++)
			deviceSignalSemaphores.push_back(signalSemaphores[i]->GetSemaphore());

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = (uint32_t)deviceCmdBuffers.size();
		submitInfo.pCommandBuffers = deviceCmdBuffers.data();
		submitInfo.waitSemaphoreCount = (uint32_t)deviceWaitSemaphores.size();
		submitInfo.pWaitSemaphores = deviceWaitSemaphores.data();
		submitInfo.pWaitDstStageMask = waitStages.data();
		submitInfo.signalSemaphoreCount = (uint32_t)deviceSignalSemaphores.size();
		submitInfo.pSignalSemaphores = deviceSignalSemaphores.data();

		VkFence fence = 0;
		if (pFence.get())
		{
			ASSERTION(pFence->GetFenceState() == Fence::FenceState::READ_FOR_USE);
			fence = pFence->GetDeviceHandle();
		}

		vkQueueSubmit(m_queue, 1, &submitInfo, fence);

		if (pFence.get())
		{
			pFence->m_fenceState = Fence::FenceState::READ_FOR_SIGNAL;
		}

		if (waitUtilQueueIdle)
		{
			CHECK_VK_ERROR(vkQueueWaitIdle(m_queue));
			if (pFence.get())
			{
				pFence->m_fenceState = Fence::FenceState::SIGNALED;
			}
		}
	}

	void Queue::WaitForIdle()
	{
		CHECK_VK_ERROR(vkQueueWaitIdle(m_queue));
	}

	Queue::~Queue()
	{

	}
}