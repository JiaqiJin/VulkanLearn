#pragma once

#include <vector>
#include <string>
#include <memory>

namespace Rendering
{
    class ApplicationImpl;

    class Instance;
    class Surface;
    class PhysicalDevice;
    class Device;
    class Window;
    class PhysicalDeviceSurfaceParameters;
    class CommandPool;

    class Application
    {
    public:
        Application(const std::string& name, bool enableValidation, bool enableApiDump, const Window& window);
        ~Application();

        const Instance& getInstance() const;
        const Surface& getSurface() const;
        const Device& getDevice() const;

        const CommandPool& getShortLivedCommandPool() const;

        const PhysicalDeviceSurfaceParameters& getPhysicalDeviceSurfaceParameters() const;
        const PhysicalDevice& getPhysicalDevice() const;

        void onSurfaceChanged();

    private:
        std::unique_ptr<ApplicationImpl> m_impl;
        std::unique_ptr<CommandPool> m_shortLivedCommandPool;
    };
}