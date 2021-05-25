#include "Queue.h"
#include "Device.h"

namespace RHI
{
	Queue::Queue(const std::shared_ptr<Device> pDevice, const std::shared_ptr<Queue> pSelf, PhysicalDevice::QueueFamily queueFamily)
	{
		if (!Init(pDevice, pSelf, queueFamily))
			std::cout << "Error Initialize Queue " << std::endl;
	}

	bool Queue::Init(const std::shared_ptr<Device>& pDevice, const std::shared_ptr<Queue>& pSelf, PhysicalDevice::QueueFamily queueFamily)
	{
		if (!DeviceObjectBase::Init(pDevice, pSelf))
			return false;

		// 1st Queue in a queue family
		vkGetDeviceQueue(pDevice->GetDeviceHandle(), pDevice->GetPhysicalDevice()->GetQueueFamilyIndex(queueFamily), 0, &m_queue);
		m_queueFamily = queueFamily;
		return true;
	}

	Queue::~Queue()
	{

	}
}