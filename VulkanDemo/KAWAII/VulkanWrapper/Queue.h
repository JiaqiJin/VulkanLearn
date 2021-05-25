#pragma once

#include "DeviceObjectBase.h"
#include "PhysicalDevice.h"

namespace RHI
{
	class Device;

	class Queue : public DeviceObjectBase<Queue>
	{
	public:
		Queue(const std::shared_ptr<Device> pDevice, const std::shared_ptr<Queue> pSelf, PhysicalDevice::QueueFamily queueFamily);

		Queue(const Queue&) = default;
		Queue& operator=(const Queue&) = delete;
		Queue& operator=(Queue&&) = delete;

		~Queue();

		// Getters
		VkQueue GetQueue() { return m_queue; }
		PhysicalDevice::QueueFamily GetQueueFamily() const { return m_queueFamily; }

	private:
		bool Init(const std::shared_ptr<Device>& pDevice, const std::shared_ptr<Queue>& pSelf, PhysicalDevice::QueueFamily queueFamily);

	private:
		VkQueue m_queue;
		PhysicalDevice::QueueFamily m_queueFamily;
	};
}
