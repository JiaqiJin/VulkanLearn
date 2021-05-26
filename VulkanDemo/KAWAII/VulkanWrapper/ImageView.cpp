#include "ImageView.h"
#include "Device.h"

namespace RHI
{
	ImageView::ImageView(const std::shared_ptr<Device> pDevice, const VkImageViewCreateInfo info)
	{
		std::shared_ptr<ImageView> pImageView = std::make_shared<ImageView>(pDevice, info);
		if (!Init(pDevice, pImageView, info))
			K_ERROR("Error Initialize ImageView");
	}

	bool ImageView::Init(const std::shared_ptr<Device>& pDevice, const std::shared_ptr<ImageView>& pSelf, const VkImageViewCreateInfo& info)
	{
		if (!DeviceObjectBase::Init(pDevice, pSelf))
			return false;

		RETURN_FALSE_VK_RESULT(vkCreateImageView(m_pDevice->GetDeviceHandle(), &info, nullptr, &m_imageView));

		return true;
	}

	ImageView::~ImageView()
	{
		if (m_imageView)
			vkDestroyImageView(GetDevice()->GetDeviceHandle(), m_imageView, nullptr);
	}

}