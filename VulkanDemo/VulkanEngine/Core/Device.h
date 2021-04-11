#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "UniqueHandle.h"

/*
* Wrapper class for VkDevice 
* Need specifying the queues to be created and device feature for the logical device creating
*/
namespace Rendering
{
	class Queue;
	class PhysicalDeviceSurfaceParameters;

	class Device
	{
	public:
		Device(const PhysicalDeviceSurfaceParameters& physicalDeviceSurfaceParam, std::vector<const char*> const& extensions);
		~Device();

		Device(const Device&) = default;
		Device(Device&&) = default;
		Device& operator=(const Device&) = default;
		Device& operator=(Device&&) = default;

		void waitIdle() const;

		VkDevice getHandle() const { return m_handle; }
		const Queue& getGraphicsQueue() const { return *m_graphicsQueue; }
		const Queue& getPresentQueue() const { return *m_presentQueue; }

	private:
		UniqueHandle<VkDevice> m_handle;
		
		std::vector<Queue> m_queues;

		Queue const* m_graphicsQueue = nullptr;
		Queue const* m_presentQueue = nullptr;
	};
}