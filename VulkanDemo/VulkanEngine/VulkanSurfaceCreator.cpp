#include "VulkanSurfaceCreator.h"

#include <GLFW/glfw3.h>
#include "Core/Instance.h"
#include "Window.h"
#include <stdexcept>

namespace Rendering
{
    UniqueHandle<VkSurfaceKHR> VulkanSurfaceCreator::createVulkanSurface(const Instance& instance, const Window& window)
    {
        UniqueHandle<VkSurfaceKHR> handle;

        if (glfwCreateWindowSurface(instance.getHandle(), window.m_handle, nullptr, &handle.get()) != VK_SUCCESS)
            throw std::runtime_error("failed to create window surface!");

        return handle;
    }
}