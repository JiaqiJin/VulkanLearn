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
	struct SwapChainSupportDetails;

	class Swapchain
	{
	public:
		Swapchain(const Device& device, const SwapChainSupportDetails& support);
		~Swapchain();

		VkSwapchainKHR getHandle() const { return m_handle; }
		VkExtent2D getExtend() { return m_extent; }
		VkSurfaceFormatKHR getSurfaceFormat() { return m_surfaceFormat; }

	private:
		void createSwapchain();

	private:
		UniqueHandle<VkSwapchainKHR> m_handle;

		VkExtent2D m_extent;
		VkSurfaceFormatKHR m_surfaceFormat;

		const Device& m_device;
		const SwapChainSupportDetails& m_detail;
	};
}