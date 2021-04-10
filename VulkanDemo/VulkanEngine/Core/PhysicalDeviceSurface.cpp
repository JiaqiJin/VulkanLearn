#include "PhysicalDeviceSurface.h"
#include "PhysicalDevice.h"
#include "QueueFamily.h"
#include "Surface.h"

namespace Rendering
{
    PhysicalDeviceSurfaceParameters::PhysicalDeviceSurfaceParameters(PhysicalDevice& physicalDevice, Surface& surface) 
        : m_physicalDevice(physicalDevice), m_surface(surface)
    {
        
    }

	PhysicalDeviceSurfaceParameters::~PhysicalDeviceSurfaceParameters() = default;

	PhysicalDeviceSurfaceParameters::PhysicalDeviceSurfaceParameters(PhysicalDeviceSurfaceParameters&&) = default;

    bool PhysicalDeviceSurfaceParameters::isPresentationSupported(QueueFamily const& queueFamily) const
    {
        uint32_t index = queueFamily.getIndex();

        if (index >= m_queuePresentationSupport.size())
            return false;

        return m_queuePresentationSupport[index];
    }

    void PhysicalDeviceSurfaceParameters::onSurfaceChanged()
    {
        queryCapabilities();
    }

    void PhysicalDeviceSurfaceParameters::queryCapabilities()
    {
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice.getPhysicalDevice(), m_surface.getSurface(), &m_capabilities);
    }

    void PhysicalDeviceSurfaceParameters::queryFormats()
    {
        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice.getPhysicalDevice(), m_surface.getSurface(),&formatCount, nullptr);

        if (formatCount != 0)
        {
            m_formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice.getPhysicalDevice(), m_surface.getSurface(), &formatCount, m_formats.data());
        }
    }

    void PhysicalDeviceSurfaceParameters::queryPresentModes()
    {
        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice.getPhysicalDevice(), m_surface.getSurface(), &presentModeCount, nullptr);

        if (presentModeCount != 0) 
        {
            m_presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice.getPhysicalDevice(), m_surface.getSurface(), 
                &presentModeCount, m_presentModes.data());
        }
    }

    void PhysicalDeviceSurfaceParameters::queryPresentationSupport()
    {
        std::vector<QueueFamily> const& queueFamilies = m_physicalDevice.getQueueFamilies();

        m_queuePresentationSupport.resize(queueFamilies.size());

        for (QueueFamily const& queueFamily : queueFamilies)
        {
            uint32_t index = queueFamily.getIndex();

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice.getPhysicalDevice(), index, m_surface.getSurface(), &presentSupport);

            if (index >= m_queuePresentationSupport.size())
                m_queuePresentationSupport.resize(index + 1);

            m_queuePresentationSupport[index] = presentSupport > 0;
        }
    }
}