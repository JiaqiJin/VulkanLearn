#pragma once

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>
#include <vector>
#include "UniqueHandle.h"

/*
* Wrapper class for VkDevice
* Need specifying the queues to be created and device feature for the logical device creating
*/
namespace Rendering
{
	class Queue;
	class PhysicalDevice;
	class Instance;
	class QueueFamilyIndices;

	class Device
	{
	public:
		Device(const PhysicalDevice& device, const QueueFamilyIndices& indice, const Instance& instance, const std::vector<const char*>& extensions);
		~Device();

		Device(const Device&) = default;
		Device(Device&&) = default;
		Device& operator=(const Device&) = default;
		Device& operator=(Device&&) = default;

		void waitIdle() const;

		VkDevice getHandle() const { return m_handle; }
		VmaAllocator getMemoryAllocator() const { return m_memoryAllocator; }
		const Queue& getGraphicsQueue() const { return *m_graphicsQueue; }
		const Queue& getPresentQueue() const { return *m_presentQueue; }

	private:
		UniqueHandle<VkDevice> m_handle;

		std::vector<Queue> m_queues;

		VmaAllocator m_memoryAllocator{ VK_NULL_HANDLE };

		Queue const* m_graphicsQueue = nullptr;
		Queue const* m_presentQueue = nullptr;
	};
}