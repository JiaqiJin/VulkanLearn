#include "Surface.h"
#include "Instance.h"
#include "../Window.h"
#include <stdexcept>
#include <GLFW/glfw3.h>

namespace Rendering
{
	UniqueHandle<VkSurfaceKHR> createVulkanSurface(Instance const& instance, Window const& window)
	{
		UniqueHandle<VkSurfaceKHR> surface;

		if (glfwCreateWindowSurface(instance.getInstance(), window.getHandle(), nullptr, &surface.get()) != VK_SUCCESS)
			throw std::runtime_error("failed to create window surface!");

		return surface;
	}

	Surface::Surface(Instance& instance, Window& window) : m_instance(instance), m_window(window)
	{
		m_surface = createVulkanSurface(instance, window);
	}

	Surface::~Surface()
	{
		vkDestroySurfaceKHR(m_instance.getInstance(), m_surface, nullptr);
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