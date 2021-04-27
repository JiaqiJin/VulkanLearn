#include "Utils.h"
#include "../Application.h"
#include "Image.h"
#include "Buffer.h"
#include "DeviceMemory.h"
#include <algorithm>

namespace Rendering
{
    bool utils::checkSupportOption(std::vector<char const*> const& availableOptions, std::vector<char const*> const& requestedOptions)
    {
        for (const auto& requestedOption : requestedOptions)
        {
            auto it = std::find_if(availableOptions.begin(), availableOptions.end(), [requestedOption](char const* availableOption)
                {
                    return std::strcmp(availableOption, requestedOption) == 0;
                });

            if (it == availableOptions.end())
                return false;
        }

        return true;
    }

    void utils::createBuffer(Application const& app, VkDeviceSize size, VkBufferUsageFlags usage,
        VkMemoryPropertyFlags properties, std::unique_ptr<Buffer>& buffer, std::unique_ptr<DeviceMemory>& bufferMemory)
    {
        buffer = std::make_unique<Buffer>(app, size, usage);
        bufferMemory = std::make_unique<DeviceMemory>(app, buffer->getMemoryRequirements(), properties);
        buffer->bindMemory(*bufferMemory);
    }

    void utils::createImage(Application const& app, uint32_t width, uint32_t height,
        VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties,
        std::unique_ptr<Image>& image, std::unique_ptr<DeviceMemory>& imageMemory)
    {
        image = std::make_unique<Image>(app, width, height, format, tiling, usage);
        imageMemory = std::make_unique<DeviceMemory>(app, image->getMemoryRequirements(), properties);
        image->bindMemory(*imageMemory);
    }



}