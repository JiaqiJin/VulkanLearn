#pragma once

#include <vulkan/vulkan.h>
#include "UniqueHandle.h"

namespace Rendering
{
	class Instance;
	class Window;

	class Surface
	{
	public:
		explicit Surface(Instance& instance, Window& window);
		~Surface();

		Surface(Surface const&) = default;
		Surface(Surface&&) = default;
		Surface& operator=(Surface const&) = default;
		Surface& operator=(Surface&&) = default;

		VkSurfaceKHR getSurface() const { return m_surface; }

		int getWidth() const;
		int getHeight() const;

	private:
		UniqueHandle<VkSurfaceKHR> m_surface;

		Instance& m_instance;
		Window& m_window;
	};
}