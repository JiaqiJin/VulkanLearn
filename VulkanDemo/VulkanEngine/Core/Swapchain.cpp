#include "Swapchain.h"
#include "Device.h"
#include "Surface.h"
#include "QueueFamily.h"

namespace Rendering
{
	// Helper function for choosing right setting for swap chain (Surface format, presnetation Mode and Swap exten)
	/*
	* Surface format (color depth)
	* Presentation mode (conditions for "swapping" images to the screen)
	* Swap extent (resolution of images in swap chain)
	*/
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
	{
		if(availableFormats.empty())
			return { VK_FORMAT_UNDEFINED , VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };

		for(const auto& availableFormat : availableFormats) 
		{
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
				return availableFormat;
		}

		return availableFormats[0];
	}

	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
	{
		for (const auto& availablePresentMode : availablePresentModes)
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
				return availablePresentMode;

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D chooseSwapExtent(const Surface& surface, const VkSurfaceCapabilitiesKHR& capabilities)
	{
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
			return capabilities.currentExtent;

		auto width = static_cast<uint32_t>(surface.getWidth());
		auto height = static_cast<uint32_t>(surface.getHeight());

		width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, width));
		height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, height));

		VkExtent2D actualExtend = { width, height };

		return actualExtend;
	}

	Swapchain::Swapchain(const Device& device, const Surface& surface)
		: m_device(device),
		m_surface(surface)
	{
		createSwapchain();
	}
	
	Swapchain::~Swapchain()
	{
		vkDestroySwapchainKHR(m_device.getHandle() , m_handle, nullptr);
	}

	void Swapchain::createSwapchain()
	{

	}
}