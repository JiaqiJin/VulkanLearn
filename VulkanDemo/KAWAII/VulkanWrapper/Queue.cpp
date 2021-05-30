#include "Queue.h"
#include "Device.h"
#include "Fence.h"
#include "Semaphore.h"

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

	void Queue::WaitForIdle()
	{
		CHECK_VK_ERROR(vkQueueWaitIdle(m_queue));
	}

	Queue::~Queue()
	{

	}
}