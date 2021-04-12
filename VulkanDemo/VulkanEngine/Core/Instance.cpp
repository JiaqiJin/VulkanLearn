#include "Instance.h"
#include "PhysicalDevice.h"
#include "Utils.h"
#include "PhysicalDeviceSurface.h"
#include <stdexcept>

namespace
{
    std::vector<VkLayerProperties> getAvailableLayers()
    {
        uint32_t count = 0;
        vkEnumerateInstanceLayerProperties(&count, nullptr);
        std::vector<VkLayerProperties> availableLayers(count);
        vkEnumerateInstanceLayerProperties(&count, availableLayers.data());

        return availableLayers;
    }

    std::vector<VkExtensionProperties> getAvailableExtensions()
    {
        uint32_t count = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
        std::vector<VkExtensionProperties> availableExtensions(count);
        vkEnumerateInstanceExtensionProperties(nullptr, &count, availableExtensions.data());

        return availableExtensions;
    }
}

namespace Rendering
{
    Instance::Instance(const std::string& appName, std::vector<const char*> extensions, bool enableValidation)
    {
        m_availableLayers = getAvailableLayers();
        m_availableLayerNames.reserve(m_availableLayers.size());
        for (const auto& layer : m_availableLayers)
            m_availableLayerNames.push_back(layer.layerName);

        m_availableExtensions = getAvailableExtensions();
        m_availableExtensionNames.reserve(m_availableExtensions.size());
        for (const auto& extension : m_availableExtensions)
            m_availableExtensionNames.push_back(extension.extensionName);

        createInstance(appName, extensions, enableValidation);
    }

    Rendering::Instance::~Instance()
    {
        vkDestroyInstance(m_handle, nullptr);
    }

    void Instance::createInstance(const std::string& appName, std::vector<const char*> extensions, bool enableValidation)
    {
        std::vector<const char*> requestedLayers;
        if (enableValidation)
            requestedLayers.push_back("VK_LAYER_KHRONOS_validation");

        std::vector<const char*> requestedExtensions = extensions;

        if (!utils::checkSupportOption(m_availableLayerNames, requestedLayers))
            throw std::runtime_error("Some of the required validation layers aren't supported");

        if (!utils::checkSupportOption(m_availableExtensionNames, requestedExtensions))
            throw std::runtime_error("Some of the required extensions aren't supported");

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = appName.c_str();
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo instanceCreateInfo{};
        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pApplicationInfo = &appInfo;
        instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(requestedExtensions.size());
        instanceCreateInfo.ppEnabledExtensionNames = requestedExtensions.data();
        instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(requestedLayers.size());
        instanceCreateInfo.ppEnabledLayerNames = requestedLayers.data();

        if (vkCreateInstance(&instanceCreateInfo, nullptr, &m_handle.get()) != VK_SUCCESS)
            throw std::runtime_error("Failed to create Vulkan instance");
    }

    std::vector<PhysicalDevice> Instance::findPhysicalDevices(const Surface& surface)
    {
        uint32_t count = 0;
        vkEnumeratePhysicalDevices(m_handle, &count, nullptr);
        if (count == 0)
            throw std::runtime_error("failed to find GPUs with Vulkan support!");

        std::vector<VkPhysicalDevice> physicalDeviceHandles(count);
        vkEnumeratePhysicalDevices(m_handle, &count, physicalDeviceHandles.data());

        std::vector<PhysicalDevice> physicalDevices;
        physicalDevices.reserve(count);
        for (auto const& handle : physicalDeviceHandles)
            physicalDevices.emplace_back(PhysicalDevice{ handle });

        return physicalDevices;
    }
}