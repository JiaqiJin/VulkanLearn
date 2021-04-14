#pragma once

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>
#include <memory>
#include "UniqueHandle.h"

namespace Rendering
{
	class Device;
	class ImageView;

	class Image
	{
	public:
		Image(const Device& device, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage);
		Image(const Device& device, VkImage image, VkFormat format);
		Image(const Device& device, VkImage image, VkFormat format, VkImageTiling tiling);
		~Image();

		Image(const Image&) = default;
		Image(Image&&) = default;
		Image& operator=(const Image&) = default;
		Image& operator=(Image&&) = default;

		VmaAllocation getMemory() const { return m_memory; }
		VkImage getHandle() const { return m_handle; }
		VkFormat getFormat() const { return m_format; }
		VkImageTiling getTiling() const { return m_tiling; }
		const Device& getDevice() const { return m_device; }
		
		std::unique_ptr<ImageView> createImageView(VkImageAspectFlags aspectFlags);

		// Features
		uint8_t* Map();
		void unMap();

	private:
		const Device& m_device;
		UniqueHandle<VkImage> m_handle;
		bool m_isOwned = true;
		
		VmaAllocation m_memory{ VK_NULL_HANDLE };

		VkImageTiling m_tiling;
		VkFormat m_format;

		uint8_t* mappedData{ nullptr };
		bool mapped = false;
	};
}