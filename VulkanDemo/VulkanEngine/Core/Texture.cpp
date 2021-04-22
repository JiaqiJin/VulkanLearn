#include "Texture.h"
#include "Image.h"
#include "Buffer.h"
#include "DeviceMemory.h"
#include "ImageView.h"
#include "ScopedOneTimeCommandBuffer.h"
#include "Utils.h"
#include "../vendor/tiny_gltf.h"
#include <stb_image.h>
#include <stdexcept>

namespace Rendering
{
    void transitionImageLayout(const Application& app, VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout)
    {
        ScopedOneTimeCommandBuffer commandBuffer{ app };

        // Barrier use for syncronization 
        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = 0;

        VkPipelineStageFlags sourceStage;
        VkPipelineStageFlags destinationStage;

        if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
        {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
        {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        else
        {
            throw std::invalid_argument("unsupported layout transition!");
        }

        vkCmdPipelineBarrier(
            commandBuffer.getHandle(),
            sourceStage, destinationStage,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier
        );
    }

    // Which part of the buffer copie to the image
    void copyBufferToImage(const Application& app, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
    {
        ScopedOneTimeCommandBuffer commandBuffer{ app };

        VkBufferImageCopy region{};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;

        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;

        region.imageOffset = { 0, 0, 0 };
        region.imageExtent = {
            width,
            height,
            1
        };

        vkCmdCopyBufferToImage(
            commandBuffer.getHandle(),
            buffer,
            image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1,
            &region
        );
    }

    Texture::Texture(Application const& app, std::string const& path)
        : Object(app)
    {
        uint32_t width, height;
        uint32_t bitsPerComponent;
        uint32_t components;
        void* pixels;

        {
            int texWidth, texHeight, texChannels;
            pixels = stbi_load(path.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

            width = static_cast<uint32_t>(texWidth);
            height = static_cast<uint32_t>(texHeight);
            bitsPerComponent = 8;
            components = 4;
        }

        createImage(pixels, width, height, bitsPerComponent, components);

        stbi_image_free(pixels);
    }

    Texture::Texture(Application const& app, tinygltf::Image const& image)
        : Object(app)
    {
        uint32_t width = static_cast<uint32_t>(image.width);
        uint32_t height = static_cast<uint32_t>(image.height);

        std::size_t bitsPerComponent = static_cast<std::size_t>(image.bits);
        std::size_t components = static_cast<std::size_t>(image.component);

        createImage(image.image.data(), width, height, bitsPerComponent, components);
    }

    VkImageView Texture::getImageViewHandle() const
    {
        return m_imageView->getHandle();
    }

    void Texture::createImage(void const* data, uint32_t width, uint32_t height, std::size_t bitsPerComponent, std::size_t components)
    {
        if (!data)
            throw std::runtime_error("failed to load texture image!");

        std::size_t bytesPerPixel = components * bitsPerComponent / 8;

        std::size_t imageSize = width * height * bytesPerPixel;

        std::unique_ptr<Buffer> stagingBuffer;
        std::unique_ptr<DeviceMemory> stagingBufferMemory;
        utils::createBuffer(getApp(), imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        stagingBufferMemory->copyFrom(data, imageSize);

        // Creating our image class
        utils::createImage(getApp(), width, height, VK_FORMAT_R8G8B8A8_SRGB, 
            VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            m_image, m_memory);


        // Layout transitions (recording and excuting the command buffer)
        transitionImageLayout(getApp(), m_image->getHandle(), VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        copyBufferToImage(getApp(), stagingBuffer->getHandle(), m_image->getHandle(), width, height);
        transitionImageLayout(getApp(), m_image->getHandle(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        m_imageView = m_image->createImageView(VK_IMAGE_ASPECT_COLOR_BIT);
    }
}