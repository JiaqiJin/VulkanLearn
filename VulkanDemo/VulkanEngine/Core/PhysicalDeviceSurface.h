#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <memory>

namespace Rendering
{
    class PhysicalDevice;
    class Surface;
    class QueueFamilyIndices;
    class QueueFamily;

    class PhysicalDeviceSurfaceParameters
    {
    public:
        PhysicalDeviceSurfaceParameters(PhysicalDevice& physicalDevice, Surface& surface);
        ~PhysicalDeviceSurfaceParameters();

        PhysicalDeviceSurfaceParameters(PhysicalDeviceSurfaceParameters const&) = delete;
        PhysicalDeviceSurfaceParameters(PhysicalDeviceSurfaceParameters&&);
        PhysicalDeviceSurfaceParameters& operator=(PhysicalDeviceSurfaceParameters const&) = delete;
        PhysicalDeviceSurfaceParameters& operator=(PhysicalDeviceSurfaceParameters&&) = delete;

        VkSurfaceCapabilitiesKHR const& getCapabilities() const { return m_capabilities; }
        std::vector<VkSurfaceFormatKHR> const& getFormats() const { return m_formats; }
        std::vector<VkPresentModeKHR> getPresentModes() const { return m_presentModes; }

        bool isPresentationSupported(QueueFamily const& queueFamily) const;

        void onSurfaceChanged();

        QueueFamilyIndices const& getQueueFamilyIndices() const { return *m_queueFamilyIndices; };

    private:
        void queryCapabilities();
        void queryFormats();
        void queryPresentModes();
        void queryPresentationSupport();

    private:
        // Swap support detail
        VkSurfaceCapabilitiesKHR m_capabilities;
        std::vector<VkSurfaceFormatKHR> m_formats;
        std::vector<VkPresentModeKHR> m_presentModes;
        std::vector<bool> m_queuePresentationSupport;

        std::unique_ptr<QueueFamilyIndices> m_queueFamilyIndices;

        PhysicalDevice& m_physicalDevice;
        Surface& m_surface;
    };
}