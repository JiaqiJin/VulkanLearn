#include "Application.h"
#include "Core/Device.h"
#include "Core/Instance.h"
#include "Core/PhysicalDevice.h"
#include "Core/PhysicalDeviceSurface.h"
#include "Core/QueueFamily.h"
#include "Window.h"
#include "Core/Surface.h"

#include <stdexcept>

const std::vector<const char*> DEVICE_EXTENSIONS = {
     VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

bool isDeviceSuitable(const Rendering::PhysicalDevice& physicalDevice, const Rendering::SwapChainSupportDetails& detail)
{
    const auto& parameters = detail;

    bool const areExtensionsSupported = physicalDevice.areExtensionsSupported(DEVICE_EXTENSIONS);

    bool swapchainSupported = false;
    if (areExtensionsSupported)
    {
        swapchainSupported = !parameters.getFormats().empty() && !parameters.getPresentModes().empty();
    }

    return parameters.getQueueFamilyIndices().isValid() && areExtensionsSupported 
        && swapchainSupported && physicalDevice.getFeatures().samplerAnisotropy;
}

std::size_t findSuitablePhysicalDeviceIndex(std::vector<Rendering::PhysicalDevice> const& physicalDevices, 
    const Rendering::SwapChainSupportDetails& detail)
{
    for (std::size_t index = 0; index < physicalDevices.size(); index++)
    {
        const auto& physicalDevice = physicalDevices[index];

        if (isDeviceSuitable(physicalDevice, detail))
            return index;
    }

    throw std::runtime_error("failed to find a suitable GPU!");
}

namespace Rendering
{
    class ApplicationImpl
    {
    public:
        ApplicationImpl(std::string const& name, bool enableValidation, Window const& window);

        const Instance& getInstance() const { return m_instance; }
        const Surface& getSurface() const { return m_surface; }
        const Device& getDevice() const { return m_device; }

        const PhysicalDevice& getPhysicalDevice() const { return m_physicalDevices[m_currentPhysicalDeviceIndex]; }
        const SwapChainSupportDetails& getSwapChainSupportDetails() const { return m_detail; }
        SwapChainSupportDetails& getSwapChainSupportDetails() {  return m_detail; }

    private:
        Instance m_instance;
        Surface m_surface;
        std::vector<Rendering::PhysicalDevice> m_physicalDevices;
        SwapChainSupportDetails m_detail;
        std::size_t m_currentPhysicalDeviceIndex;
        Device m_device;
    };

    ApplicationImpl::ApplicationImpl(std::string const& name, bool enableValidation, Window const& window)
        : m_instance(name, window.getRequiredInstanceExtensions(), enableValidation)
        , m_surface(m_instance, window)
        , m_physicalDevices(m_instance.findPhysicalDevices(m_surface))
        , m_detail(m_physicalDevices.front(), m_surface)
        , m_currentPhysicalDeviceIndex(findSuitablePhysicalDeviceIndex(m_physicalDevices, m_detail))
        , m_device(getPhysicalDevice(), getSwapChainSupportDetails().getQueueFamilyIndices(), DEVICE_EXTENSIONS)
    {
        //printf("creating all");
    }

    // Application
    Application::Application(std::string const& name, bool enableValidation, Window const& window)
    {
        //printf("creating all");
        m_impl = std::make_unique<ApplicationImpl>(name, enableValidation, window);
    }

    Application::~Application() = default;

    Instance const& Application::getInstance() const
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

    const SwapChainSupportDetails& Application::getSwapChainSupportDetails() const
    {
        return m_impl->getSwapChainSupportDetails();
    }

    const PhysicalDevice& Application::getPhysicalDevice() const
    {
        return m_impl->getPhysicalDevice();
    }

    void Application::onSurfaceChanged()
    {
        m_impl->getSwapChainSupportDetails().onSurfaceChanged();
    }
}