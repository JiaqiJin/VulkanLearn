#pragma once

#include "DeviceObjectBase.h"

namespace RHI
{
	// Describe how to access the image and which part to access
	class ImageView : public DeviceObjectBase<ImageView>
	{
	public:
		ImageView(const std::shared_ptr<Device> pDevice, const VkImageViewCreateInfo info);
		~ImageView();

		ImageView(const ImageView&) = default;
		ImageView(ImageView&&) = default;
		ImageView& operator=(const ImageView&) = default;
		ImageView& operator=(ImageView&&) = default;

		VkImageView GetDeviceHandle() const { return m_imageView; }
	protected:
		bool Init(const std::shared_ptr<Device>& pDevice, const std::shared_ptr<ImageView>& pSelf, const VkImageViewCreateInfo& info);

	private:
		VkImageView m_imageView;
	};
}