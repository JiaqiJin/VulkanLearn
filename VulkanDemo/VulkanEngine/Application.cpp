#include "Application.h"
#include "Core/Device.h"
#include "Core/Instance.h"
#include "Core/PhysicalDevice.h"
#include "Core/SwapChainSupport.h"
#include "Core/QueueFamily.h"
#include "Core/Swapchain.h"
#include "Window.h"
#include "Core/Surface.h"
#include "Core/CommandPool.h"

#include <stdexcept>
#include <iostream>

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
        //std::shared_ptr<Shader> m_shader;
    };

    ApplicationImpl::ApplicationImpl(std::string const& name, bool enableValidation, Window const& window)
        : m_instance(name, window.getRequiredInstanceExtensions(), enableValidation)
        , m_surface(m_instance, window)
        , m_physicalDevices(m_instance.findPhysicalDevices(m_surface))
        , m_detail(m_physicalDevices.front(), m_surface)
        , m_currentPhysicalDeviceIndex(findSuitablePhysicalDeviceIndex(m_physicalDevices, m_detail))
        , m_device(getPhysicalDevice(), getSwapChainSupportDetails().getQueueFamilyIndices(), getInstance(),DEVICE_EXTENSIONS)
    {
        printf("creating all");
        //printf(m_physicalDevices[m_currentPhysicalDeviceIndex].getProperties().deviceName);
        //std::cout << static_cast<int>(m_swapChain.getExtend().height) << std::endl;
       /* m_shader = ShaderBuilder().addShader(ShaderType::Vertex, "Shaders/vert.spv", "main").
            addShader(ShaderType::Fragment, "Shaders/frag.spv", "main").buildShader(m_device);
        m_shader->createShaderStageCreateInfo();*/
    }

    // Application
    Application::Application(std::string const& name, bool enableValidation, Window const& window)
    {
        //printf("creating all");
        m_impl = std::make_unique<ApplicationImpl>(name, enableValidation, window);

        m_shortLivedCommandPool = std::make_unique<CommandPool>(getDevice(), getSwapChainSupportDetails());
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

    const CommandPool& Application::getShortLivedCommandPool() const
    {
        return *m_shortLivedCommandPool;
    }

    void Application::onSurfaceChanged()
    {
        m_impl->getSwapChainSupportDetails().onSurfaceChanged();
    }
}