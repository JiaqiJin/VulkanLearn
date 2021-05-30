#include "ImageView.h"
#include "Device.h"

#include "../Common/Macro.h"
#include "../Common/Logger.h"

namespace RHI
{
	ImageView::ImageView(const std::shared_ptr<Device>& pDevice, const VkImageViewCreateInfo info)
		: m_device(pDevice)
	{
		if (!Init(info))
			K_ERROR("Error Initialize ImageView");
	}

	bool ImageView::Init(const VkImageViewCreateInfo& info)
	{
		RETURN_FALSE_VK_RESULT(vkCreateImageView(m_device->GetDeviceHandle(), &info, nullptr, &m_imageView));

		return true;
	}

	ImageView::~ImageView()
	{
		if (m_imageView)
			vkDestroyImageView(m_device->GetDeviceHandle(), m_imageView, nullptr);
	}

}