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
		// Memory Allocation
		m_pMemKey = DeviceMemMgr()->AllocateImageMemChunk(this, memoryPropertyFlag);

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

	void Image::CreateEmptyTexture(const Vector3ui& size,
		uint32_t mipLevels,
		uint32_t layers,
		VkFormat format,
		VkImageLayout defaultLayout,
		VkImageUsageFlags usage,
		VkPipelineStageFlags stageFlag,
		VkAccessFlags accessFlag,
		VkImageViewCreateFlags createFlag)
	{
		// TODO

		VkImageCreateInfo textureCreateInfo = {};
		textureCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		textureCreateInfo.flags = createFlag;
		textureCreateInfo.format = format;
		textureCreateInfo.usage = usage;
		textureCreateInfo.extent.depth = size.z;
		textureCreateInfo.extent.width = size.x;
		textureCreateInfo.extent.height = size.y;
		textureCreateInfo.arrayLayers = layers;
		textureCreateInfo.imageType = size.z == 1 ? VK_IMAGE_TYPE_2D : VK_IMAGE_TYPE_3D;
		textureCreateInfo.initialLayout = defaultLayout;
		textureCreateInfo.mipLevels = mipLevels;
		textureCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		textureCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL; // Texels are laid out in an implementation defined order for optimal access
		textureCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		Init(textureCreateInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	}

	Image::~Image()
	{
		vkDestroyImage(m_pDevice->GetDeviceHandle(), m_image, nullptr);
	}

	
}