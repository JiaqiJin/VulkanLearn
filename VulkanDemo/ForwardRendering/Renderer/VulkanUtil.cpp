#include "VulkanUtil.h"

#include "context.h"

VUtility::VUtility(const VContext& context)
	: context(&context),
	graphicsDevice(context.getDevice()),
	device(graphicsDevice),
	physicalDevice(context.getPhysicalDevice()),
	graphicsQueue(context.getGraphicsQueue()),
	presentQueue(context.getPresentQueue())
{	

}

VkSurfaceFormatKHR VUtility::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED)
	{
		return{ VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
	}

	for (const auto& availableFormat : availableFormats)
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return availableFormat;
		}
	}

	return availableFormats[0];
}

VkPresentModeKHR VUtility::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) 
{
	for (const auto& availablePresentMode : availablePresentModes)
	{
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VUtility::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	if (capabilities.currentExtent.width != UINT32_MAX) 
	{
		return capabilities.currentExtent;
	}
	else {
		int width, height;
		std::tie(width, height) = context->getWindowFrameBufferSize();

		VkExtent2D actualExtent = {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

		return actualExtent;
	}
}

void VUtility::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectMask, VkImageView* pImageView)
{
	VkImageViewCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	createInfo.image = image;
	createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	createInfo.format = format;
	// no swizzle
	createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
	// subresource =  image's purpose
	createInfo.subresourceRange.aspectMask = aspectMask;
	createInfo.subresourceRange.baseMipLevel = 0;
	createInfo.subresourceRange.levelCount = 1;
	createInfo.subresourceRange.baseArrayLayer = 0;
	createInfo.subresourceRange.layerCount = 1;

	if (vkCreateImageView(graphicsDevice, &createInfo, nullptr, pImageView) != VK_SUCCESS) {
		throw std::runtime_error("failed to create image view!");
	}
}

VRaii<VkImageView> VUtility::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectMask)
{
	VkImageView imageView;
	createImageView(image, format, aspectMask, &imageView);
	return VRaii<VkImageView>(
		imageView, [device = this->device](auto& obj) {
		device.destroyImageView(obj); 
		}
	);
}