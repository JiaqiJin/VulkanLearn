#include "QueueFamily.h"
#include "PhysicalDevice.h"
#include "PhysicalDeviceSurface.h"

#include <optional>
#include <vector>

namespace Rendering
{
    QueueFamily::QueueFamily(uint32_t index, VkQueueFamilyProperties properties)
        : m_index(index)
        , m_properties(properties)
    {

    }

    QueueFamilyIndices::QueueFamilyIndices(const PhysicalDevice& physicalDevice, 
        const SwapChainSupportDetails& support)
    {
        for (const QueueFamily& queueFamily : physicalDevice.getQueueFamilies())
        {
            if (queueFamily.getProperties().queueFlags & VK_QUEUE_GRAPHICS_BIT)
                m_graphicsQueueFamily = &queueFamily;

            if (support.isPresentationSupported(queueFamily))
                m_presentQueueFamily = &queueFamily;

            if (isValid())
                break;
        }
    }
}