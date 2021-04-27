#include "Application.h"
#include "Core/Instance.h"
#include "Core/Surface.h"
#include "Core/PhysicalDevice.h"
#include "Core/Device.h"
#include "Core/PhysicalDeviceSurfaceContainer.h"
#include "Core/PhysicalDeviceSurfaceParameters.h"
#include "Core/QueueFamilyIndices.h"
#include "Core/CommandPool.h"
#include "Window.h"
#include <stdexcept>

#include <stdexcept>
#include <iostream>

const std::vector<const char*> DEVICE_EXTENSIONS = {
       VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

bool isDeviceSuitable(const Rendering::PhysicalDeviceSurfaceContainer& container)
{
    auto const& physicalDevice = container.getPhysicalDevice();
    auto const& parameters = container.getParameters();

    bool const areExtensionsSupported = physicalDevice.areExtensionsSupported(DEVICE_EXTENSIONS);

    bool swapchainSupported = false;
    if (areExtensionsSupported)
    {
        swapchainSupported = !parameters.getFormats().empty() && !parameters.getPresentModes().empty();
    }

    return parameters.getQueueFamilyIndices().isValid() && areExtensionsSupported && swapchainSupported && physicalDevice.getFeatures().samplerAnisotropy;
}

std::size_t findSuitablePhysicalDeviceIndex(std::vector<Rendering::PhysicalDeviceSurfaceContainer> const& physicalDevices)
{
    for (std::size_t index = 0; index < physicalDevices.size(); index++)
    {
        auto const& physicalDevice = physicalDevices[index];

        if (isDeviceSuitable(physicalDevice))
            return index;
    }

    throw std::runtime_error("failed to find a suitable GPU!");
}

namespace Rendering
{
    class ApplicationImpl
    {
    public:
        ApplicationImpl(std::string const& name, bool enableValidation, bool enableApiDump, Window const& window);

        const Instance& getInstance() const { return m_instance; }
        const Surface& getSurface() const { return m_surface; }
        const Device& getDevice() const { return m_device; }

        const PhysicalDeviceSurfaceContainer& getPhysicalDeviceSurfaceContainer() const { return m_physicalDevices[m_currentPhysicalDeviceIndex]; }
        PhysicalDeviceSurfaceContainer& getPhysicalDeviceSurfaceContainer() { return m_physicalDevices[m_currentPhysicalDeviceIndex]; }
        const PhysicalDevice& getPhysicalDevice() const { return getPhysicalDeviceSurfaceContainer().getPhysicalDevice(); }
        const PhysicalDeviceSurfaceParameters& getPhysicalDeviceSurfaceParameters() const { return getPhysicalDeviceSurfaceContainer().getParameters(); }
        PhysicalDeviceSurfaceParameters& getPhysicalDeviceSurfaceParameters() { return getPhysicalDeviceSurfaceContainer().getParameters(); }

    private:
        Instance m_instance;
        Surface m_surface;
        std::vector<PhysicalDeviceSurfaceContainer> m_physicalDevices;
        std::size_t m_currentPhysicalDeviceIndex;
        Device m_device;
    };

    ApplicationImpl::ApplicationImpl(const std::string& name, bool enableValidation, bool enableApiDump, const Window& window)
        : m_instance(name, window.getRequiredInstanceExtensions(), enableValidation, enableApiDump)
        , m_surface(m_instance, window)
        , m_physicalDevices(m_instance.findPhysicalDevices(m_surface))
        , m_currentPhysicalDeviceIndex(findSuitablePhysicalDeviceIndex(m_physicalDevices))
        , m_device(getPhysicalDeviceSurfaceContainer(), DEVICE_EXTENSIONS)
    {

    }

    Application::Application(std::string const& name, bool enableValidation, bool enableApiDump, Window const& window)
    {
        m_impl = std::make_unique<ApplicationImpl>(name, enableValidation, enableApiDump, window);

        m_shortLivedCommandPool = std::make_unique<CommandPool>(*this);
    }

    Application::~Application() = default;

    const Instance& Application::getInstance() const
    {
        return m_impl->getInstance();
    }

    const Surface& Application::getSurface() const
    {
        return m_impl->getSurface();
    }

    const Device& Application::getDevice() const
    {
        return m_impl->getDevice();
    }

    const CommandPool& Application::getShortLivedCommandPool() const
    {
        return *m_shortLivedCommandPool;
    }

    const PhysicalDeviceSurfaceParameters& Application::getPhysicalDeviceSurfaceParameters() const
    {
        return m_impl->getPhysicalDeviceSurfaceParameters();
    }

    const PhysicalDevice& Application::getPhysicalDevice() const
    {
        return m_impl->getPhysicalDevice();
    }

    void Application::onSurfaceChanged()
    {
        m_impl->getPhysicalDeviceSurfaceParameters().onSurfaceChanged();
    }

}

  