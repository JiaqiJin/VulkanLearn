#include "QueueFamilyIndices.h"

#include <optional>
#include <vector>

#include "PhysicalDevice.h"
#include "PhysicalDeviceSurfaceParameters.h"
#include "QueueFamily.h"

namespace Rendering
{
    QueueFamilyIndices::QueueFamilyIndices(const PhysicalDevice& physicalDevice, const PhysicalDeviceSurfaceParameters& physicalDeviceSurfaceParameters)
    {
        for (const QueueFamily& queueFamily : physicalDevice.getQueueFamilies())
        {
            if (queueFamily.getProperties().queueFlags & VK_QUEUE_GRAPHICS_BIT)
                m_graphicsQueueFamily = &queueFamily;

            if (physicalDeviceSurfaceParameters.isPresentationSupported(queueFamily))
                m_presentQueueFamily = &queueFamily;

            if (isValid())
                break;
        }
    }
}