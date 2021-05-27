#pragma once

#include <vulkan/vulkan.h>
#include "PhysicalDevice.h"
#include <memory>

namespace RHI
{
	class Device;

	class Queue
	{
	public:
		Queue(const std::shared_ptr<Device> pDevice, PhysicalDevice::QueueFamily queueFamily);

		Queue(const Queue&) = default;
		Queue& operator=(const Queue&) = delete;
		Queue& operator=(Queue&&) = delete;

		~Queue();

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
