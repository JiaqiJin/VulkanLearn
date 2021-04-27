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
        Instance(const std::string& appName, std::vector<const char*> extensions, bool enableValidation, bool enableApiDump);
        ~Instance();

        Instance(const Instance&) = default;
        Instance(Instance&&) = default;
        Instance& operator=(const Instance&) = default;
        Instance& operator=(Instance&&) = default;

        VkInstance getHandle() const { return m_handle; }

        std::vector<PhysicalDeviceSurfaceContainer> findPhysicalDevices(const Surface& surface);

    private:
        void createInstance(const std::string& appName, std::vector<const char*> extensions, bool enableValidation, bool enableApiDump);

    private:
        UniqueHandle<VkInstance> m_handle;

        std::vector<VkLayerProperties> m_availableLayers;
        std::vector<const char*> m_availableLayerNames;

        std::vector<VkExtensionProperties> m_availableExtensions;
        std::vector<const char*> m_availableExtensionNames;
    };
}