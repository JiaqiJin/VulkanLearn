#pragma once

#include "Vulkan.h"

#include <memory>

namespace Rendering
{
	class Device;
	class CommandPool;
	class Memory;

	class Image
	{
	public:
		NON_COPIABLE(Image);
		Image(const class Device& device,
			VkExtent2D extent,
			VkFormat format,
			VkImageTiling tiling,
			VkImageType imageType,
			VkImageUsageFlags usage,
			VkMemoryPropertyFlags properties);
		~Image();

		VkImage Get() const
		{
			return m_image;
		}

		VkFormat GetFormat() const
		{
			return m_format;
		}

		const class Memory& GetMemory() const
		{
			return *m_memory;
		}

		const class Device& GetDevice() const
		{
			return m_device;
		}

	private:
		VkImage m_image;
		VkExtent2D m_extent;
		VkFormat m_format;
		VkImageType m_imageType;
		const Device& m_device;
		std::unique_ptr<Memory> m_memory;
	};
}