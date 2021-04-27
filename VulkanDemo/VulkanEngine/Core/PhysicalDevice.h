#pragma once

#include <vulkan/vulkan.h>
#include "QueueFamily.h"
#include <vector>

/*
* Wrapper class for VkPhysicalDevice
* This class is responsible for handling gpu features, properties, and queue families for the device creation.
*/
namespace Rendering
{
    class PhysicalDevice
    {
    public:
        explicit PhysicalDevice(VkPhysicalDevice handle);

        VkPhysicalDevice getHandle() const { return m_handle; }

        const VkPhysicalDeviceProperties& getProperties() const { return m_properties; }
        const VkPhysicalDeviceFeatures& getFeatures() const { return m_features; }
        const std::vector<QueueFamily>& getQueueFamilies() const { return m_queueFamilies; }

        bool areExtensionsSupported(std::vector<const char*> const& requestedExtensions) const;

    private:
        void init();

        void queryAvailableExtensions();
        void queryProperties();
        void queryFeatures();
        void queryQueueFamilyProperties();

    private:
        VkPhysicalDevice m_handle = VK_NULL_HANDLE;

        std::vector<VkExtensionProperties> m_availableExtensions;
        std::vector<const char*> m_availableExtensionNames;

        VkPhysicalDeviceProperties m_properties;
        VkPhysicalDeviceFeatures m_features;
        std::vector<QueueFamily> m_queueFamilies;
    };
}