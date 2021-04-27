#include "Image.h"

#include "DeviceMemory.h"
#include "ImageView.h"
#include "Device.h"
#include <stdexcept>

namespace Rendering
{
    Image::Image(Application const& app, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage) : Object(app)
    {
        m_format = format;

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

        if (vkCreateImage(getDevice().getHandle(), &imageCreateInfo, nullptr, &m_handle.get()) != VK_SUCCESS)
            throw std::runtime_error("failed to create image!");
    }

    Image::Image(Application const& app, VkImage image, VkFormat format) : Object(app)
    {
        m_isOwned = false;
        m_handle = image;
        m_format = format;
    }

    Image::~Image()
    {
        if (m_isOwned)
            vkDestroyImage(getDevice().getHandle(), m_handle, nullptr);
    }

    VkMemoryRequirements Image::getMemoryRequirements() const
    {
        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(getDevice().getHandle(), m_handle, &memRequirements);

        return memRequirements;
    }

    void Image::bindMemory(DeviceMemory const& memory) const
    {
        vkBindImageMemory(getDevice().getHandle(), m_handle, memory.getHandle(), 0);
    }

    std::unique_ptr<ImageView> Image::createImageView(VkImageAspectFlags aspectFlags)
    {
        return std::make_unique<ImageView>(getApp(), *this, aspectFlags);
    }
}