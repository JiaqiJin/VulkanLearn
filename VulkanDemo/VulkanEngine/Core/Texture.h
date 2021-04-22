#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include <string>
#include "../Objects/Object.h"

namespace tinygltf
{
    struct Image;
}

namespace Rendering
{
    class Image;
    class DeviceMemory;
    class ImageView;

    class Texture : public Object
    {
    public:
        Texture(Application const& app, std::string const& path);
        Texture(Application const& app, tinygltf::Image const& image);

        VkImageView getImageViewHandle() const;

    private:
        void createImage(void const* data, uint32_t width, uint32_t height, std::size_t bitsPerComponent, std::size_t components);

    private:
        std::unique_ptr<Image> m_image;
        std::unique_ptr<ImageView> m_imageView;
        std::unique_ptr<DeviceMemory> m_memory;
    };
}