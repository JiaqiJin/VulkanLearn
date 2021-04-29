#pragma once

#include "Vulkan.h"

#include <vector>
#include <memory>

namespace Rendering
{
	struct SwapChainSupportDetails 
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	class Device;
	class Surface;
	class ImageView;

	class SwapChain
	{
	public:
		NON_COPIABLE(SwapChain);

		SwapChain(const Device& device);
		~SwapChain();

	private:
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device) const;
		static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		static VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;
		static uint32_t ChooseImageCount(const VkSurfaceCapabilitiesKHR& capabilities);

	private:
		VkSwapchainKHR m_swapChain;
		const Device& m_device;
		const Surface& m_surface;
		std::vector<VkImage> images;
		std::vector<std::unique_ptr<ImageView>> imageViews;
	public:
		VkPresentModeKHR PresentMode{};
		VkFormat Format{};
		VkExtent2D Extent{};
		uint32_t MinImageCount{};
	};
}