#pragma once

#include <vulkan/vulkan.h>
#include "UniqueHandle.h"
#include "../Objects/Object.h"
/*
* Wrapper class for VkImageView
* Describe how to access the image and which part of the image to access
*/
namespace Rendering
{
	class Image;

	class ImageView : public Object
	{
	public:
		explicit ImageView(const Application& app, const Image& image, VkImageAspectFlags aspectFlags);
		~ImageView();

		ImageView(const ImageView&) = default;
		ImageView(ImageView&&) = default;
		ImageView& operator=(const ImageView&) = default;
		ImageView& operator=(ImageView&&) = default;

		VkImageView getHandle() const { return m_handle; }

	private:
		UniqueHandle<VkImageView> m_handle;
	};
}