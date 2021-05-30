#pragma once

#include <vulkan/vulkan.h>
#include <memory>

namespace RHI
{
	class Device;

	// Describe how to access the image and which part to access
	class ImageView
	{
	public:
		ImageView(const std::shared_ptr<Device>& pDevice, const VkImageViewCreateInfo info);
		~ImageView();

		ImageView(const ImageView&) = default;
		ImageView(ImageView&&) = default;
		ImageView& operator=(const ImageView&) = default;
		ImageView& operator=(ImageView&&) = default;

		VkImageView GetDeviceHandle() const { return m_imageView; }
	protected:
		bool Init(const VkImageViewCreateInfo& info);

	private:
		std::shared_ptr<Device> m_device;
		VkImageView m_imageView;
	};
}