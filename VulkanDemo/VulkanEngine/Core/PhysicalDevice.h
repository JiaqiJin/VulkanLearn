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
        PhysicalDevice() {}
        PhysicalDevice(VkPhysicalDevice handle);
       
        VkPhysicalDevice getHandle() const { return m_handle; }

        // Getters
        const VkPhysicalDeviceProperties& getProperties() const { return m_properties; }
        const VkPhysicalDeviceFeatures& getFeatures() const { return m_features; }
        const std::vector<QueueFamily>& getQueueFamilies() const { return m_queueFamilies; }

        // Check supported extension
        bool areExtensionsSupported(std::vector<char const*> const& requestedExtensions) const;

    private:
        void init();

        void queryAvailableExtensions();
        void queryProperties();
        void queryFeatures();
        void queryQueueFamilyProperties();

    private:
        VkPhysicalDevice m_handle = VK_NULL_HANDLE;

        // Holds the extension properties
        std::vector<VkExtensionProperties> m_availableExtensions;
        std::vector<char const*> m_availableExtensionNames;

        // The GPU properties
        VkPhysicalDeviceProperties m_properties;
        // The features that will be requested to be enabled in the logical device
        VkPhysicalDeviceFeatures m_features;
        // The queue family properties
        std::vector<QueueFamily> m_queueFamilies;
    };
}