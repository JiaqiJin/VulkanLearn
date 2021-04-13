#include "QueueFamily.h"
#include "PhysicalDevice.h"
#include "SwapChainSupport.h"

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
        const SwapChainSupportDetails& SwapChainSupportDetails)
    {
        for (const QueueFamily& queueFamily : physicalDevice.getQueueFamilies())
        {
            if (queueFamily.getProperties().queueFlags & VK_QUEUE_GRAPHICS_BIT)
                m_graphicsQueueFamily = &queueFamily;

            if (SwapChainSupportDetails.isPresentationSupported(queueFamily))
                m_presentQueueFamily = &queueFamily;

            if (isValid())
                break;
        }
    }
}