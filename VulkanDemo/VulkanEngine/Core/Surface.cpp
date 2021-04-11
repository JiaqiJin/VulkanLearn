#include "Surface.h"
#include "Instance.h"
#include "../Window.h"
#include <GLFW/glfw3.h>

#include <stdexcept>

namespace Rendering
{
    static UniqueHandle<VkSurfaceKHR> createVulkanSurface(Instance const& instance, Window const& window)
    {
        UniqueHandle<VkSurfaceKHR> handle;

        if (glfwCreateWindowSurface(instance.getHandle(), window.getHandle(), nullptr, &handle.get()) != VK_SUCCESS)
            throw std::runtime_error("failed to create window surface!");

        return handle;
    }

    Surface::Surface(const Instance& instance, const Window& window)
        : m_instance(instance), m_window(window)
    {
        m_handle = createVulkanSurface(instance, window);
    }

    Surface::~Surface()
    {
        vkDestroySurfaceKHR(m_instance.getHandle(), m_handle, nullptr);
    }

    int Surface::getWidth() const
    {
        return m_window.getWidth();
    }

    int Surface::getHeight() const
    {
        return m_window.getHeight();
    }
}