#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <memory>

#include "PhysicalDevice.h"
#include "Surface.h"

/*
* Wrapper class for checking physical device and surface properties use for querying details of swap chain support
* PhysicalDeviceSurfaceParameters class represents all parameters to evaluate features of physical device and surface.
* Basic properties need to check : Surface capabilities(min/max number img in swap chain),
* Surface format and Avaliable presentation modes. 
*/
namespace Rendering
{
    class PhysicalDevice;
    class Surface;
    class QueueFamilyIndices;
    class QueueFamily;

    class SwapChainSupportDetails
    {
    public:
        SwapChainSupportDetails(const PhysicalDevice& physicalDevice, const Surface& surface);
        ~SwapChainSupportDetails() = default;

        const VkSurfaceCapabilitiesKHR& getCapabilities() const { return m_capabilities; }
        const std::vector<VkSurfaceFormatKHR>& getFormats() const { return m_formats; }
        std::vector<VkPresentModeKHR> getPresentModes() const { return m_presentModes; }
        bool isPresentationSupported(const QueueFamily& queueFamily) const;

        void onSurfaceChanged(const PhysicalDevice& physicalDevice, const Surface& surface);

        const QueueFamilyIndices& getQueueFamilyIndices() const { return *m_queueFamilyIndices; };

    private:
        void queryCapabilities(const PhysicalDevice& physicalDevice, const Surface& surface);
        void queryFormats(const PhysicalDevice& physicalDevice, const Surface& surface);
        void queryPresentModes(const PhysicalDevice& physicalDevice, const Surface& surface);
        void queryPresentationSupport(const PhysicalDevice& physicalDevice, const Surface& surface);

    private:
        VkSurfaceCapabilitiesKHR m_capabilities;
        std::vector<VkSurfaceFormatKHR> m_formats;
        std::vector<VkPresentModeKHR> m_presentModes;
        std::vector<bool> m_queuePresentationSupport;

        std::unique_ptr<QueueFamilyIndices> m_queueFamilyIndices;
    };
}