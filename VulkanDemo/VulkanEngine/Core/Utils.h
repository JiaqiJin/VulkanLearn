#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include <vector>

namespace Rendering
{
	class Buffer;
	class DeviceMemory;
	class Image;
	class Application;

	namespace utils
	{
		bool checkSupportOption(std::vector<char const*> const& availableOptions, std::vector<char const*> const& requestedOptions);

		void createBuffer(Application const& app, VkDeviceSize size, VkBufferUsageFlags usage, 
			VkMemoryPropertyFlags properties, std::unique_ptr<Buffer>& buffer, std::unique_ptr<DeviceMemory>& bufferMemory);

		void createImage(Application const& app, uint32_t width, uint32_t height, 
			VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, 
			std::unique_ptr<Image>& image, std::unique_ptr<DeviceMemory>& imageMemory);
	}
}