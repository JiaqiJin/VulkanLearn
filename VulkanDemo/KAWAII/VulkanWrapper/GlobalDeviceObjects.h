#pragma once

#include <vulkan/vulkan.h>
#include "Device.h"
#include "../Common/Singleton.h"
#include "PhysicalDevice.h"

#include <map>

namespace RHI
{
	class Queue;
	class SwapChain;
	class PhysicalDevice;
	class CommandPool;
	class DeviceMemoryManager;

	class GlobalDeviceObjects;

	class GlobalDeviceObjects : public Singleton<GlobalDeviceObjects>
	{
	public:
		bool InitObjects(const std::shared_ptr<Device>& pDevice);

		~GlobalDeviceObjects();

	protected:
		std::shared_ptr<Device> m_pDevice;
		std::shared_ptr<Queue> m_queues[(uint32_t)PhysicalDevice::QueueFamily::COUNT];
		std::shared_ptr<CommandPool> m_pMainThreadCommandPools[(uint32_t)PhysicalDevice::QueueFamily::COUNT];
		std::shared_ptr<DeviceMemoryManager> m_pDeviceMemMgr;

		std::shared_ptr<SwapChain> m_pSwapChain;

	};
}