#pragma once

#include "Vulkan.h"

namespace Rendering
{
	class Instance;

	class Surface
	{
	public:
		NON_COPIABLE(Surface);

		explicit Surface(const Instance& instance);
		~Surface();

		VkSurfaceKHR Get() const
		{
			return surface;
		};

		const Instance& GetInstance() const
		{
			return instance;
		}

	private:
		VkSurfaceKHR surface{};
		const Instance& instance;
	};
}