#include "Image.h"
#include "Device.h"
#include "ImageView.h"
#include "DeviceMemory.h"

#include <stdexcept>

namespace Rendering
{
	Image::Image(const Device& device, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage)
		: m_device(device)
	{
		m_format = format;
		m_tiling = tiling;
		VkImageCreateInfo imageCreateInfo{};
		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		imageCreateInfo.extent.width = width;
		imageCreateInfo.extent.height = height;
		imageCreateInfo.extent.depth = 1;
		imageCreateInfo.mipLevels = 1;
		imageCreateInfo.arrayLayers = 1;
		imageCreateInfo.format = format;
		imageCreateInfo.tiling = tiling;
		imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageCreateInfo.usage = usage;
		imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;

		if (vkCreateImage(device.getHandle(), &imageCreateInfo, nullptr, &m_handle.get()) != VK_SUCCESS)
			throw std::runtime_error("failed to create image!");
		// changed to vmaCreateImage(device.getMemoryAllocator(), &imageCreateInfo, & memInfo, &m_handle, &memory, nullptr);
	}

	Image::Image(const Device& device, VkImage image, VkFormat format)
		:m_device(device)
	{
		m_isOwned = false;
		m_handle = image;
		m_format = format;
		// here m_memory == memory; add in feature
	}

	Image::Image(const Device& device, VkImage image, VkFormat format, VkImageTiling tiling)
		:m_device(device)
	{
		m_isOwned = false;
		m_handle = image;
		m_format = format;
		m_tiling = tiling;
		// here m_memory == memory; add in feature
	}

	Image::~Image()
	{
		if (m_isOwned)
			vkDestroyImage(m_device.getHandle(), m_handle, nullptr);
		if (m_Vmamemory != VK_NULL_HANDLE && m_isOwned)
		{
			unMap();
			vmaDestroyImage(m_device.getMemoryAllocator(), m_handle, m_Vmamemory);
		}
	}

	std::unique_ptr<ImageView> Image::createImageView(VkImageAspectFlags aspectFlags)
	{
		return std::make_unique<ImageView>(m_device, *this, aspectFlags);
	}

	uint8_t* Image::Map()
	{
		if (!mappedData)
		{
			if (m_tiling != VK_IMAGE_TILING_LINEAR)
			{
				printf("Mapping image memory that is not linear");
			}
			auto result = (vmaMapMemory(m_device.getMemoryAllocator(), m_Vmamemory, reinterpret_cast<void**>(&mappedData)));
			if(result != VK_SUCCESS)
				throw std::runtime_error("failed Mapping memory!");
			mapped = true;
		}
		return mappedData;
	}

	void Image::unMap()
	{
		if (mapped)
		{
			vmaUnmapMemory(m_device.getMemoryAllocator(), m_Vmamemory);
			mappedData = nullptr;
			mapped = false;
		}
	}

	VkMemoryRequirements Image::getMemoryRequirements() const
	{
		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(m_device.getHandle(), m_handle, &memRequirements);

		return memRequirements;
	}

	void Image::bindMemory(DeviceMemory const& memory) const
	{
		vkBindImageMemory(m_device.getHandle(), m_handle, memory.getHandle(), 0);
	}

}