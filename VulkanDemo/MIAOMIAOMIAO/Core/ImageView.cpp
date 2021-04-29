#include "ImageView.h"
#include "Device.h"

namespace Rendering
{
	ImageView::ImageView(const class Device& device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
		: m_device(device), m_format(format), m_image(image)
	{
		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = image;
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = format;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = aspectFlags;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		VK_CHECK(vkCreateImageView(m_device.Get(), &createInfo, nullptr, &m_imageView), "Create image view");
	}

	ImageView::~ImageView()
	{
		if (m_imageView != nullptr)
		{
			vkDestroyImageView(m_device.Get(), m_imageView, nullptr);
			m_imageView = nullptr;
		}
	}
}