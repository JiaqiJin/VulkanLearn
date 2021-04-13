#include "SwapChainSupport.h"
#include "PhysicalDevice.h"
#include "Surface.h"
#include "QueueFamily.h"

namespace Rendering
{
    SwapChainSupportDetails::SwapChainSupportDetails(const PhysicalDevice& physicalDevice, const Surface& surface)
        : m_physicalDevice(physicalDevice),
        m_surface(surface)
    {
        queryCapabilities();
        queryFormats();
        queryPresentModes();
        queryPresentationSupport();

        m_queueFamilyIndices = std::make_unique<QueueFamilyIndices>(physicalDevice, *this);
    }

    bool Rendering::SwapChainSupportDetails::isPresentationSupported(const QueueFamily& queueFamily) const
    {
        uint32_t index = queueFamily.getIndex();

        if (index >= m_queuePresentationSupport.size())
            return false;

        return m_queuePresentationSupport[index];
    }

    void SwapChainSupportDetails::onSurfaceChanged()
    {
        queryCapabilities();
    }

    void SwapChainSupportDetails::queryCapabilities()
    {
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice.getHandle(), m_surface.getHandle(), &m_capabilities);
    }

    void SwapChainSupportDetails::queryFormats()
    {
        uint32_t count;
        vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice.getHandle(), m_surface.getHandle(), &count, nullptr);

        if (count > 0)
        {
            m_formats.resize(count);
            vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice.getHandle(), m_surface.getHandle(), &count, m_formats.data());
        }
    }

    void SwapChainSupportDetails::queryPresentModes()
    {
        uint32_t count;
        vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice.getHandle(), m_surface.getHandle(), &count, nullptr);

        if (count > 0)
        {
            m_presentModes.resize(count);
            vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice.getHandle(), m_surface.getHandle(), &count, m_presentModes.data());
        }
    }

    void SwapChainSupportDetails::queryPresentationSupport()
    {
        const std::vector<QueueFamily>& queueFamilies = m_physicalDevice.getQueueFamilies();

        m_queuePresentationSupport.resize(queueFamilies.size());

        for (const QueueFamily& queueFamily : queueFamilies)
        {
            uint32_t index = queueFamily.getIndex();

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice.getHandle(), index, m_surface.getHandle(), &presentSupport);

            if (index >= m_queuePresentationSupport.size())
                m_queuePresentationSupport.resize(index + 1);

            m_queuePresentationSupport[index] = presentSupport > 0;
        }
    }
}