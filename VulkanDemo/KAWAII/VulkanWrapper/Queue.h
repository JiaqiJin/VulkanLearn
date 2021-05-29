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
	// Wrapper class for VkQueue
	// A physical device may support : Graphic, Compute, Transfer and Sparse.
	class Queue
	{
	public:
		Queue(const std::shared_ptr<Device> pDevice, PhysicalDevice::QueueFamily queueFamily);

		Queue(const Queue&) = default;
		Queue& operator=(const Queue&) = delete;
		Queue& operator=(Queue&&) = delete;

		~Queue();

		void WaitForIdle();

		// Getters
		VkQueue GetQueue() { return m_queue; }
		PhysicalDevice::QueueFamily GetQueueFamily() const { return m_queueFamily; }

	private:
		bool Init(const std::shared_ptr<Device>& pDevice, PhysicalDevice::QueueFamily queueFamily);

	private:
		std::shared_ptr<Device> m_device;
		VkQueue m_queue;
		PhysicalDevice::QueueFamily m_queueFamily;
	};
}
