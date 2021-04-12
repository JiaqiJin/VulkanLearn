#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "UniqueHandle.h"
#include <string>

/*
* Wrapper class for VkInstance
* Instance is the connection between your application and the Vulkan library.
* This class is responsible for initializing vkInstance and all avaliable extensions and validation layers.
*/
namespace Rendering
{
    class PhysicalDeviceSurfaceContainer;
    class Surface;

    class Instance
    {
    public:
        Instance(const std::string& appName, std::vector<const char*> extensions, bool enableValidation);
        ~Instance();

        // Queries the GPUs of a VkInstance that is already created
        Instance(const Instance&) = default;
        Instance(Instance&&) = default;
        Instance& operator=(const Instance&) = default;
        Instance& operator=(Instance&&) = default;

        VkInstance getHandle() const { return m_handle; }

        // Querying valid physical devices on the machine
        std::vector<PhysicalDeviceSurfaceContainer> findPhysicalDevices(const Surface& surface);

    private:
        // Creating VkInstance object
        void createInstance(const std::string& appName, std::vector<const char*> extensions, bool enableValidation);

    private:
        UniqueHandle<VkInstance> m_handle;

        // validation layers 
        std::vector<VkLayerProperties> m_availableLayers;
        std::vector<const char*> m_availableLayerNames;

        // Extension layers
        std::vector<VkExtensionProperties> m_availableExtensions;
        std::vector<const char*> m_availableExtensionNames;
    };
}