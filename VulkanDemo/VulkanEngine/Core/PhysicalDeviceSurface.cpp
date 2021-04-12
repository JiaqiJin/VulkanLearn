#include "PhysicalDeviceSurface.h"
#include "PhysicalDevice.h"
#include "Surface.h"
#include "QueueFamily.h"

namespace Rendering
{
    SwapChainSupportDetails::SwapChainSupportDetails(const PhysicalDevice& physicalDevice, const Surface& surface)
    {
        queryCapabilities(physicalDevice, surface);
        queryFormats(physicalDevice, surface);
        queryPresentModes(physicalDevice, surface);
        queryPresentationSupport(physicalDevice, surface);

        m_queueFamilyIndices = std::make_unique<QueueFamilyIndices>(physicalDevice, *this);
    }

    bool Rendering::SwapChainSupportDetails::isPresentationSupported(const QueueFamily& queueFamily) const
    {
        uint32_t index = queueFamily.getIndex();

        if (index >= m_queuePresentationSupport.size())
            return false;

        return m_queuePresentationSupport[index];
    }

    void SwapChainSupportDetails::onSurfaceChanged(const PhysicalDevice& physicalDevice, const Surface& surface)
    {
        queryCapabilities(physicalDevice, surface);
    }

    void SwapChainSupportDetails::queryCapabilities(const PhysicalDevice& physicalDevice, const Surface& surface)
    {
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice.getHandle(), surface.getHandle(), &m_capabilities);
    }

    void SwapChainSupportDetails::queryFormats(const PhysicalDevice& physicalDevice, const Surface& surface)
    {
        uint32_t count;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice.getHandle(), surface.getHandle(), &count, nullptr);

        if (count > 0)
        {
            m_formats.resize(count);
            vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice.getHandle(), surface.getHandle(), &count, m_formats.data());
        }
    }

    void SwapChainSupportDetails::queryPresentModes(const PhysicalDevice& physicalDevice, const Surface& surface)
    {
        uint32_t count;
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice.getHandle(), surface.getHandle(), &count, nullptr);

        if (count > 0)
        {
            m_presentModes.resize(count);
            vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice.getHandle(), surface.getHandle(), &count, m_presentModes.data());
        }
    }

    void SwapChainSupportDetails::queryPresentationSupport(const PhysicalDevice& physicalDevice, const Surface& surface)
    {
        const std::vector<QueueFamily>& queueFamilies = physicalDevice.getQueueFamilies();

        m_queuePresentationSupport.resize(queueFamilies.size());

        for (const QueueFamily& queueFamily : queueFamilies)
        {
            uint32_t index = queueFamily.getIndex();

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice.getHandle(), index, surface.getHandle(), &presentSupport);

            if (index >= m_queuePresentationSupport.size())
                m_queuePresentationSupport.resize(index + 1);

            m_queuePresentationSupport[index] = presentSupport > 0;
        }
    }

    //// PhysicalDeviceSurfaceContainer
    //PhysicalDeviceSurfaceContainer::PhysicalDeviceSurfaceContainer(PhysicalDevice&& physicalDdevice, const Surface& surface)
    //    : m_physicalDevice(std::move(physicalDdevice))
    //    , m_surface(surface)
    //    , m_parameters(m_physicalDevice, m_surface)
    //{

    //}

}