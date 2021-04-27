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
        PhysicalDeviceSurfaceParameters(const PhysicalDevice& physicalDevice, Surface const& surface);
        ~PhysicalDeviceSurfaceParameters();

        PhysicalDeviceSurfaceParameters(const PhysicalDeviceSurfaceParameters&) = delete;
        PhysicalDeviceSurfaceParameters(PhysicalDeviceSurfaceParameters&&);
        PhysicalDeviceSurfaceParameters& operator=(const PhysicalDeviceSurfaceParameters&) = delete;
        PhysicalDeviceSurfaceParameters& operator=(PhysicalDeviceSurfaceParameters&&) = delete;

        const VkSurfaceCapabilitiesKHR& getCapabilities() const { return m_capabilities; }
        const std::vector<VkSurfaceFormatKHR>& getFormats() const { return m_formats; }
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
        VkSurfaceCapabilitiesKHR m_capabilities;
        std::vector<VkSurfaceFormatKHR> m_formats;
        std::vector<VkPresentModeKHR> m_presentModes;
        std::vector<bool> m_queuePresentationSupport;

        std::unique_ptr<QueueFamilyIndices> m_queueFamilyIndices;

        PhysicalDevice const& m_physicalDevice;
        Surface const& m_surface;
    };
}