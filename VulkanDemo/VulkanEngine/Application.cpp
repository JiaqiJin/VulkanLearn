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

bool isDeviceSuitable(const Rendering::PhysicalDevice& device, const Rendering::SwapChainSupportDetails& detail)
{
    
    bool const areExtensionsSupported = device.areExtensionsSupported(DEVICE_EXTENSIONS);

    bool swapchainSupported = false;
    if (areExtensionsSupported)
    {
        swapchainSupported = !detail.getFormats().empty() && !detail.getPresentModes().empty();
    }

    return detail.getQueueFamilyIndices().isValid() && areExtensionsSupported && swapchainSupported && device.getFeatures().samplerAnisotropy;
}

std::size_t findSuitablePhysicalDeviceIndex(std::vector<Rendering::PhysicalDevice> const& physicalDevices, const Rendering::SwapChainSupportDetails& detail)
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

        const SwapChainSupportDetails& getSupportDetail() const { return detail; }
        SwapChainSupportDetails& getSupportDetail() { return detail; }
        const PhysicalDevice& getPhysicalDevice() const { return m_physicalDevices[m_currentPhysicalDeviceIndex]; }
       
    private:
        Instance m_instance;
        Surface m_surface;
        std::vector<Rendering::PhysicalDevice> m_physicalDevices;
        uint32_t m_currentPhysicalDeviceIndex;
        SwapChainSupportDetails detail;
        Device m_device;
    };

    ApplicationImpl::ApplicationImpl(std::string const& name, bool enableValidation, Window const& window)
        : m_instance(name, window.getRequiredInstanceExtensions(), enableValidation)
        , m_surface(m_instance, window)
        , m_physicalDevices(m_instance.findPhysicalDevices(m_surface))
        , detail(m_physicalDevices.front(), m_surface)
        , m_currentPhysicalDeviceIndex(findSuitablePhysicalDeviceIndex(m_physicalDevices, detail))
        , m_device(detail,m_physicalDevices[m_currentPhysicalDeviceIndex], m_surface, DEVICE_EXTENSIONS)
    {
        printf("creating all");
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

    const SwapChainSupportDetails& Application::getDetail() const
    {
        return m_impl->getSupportDetail();
    }

    const PhysicalDevice& Application::getPhysicalDevice() const
    {
        return m_impl->getPhysicalDevice();
    }

    void Application::onSurfaceChanged()
    {
        m_impl->getSupportDetail().onSurfaceChanged(getPhysicalDevice(), getSurface());
    }
}
