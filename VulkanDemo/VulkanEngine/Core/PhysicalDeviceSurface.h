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

    class PhysicalDeviceSurfaceParameters
    {
    public:
        PhysicalDeviceSurfaceParameters(const PhysicalDevice& physicalDevice, const Surface& surface);
        ~PhysicalDeviceSurfaceParameters() = default;;

        PhysicalDeviceSurfaceParameters(const PhysicalDeviceSurfaceParameters&) = delete;
        PhysicalDeviceSurfaceParameters(PhysicalDeviceSurfaceParameters&&) = default;;
        PhysicalDeviceSurfaceParameters& operator=(const PhysicalDeviceSurfaceParameters&) = delete;
        PhysicalDeviceSurfaceParameters& operator=(PhysicalDeviceSurfaceParameters&&) = delete;

        const VkSurfaceCapabilitiesKHR& getCapabilities() const { return m_capabilities; }
        const std::vector<VkSurfaceFormatKHR>& getFormats() const { return m_formats; }
        std::vector<VkPresentModeKHR> getPresentModes() const { return m_presentModes; }
        bool isPresentationSupported(const QueueFamily& queueFamily) const;

        void onSurfaceChanged();

        const QueueFamilyIndices& getQueueFamilyIndices() const { return *m_queueFamilyIndices; };

    private:
        void queryCapabilities();
        void queryFormats();
        void queryPresentModes();
        void queryPresentationSupport();

    private:
        VkSurfaceCapabilitiesKHR m_capabilities;
        std::vector<VkSurfaceFormatKHR> m_formats;
        std::vector<VkPresentModeKHR> m_presentModes;
        std::vector<bool> m_queuePresentationSupport;

        std::unique_ptr<QueueFamilyIndices> m_queueFamilyIndices;

        const PhysicalDevice& m_physicalDevice;
        const Surface& m_surface;
    };

    class PhysicalDeviceSurfaceContainer
    {
    public:
        PhysicalDeviceSurfaceContainer(PhysicalDevice&& physicalDdevice, const Surface& surface);

        PhysicalDeviceSurfaceContainer(const PhysicalDeviceSurfaceContainer&) = delete;
        PhysicalDeviceSurfaceContainer(PhysicalDeviceSurfaceContainer&&) = default;
        PhysicalDeviceSurfaceContainer& operator=(const PhysicalDeviceSurfaceContainer&) = delete;
        PhysicalDeviceSurfaceContainer& operator=(PhysicalDeviceSurfaceContainer&&) = delete;

        const PhysicalDevice& getPhysicalDevice() const { return m_physicalDevice; }
        const PhysicalDeviceSurfaceParameters& getParameters() const { return m_parameters; }
        PhysicalDeviceSurfaceParameters& getParameters() { return m_parameters; }

    private:
        PhysicalDevice m_physicalDevice;
        const Surface& m_surface;
        PhysicalDeviceSurfaceParameters m_parameters;
    };
}