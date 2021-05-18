#pragma once

#include "Vulkan.h"

namespace Rendering
{
	class Device;

	class Semaphore final
	{
	public:
		NON_COPIABLE(Semaphore)

		explicit Semaphore(const Device& device);
		~Semaphore();

		VkSemaphore Get() const
		{
			return semaphore;
		}

	private:
		const Device& device;
		VkSemaphore semaphore;
	};
}