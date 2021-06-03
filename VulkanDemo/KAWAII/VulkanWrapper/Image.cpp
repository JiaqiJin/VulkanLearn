#include "Image.h"
#include "GlobalDeviceObjects.h"
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

	bool Image::Init(VkImage img)
	{
		m_image = img;

		return true;
	}

	bool Image::Init(const VkImageCreateInfo& info, uint32_t memoryPropertyFlag)
	{
		VkImageLayout layout = info.initialLayout;
		m_info = info;
		m_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		CHECK_VK_ERROR(vkCreateImage(m_pDevice->GetDeviceHandle(), &m_info, nullptr, &m_image));
		m_pMemKey = DeviceMemMgr()->AllocateImageMemChunk(shared_from_this(), memoryPropertyFlag);

		m_info.initialLayout = layout;
		m_memProperty = memoryPropertyFlag;

		// TODO

		return true;
	}

	bool Image::Init(const GliImageWrapper& gliTex,
		const VkImageCreateInfo& info, uint32_t memoryPropertyFlag)
	{
		return true;
	}

	Image::~Image()
	{
		vkDestroyImage(m_pDevice->GetDeviceHandle(), m_image, nullptr);
	}

	
}