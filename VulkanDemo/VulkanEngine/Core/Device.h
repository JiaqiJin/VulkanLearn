#pragma once

#include <vulkan/vulkan.h>
#include <vector>

#include "UniqueHandle.h"

namespace Rendering
{
	class Queue;

	class Device
	{
	public:
		explicit Device();
		~Device();

		Device(Device const&) = default;
		Device(Device&&) = default;
		Device& operator=(Device const&) = default;
		Device& operator=(Device&&) = default;

		VkDevice getDevice() const { return m_device; }

	private:
		UniqueHandle<VkDevice> m_device = VK_NULL_HANDLE;
	};
}