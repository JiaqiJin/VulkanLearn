#pragma once

#include <vulkan\vulkan.h>

namespace Rendering
{
    class PhysicalDevice;
    class PhysicalDeviceSurfaceParameters;
    class QueueFamily;

    class QueueFamilyIndices
    {
    public:
        QueueFamilyIndices(const PhysicalDevice& physicalDevice, const PhysicalDeviceSurfaceParameters& physicalDeviceSurfaceParameters);

        bool isValid() const { return m_graphicsQueueFamily && m_presentQueueFamily; }

        const QueueFamily& getGraphicsQueueFamily() const { return *m_graphicsQueueFamily; }
        const QueueFamily& getPresentQueueFamily() const { return *m_presentQueueFamily; }

    private:
        const QueueFamily* m_graphicsQueueFamily = nullptr;
        const QueueFamily* m_presentQueueFamily = nullptr;
    };
}