#pragma once

namespace Rendering
{
    class Application;

    class Instance;
    class Surface;
    class Device;
    class PhysicalDevice;
    class PhysicalDeviceSurfaceParameters;

    class Object
    {
    public:
        Object(const Application& app) : m_app(app) {}

        Application const& getApp() const { return m_app; }

        const Instance& getInstance() const;
        const Surface& getSurface() const;
        const Device& getDevice() const;
        const PhysicalDevice& getPhysicalDevice() const;
        const PhysicalDeviceSurfaceParameters& getPhysicalDeviceSurfaceParameters() const;

    private:
        const Application& m_app;
    };
}