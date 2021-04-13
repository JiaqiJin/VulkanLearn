#include "Swapchain.h"
#include "Device.h"
#include "Surface.h"
#include "QueueFamily.h"
#include "SwapChainSupport.h"

#include <algorithm>
#include <stdexcept>
#include <iostream>

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

	Swapchain::Swapchain(const Device& device, const SwapChainSupportDetails& support)
		: m_device(device), 
		m_detail(support)
	{
		createSwapchain();
	}
	
	Swapchain::~Swapchain()
	{
		vkDestroySwapchainKHR(m_device.getHandle() , m_handle, nullptr);
	}

	void Swapchain::createSwapchain()
	{
		m_surfaceFormat = chooseSwapSurfaceFormat(m_detail.getFormats());
		VkPresentModeKHR presentMode = chooseSwapPresentMode(m_detail.getPresentModes());
		m_extent = chooseSwapExtent(m_detail.getSurface(), m_detail.getCapabilities());

		// how many img have in swap chain
		const uint32_t minImageCount = m_detail.getCapabilities().minImageCount;
		const uint32_t maxImageCount = m_detail.getCapabilities().maxImageCount;

		uint32_t imageCount = minImageCount + 1;

		if(maxImageCount > 0)
			imageCount = std::min(imageCount, maxImageCount);

		VkSwapchainCreateInfoKHR swapchainCreateInfo{};
		swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainCreateInfo.surface = m_detail.getSurface().getHandle();

		swapchainCreateInfo.minImageCount = imageCount;
		swapchainCreateInfo.imageFormat = m_surfaceFormat.format;
		swapchainCreateInfo.imageColorSpace = m_surfaceFormat.colorSpace;
		swapchainCreateInfo.imageExtent = m_extent;
		swapchainCreateInfo.imageArrayLayers = 1;
		swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		// handle swap chain images to multiple queue families
		const QueueFamilyIndices& indices = m_detail.getQueueFamilyIndices();
		int32_t graphicsQueueFamilyIndex = indices.getGraphicsQueueFamily().getIndex();
		uint32_t presentQueueFamilyIndex = indices.getPresentQueueFamily().getIndex();

		if(graphicsQueueFamilyIndex != presentQueueFamilyIndex)
		{
			uint32_t queueFamilyIndices[] = { graphicsQueueFamilyIndex, presentQueueFamilyIndex };
			swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT; // Images can be used across multiple queue families
			swapchainCreateInfo.queueFamilyIndexCount = 2;
			swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else
		{
			swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE; // An image is owned by one queue family 
			swapchainCreateInfo.queueFamilyIndexCount = 0;
			swapchainCreateInfo.pQueueFamilyIndices = nullptr;
		}

		swapchainCreateInfo.preTransform = m_detail.getCapabilities().currentTransform;
		swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; // alpha channel 
		swapchainCreateInfo.presentMode = presentMode;
		swapchainCreateInfo.clipped = VK_TRUE;
		swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

		if (vkCreateSwapchainKHR(m_device.getHandle(), &swapchainCreateInfo, nullptr, &m_handle.get()) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create swap chain!");
		}
	}
}