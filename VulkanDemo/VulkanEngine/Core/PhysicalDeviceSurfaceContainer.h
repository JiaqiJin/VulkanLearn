#pragma once

#include "PhysicalDevice.h"
#include "Surface.h"
#include "PhysicalDeviceSurfaceParameters.h"

namespace Rendering
{
    class PhysicalDeviceSurfaceContainer
    {
    public:
        PhysicalDeviceSurfaceContainer(PhysicalDevice&& physicalDdevice, Surface const& surface);

        PhysicalDeviceSurfaceContainer(PhysicalDeviceSurfaceContainer const&) = delete;
        PhysicalDeviceSurfaceContainer(PhysicalDeviceSurfaceContainer&&);
        PhysicalDeviceSurfaceContainer& operator=(PhysicalDeviceSurfaceContainer const&) = delete;
        PhysicalDeviceSurfaceContainer& operator=(PhysicalDeviceSurfaceContainer&&) = delete;

        const PhysicalDevice& getPhysicalDevice() const { return m_physicalDevice; }
        const PhysicalDeviceSurfaceParameters& getParameters() const { return m_parameters; }
        PhysicalDeviceSurfaceParameters& getParameters() { return m_parameters; }

    private:
        PhysicalDevice m_physicalDevice;
        Surface const& m_surface;
        PhysicalDeviceSurfaceParameters m_parameters;
    };
}