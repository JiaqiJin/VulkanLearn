#include "Queue.h"
#include "Device.h"

#include "../Common/Logger.h"
#include "../Common/Macro.h"

namespace RHI
{
	Queue::Queue(const std::shared_ptr<Device> pDevice, PhysicalDevice::QueueFamily queueFamily)
		: m_device(pDevice)
	{
		std::shared_ptr<Queue> pQueue = std::make_shared<Queue>(pDevice, queueFamily);
		if (!Init(pDevice, queueFamily))
			K_ERROR("Error Initialize Queue Device");
	}

	bool Queue::Init(const std::shared_ptr<Device>& pDevice, PhysicalDevice::QueueFamily queueFamily)
	{
		// 1st Queue in a queue family
		vkGetDeviceQueue(m_device->GetDeviceHandle(), pDevice->GetPhysicalDevice()->GetQueueFamilyIndex(queueFamily), 0, &m_queue);
		m_queueFamily = queueFamily;
		return true;
	}

	Queue::~Queue()
	{

	}
}