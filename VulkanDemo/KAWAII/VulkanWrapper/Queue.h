#pragma once

#include <vulkan/vulkan.h>
#include "PhysicalDevice.h"
#include <memory>
#include <vector>

namespace RHI
{
	class Device;
	class Fence;
	class Semaphore;
	class CommandBuffer;

	// Wrapper class for VkQueue
	// A physical device may support : Graphic, Compute, Transfer and Sparse.
	class Queue
	{
	public:
		Queue(const std::shared_ptr<Device>& pDevice, PhysicalDevice::QueueFamily queueFamily);

		Queue(const Queue&) = default;
		Queue& operator=(const Queue&) = delete;
		Queue& operator=(Queue&&) = delete;

		~Queue();

		// Submitting the command buffer
		// Queue submission and synchronization is configured through parameters in the VkSubmitInfo structure.
		void SubmitCommandBuffer(const std::shared_ptr<CommandBuffer>& pCmdBuffer, const std::shared_ptr<Fence>& pFence, bool waitUtilQueueIdle = false);
		void SubmitCommandBuffers(const std::vector<std::shared_ptr<CommandBuffer>>& cmdBuffers, const std::shared_ptr<Fence>& pFence, bool waitUtilQueueIdle = false);

		void SubmitCommandBuffer(
			const std::shared_ptr<CommandBuffer>& pCmdBuffer,
			const std::vector<std::shared_ptr<Semaphore>>& waitSemaphores,
			const std::vector<VkPipelineStageFlags>& waitStages,
			const std::shared_ptr<Fence>& pFence,
			bool waitUtilQueueIdle = false);
		void SubmitCommandBuffers(
			const std::vector<std::shared_ptr<CommandBuffer>>& cmdBuffers,
			const std::vector<std::shared_ptr<Semaphore>>& waitSemaphores,
			const std::vector<VkPipelineStageFlags>& waitStages,
			const std::shared_ptr<Fence>& pFence,
			bool waitUtilQueueIdle = false);

		void SubmitCommandBuffer(
			const std::shared_ptr<CommandBuffer>& pCmdBuffer,
			const std::vector<std::shared_ptr<Semaphore>>& waitSemaphores,
			const std::vector<VkPipelineStageFlags>& waitStages,
			const std::vector<std::shared_ptr<Semaphore>>& signalSemaphores,
			const std::shared_ptr<Fence>& pFence,
			bool waitUtilQueueIdle = false);
		void SubmitCommandBuffers(
			const std::vector<std::shared_ptr<CommandBuffer>>& cmdBuffers,
			const std::vector<std::shared_ptr<Semaphore>>& waitSemaphores,
			const std::vector<VkPipelineStageFlags>& waitStages,
			const std::vector<std::shared_ptr<Semaphore>>& signalSemaphores,
			const std::shared_ptr<Fence>& pFence,
			bool waitUtilQueueIdle = false);

		void WaitForIdle();

		// Getters
		VkQueue GetQueue() { return m_queue; }
		PhysicalDevice::QueueFamily GetQueueFamily() const { return m_queueFamily; }

	private:
		bool Init();

	private:
		std::shared_ptr<Device> m_device;
		VkQueue m_queue;
		PhysicalDevice::QueueFamily m_queueFamily;
	};
}
