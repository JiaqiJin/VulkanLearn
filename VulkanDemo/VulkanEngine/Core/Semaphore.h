#pragma once

#include <vulkan/vulkan.h>
#include "UniqueHandle.h"

namespace Rendering
{
	class Device;

	class Semaphore
	{
	public:
		Semaphore(const Device& device);
		~Semaphore();

		Semaphore(const Semaphore&) = default;
		Semaphore(Semaphore&&) = default;
		Semaphore& operator=(const Semaphore&) = default;
		Semaphore& operator=(Semaphore&&) = default;

		VkSemaphore getHandle() const { return m_handle; }

	private:
		UniqueHandle<VkSemaphore> m_handle;

		const Device& m_device;
	};
}