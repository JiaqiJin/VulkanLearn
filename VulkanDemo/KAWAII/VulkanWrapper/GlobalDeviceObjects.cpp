#include "GlobalDeviceObjects.h"
#include "PhysicalDevice.h"
#include "Queue.h"
#include "SwapChain.h"
#include "CommandPool.h"
#include "DeviceMemoryManager.h"
#include "Image.h"
namespace RHI
{
	bool GlobalDeviceObjects::InitObjects(const std::shared_ptr<Device>& pDevice)
	{
		m_pDevice = pDevice;

		for (uint32_t i = 0; i < (uint32_t)PhysicalDevice::QueueFamily::COUNT; i++)
		{
			m_queues[i] = std::make_shared<Queue>(pDevice, (PhysicalDevice::QueueFamily)i);
			m_pMainThreadCommandPools[i] = std::make_shared<CommandPool>(m_pDevice, PhysicalDevice::QueueFamily::ALL_ROUND, CommandPool::CBPersistancy::PERSISTANT);
		}
		
		if (m_pDeviceMemMgr == nullptr)
			m_pDeviceMemMgr = std::make_shared<DeviceMemoryManager>(m_pDevice);

		m_pSwapChain = std::make_shared<SwapChain>(pDevice);
	
	/*	m_image = std::make_shared<Image>(m_pDevice);
		test_node = m_pDeviceMemMgr->AllocateImageMemChunk(m_image.get(), VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);*/

		return true;
	}

	GlobalDeviceObjects* GlobalObjects()
	{
		return GlobalDeviceObjects::GetInstance();
	}

	GlobalDeviceObjects::~GlobalDeviceObjects()
	{

	}

	const std::shared_ptr<CommandPool>& MainThreadCommandPool(PhysicalDevice::QueueFamily queueFamily) { return GlobalObjects()->GetMainThreadCommandPool(queueFamily); }
	const std::shared_ptr<DeviceMemoryManager>& DeviceMemMgr() { return GlobalObjects()->GetDeviceMemMgr(); }
	const std::shared_ptr<SwapChain>& GetSwapChain() { return GlobalObjects()->GetSwapChain(); }
	const std::shared_ptr<Device>& GetDevice() { return GlobalObjects()->GetDevice(); }
	std::shared_ptr<PhysicalDevice> GetPhysicalDevice() { return GetDevice()->GetPhysicalDevice(); }
}