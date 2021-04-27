#include "Surface.h"
#include "Instance.h"
#include "../Window.h"
#include <GLFW/glfw3.h>
#include "../VulkanSurfaceCreator.h"
#include <stdexcept>

namespace Rendering
{
    Surface::Surface(const Instance& instance, const Window& window) : m_instance(instance), m_window(window)
    {
        m_handle = VulkanSurfaceCreator::createVulkanSurface(instance, window);
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