#include "QueueFamily.h"
#include "PhysicalDevice.h"

#include <optional>
#include <vector>

namespace Rendering
{
	QueueFamily::QueueFamily(uint32_t index, VkQueueFamilyProperties properties)
		: m_index(index), m_properties(properties)
	{

	}

	QueueFamilyIndices::QueueFamilyIndices(Rendering::PhysicalDevice& physicalDevice)
	{
		for (const auto& queueFamily : physicalDevice.getQueueFamilies())
		{
			if (queueFamily.getProperties().queueFlags & VK_QUEUE_GRAPHICS_BIT)
				m_graphicsQueueFamily = &queueFamily;

			// TODO
			if (isValid())
				break;
		}
	}

}
