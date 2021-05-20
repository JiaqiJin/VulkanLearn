#include "Image.h"
#include "Device.h"
#include "Memory.h"

namespace Rendering
{
	Image::Image(const class Device& device,
		VkExtent2D extent,
		VkFormat format,
		VkImageTiling tiling,
		VkImageType imageType,
		VkImageUsageFlags usage,
		VkMemoryPropertyFlags properties)
		: m_device(device), m_extent(extent), m_format(format), m_imageType(imageType)
	{
		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = imageType;
		imageInfo.extent.width = extent.width;
		imageInfo.extent.height = extent.height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = format;
		imageInfo.tiling = tiling;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = usage;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VK_CHECK(vkCreateImage(device.Get(), &imageInfo, nullptr, &m_image), "Create image");

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(m_device.Get(), m_image, &memRequirements);

		m_memory.reset(new Memory(device, memRequirements, properties));
		// allocating memory
		vkBindImageMemory(device.Get(), m_image, m_memory->Get(), 0);
	}

	Image::~Image()
	{

	}
}