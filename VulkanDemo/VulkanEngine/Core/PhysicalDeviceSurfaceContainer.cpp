#include "PhysicalDeviceSurfaceContainer.h"

namespace Rendering
{
    PhysicalDeviceSurfaceContainer::PhysicalDeviceSurfaceContainer(PhysicalDevice&& physicalDdevice, Surface const& surface)
        : m_physicalDevice(std::move(physicalDdevice))
        , m_surface(surface)
        , m_parameters(m_physicalDevice, m_surface)
    {

    }

    PhysicalDeviceSurfaceContainer::PhysicalDeviceSurfaceContainer(PhysicalDeviceSurfaceContainer&& rhs) = default;
}