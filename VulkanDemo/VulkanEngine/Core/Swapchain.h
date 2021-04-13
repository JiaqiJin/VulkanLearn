#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <memory>
#include "UniqueHandle.h"

/*
* Wrapper class for VkSwapchainKHR
* 
*/
namespace Rendering
{
	class Device;
	class Surface;

	class Swapchain
	{
	public:
		Swapchain(const Device& device, const Surface& surface);
		~Swapchain();

		VkSwapchainKHR getHandle() const { return m_handle; }
		const Device& getDevice() const { return m_device; }
		const Surface& getSurface() const { return m_surface; }

	private:
		void createSwapchain();

	private:
		UniqueHandle<VkSwapchainKHR> m_handle;

		const Device& m_device;
		const Surface& m_surface;
	};
}