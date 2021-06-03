#include "Image.h"
#include "Device.h"

#include "../Common/Macro.h"
#include "../Common/Logger.h"

namespace RHI
{
	Image::Image(const std::shared_ptr<Device>& pDevice)
		: VKGPUSyncRes(pDevice)
	{

	}

	VkMemoryRequirements Image::GetMemoryReqirments() const
	{
		VkMemoryRequirements reqs;
		vkGetImageMemoryRequirements(m_pDevice->GetDeviceHandle(), m_image, &reqs);
		return reqs;
	}

	void Image::BindMemory(VkDeviceMemory memory, uint32_t offset) const
	{
		CHECK_VK_ERROR(vkBindImageMemory(m_pDevice->GetDeviceHandle(), m_image, memory, offset));
	}
}