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

		VkSwapchainKHR Get() const
		{
			return m_swapChain;
		};

		const std::vector<VkImage>& GetImage() const
		{
			return images;
		}

		const std::vector<std::unique_ptr<ImageView>>& GetImageViews() const
		{
			return m_imageViews;
		}

	private:
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device) const;
		static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		static VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;
		static uint32_t ChooseImageCount(const VkSurfaceCapabilitiesKHR& capabilities);

		void CreateImageView();

	private:
		VkSwapchainKHR m_swapChain;
		const Device& m_device;
		const Surface& m_surface;
		std::vector<VkImage> images;
		std::vector<std::unique_ptr<ImageView>> m_imageViews;

	public:
		VkPresentModeKHR PresentMode{};
		VkFormat Format{};
		VkExtent2D Extent{};
		uint32_t MinImageCount{};
	};
}