#pragma once

#include <vulkan/vulkan.h>
#include "UniqueHandle.h"

/*
* Wrapper class for VkImageView
* Describe how to access the image and which part of the image to access
*/
namespace Rendering
{
	class Image;
	class Device;

	class ImageView
	{
	public:
		ImageView(const Device& device, const Image& image, VkImageAspectFlags aspectFlags);
		~ImageView();

		ImageView(const ImageView&) = default;
		ImageView(ImageView&&) = default;
		ImageView& operator=(const ImageView&) = default;
		ImageView& operator=(ImageView&&) = default;

		VkImageView getHandle() const { return m_handle; }
	private:
		const Device& m_device;
		UniqueHandle<VkImageView> m_handle;
	};
}