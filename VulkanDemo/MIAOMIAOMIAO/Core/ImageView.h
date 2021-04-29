#pragma once

#include "Vulkan.h"

namespace Rendering
{
	class Device;

	class ImageView
	{
	public:
		NON_COPIABLE(ImageView);

		ImageView(const class Device& device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT);
		~ImageView();

		VkImageView Get() const
		{
			return m_imageView;
		}

		const Device& GetDevice() const
		{
			return m_device;
		}

		VkFormat GetFormat() const
		{
			return m_format;
		}

		VkImage GetImage() const
		{
			return m_image;
		}

	private:
		VkImageView m_imageView;
		const Device& m_device;
		const VkImage m_image;
		const VkFormat m_format;
	};
}