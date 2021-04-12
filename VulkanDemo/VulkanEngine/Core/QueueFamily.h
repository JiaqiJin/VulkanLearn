#pragma once

#include <vulkan/vulkan.h>

/*
* Wrapper class for different type of the queues that originate from different queue families (graphics families and present families)
* Support graphic command and present commands.
* Graphics queues can run graphics pipeline (vkCmdDraw)
* Compute queues can run compute pipelines (vkCmdDispatch)
* Transfer queues can perform transfer (copy) operations (vkCmdCopy)
* Some queues can perform multiple kinds of operations.
*/

namespace Rendering
{
    class QueueFamily
    {
    public:
        QueueFamily(uint32_t index, VkQueueFamilyProperties properties);

        uint32_t getIndex() const { return m_index; }
        const VkQueueFamilyProperties& getProperties() const { return m_properties; }

    private:
        uint32_t m_index;
        VkQueueFamilyProperties m_properties;
    };

    // Forward declaration
    class PhysicalDevice;
    class SwapChainSupportDetails;
    class QueueFamily;

    class QueueFamilyIndices
    {
    public:
        QueueFamilyIndices(const PhysicalDevice& physicalDevice, const SwapChainSupportDetails& SwapChainSupportDetails);

        bool isValid() const { return m_graphicsQueueFamily && m_presentQueueFamily; }

        const QueueFamily& getGraphicsQueueFamily() const { return *m_graphicsQueueFamily; }
        const QueueFamily& getPresentQueueFamily() const { return *m_presentQueueFamily; }

    private:
        const QueueFamily* m_graphicsQueueFamily = nullptr;
        const QueueFamily* m_presentQueueFamily = nullptr;
    };
}