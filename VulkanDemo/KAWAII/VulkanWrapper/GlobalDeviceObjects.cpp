#include "GlobalDeviceObjects.h"
#include "PhysicalDevice.h"
#include "Queue.h"
#include "SwapChain.h"

namespace RHI
{
	bool GlobalDeviceObjects::InitObjects(const std::shared_ptr<Device>& pDevice)
	{
		m_pDevice = pDevice;

		for (uint32_t i = 0; i < (uint32_t)PhysicalDevice::QueueFamily::COUNT; i++)
		{
			m_queues[i] = std::make_shared<Queue>(pDevice, (PhysicalDevice::QueueFamily)i);
			// TODO
		}

		m_pSwapChain = std::make_shared<SwapChain>(pDevice);

		return true;
	}
}