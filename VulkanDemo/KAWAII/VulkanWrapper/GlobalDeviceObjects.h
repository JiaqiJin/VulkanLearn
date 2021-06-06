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

	//class Image;
	//class MemoryKey;
	class StagingBuffer;
	class Buffer;
	class GlobalDeviceObjects;

	// ---------------- Global Getters -----------------------
	GlobalDeviceObjects* GlobalObjects();
	const std::shared_ptr<CommandPool>& MainThreadCommandPool(PhysicalDevice::QueueFamily queueFamily);
	const std::shared_ptr<DeviceMemoryManager>& DeviceMemMgr();
	const std::shared_ptr<SwapChain>& GetSwapChain();
	const std::shared_ptr<Device>& GetDevice();
	std::shared_ptr<PhysicalDevice> GetPhysicalDevice();

	class GlobalDeviceObjects : public Singleton<GlobalDeviceObjects>
	{
	public:
		// Getters
		const std::shared_ptr<Device>& GetDevice() const { return m_pDevice; }
		const std::shared_ptr<Queue>& GetQueue(PhysicalDevice::QueueFamily queueFamily) const { return m_queues[(uint32_t)queueFamily]; }
		const std::shared_ptr<CommandPool>& GetMainThreadCommandPool(PhysicalDevice::QueueFamily queueFamily) { return m_pMainThreadCommandPools[(uint32_t)queueFamily]; }
		const std::shared_ptr<DeviceMemoryManager>& GetDeviceMemMgr() const { return m_pDeviceMemMgr; }
		const std::shared_ptr<SwapChain>& GetSwapChain() const { return m_pSwapChain; }

		bool InitObjects(const std::shared_ptr<Device>& pDevice);

		~GlobalDeviceObjects();

	protected:
		std::shared_ptr<Device> m_pDevice;
		std::shared_ptr<Queue> m_queues[(uint32_t)PhysicalDevice::QueueFamily::COUNT];
		std::shared_ptr<CommandPool> m_pMainThreadCommandPools[(uint32_t)PhysicalDevice::QueueFamily::COUNT];
		std::shared_ptr<DeviceMemoryManager> m_pDeviceMemMgr;

		std::shared_ptr<SwapChain> m_pSwapChain;

		std::shared_ptr<StagingBuffer> m_bufferTest;
		std::shared_ptr<Buffer> m_buffer;

	/*	std::shared_ptr<MemoryKey> test_node;
		std::shared_ptr<Image> m_image;*/
	};
}