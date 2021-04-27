#pragma once

#include <vulkan/vulkan.h>
#include "UniqueHandle.h"
#include "../Objects/Object.h"
/*
* Wrapper class for VkFramebuffer
* Framebuffer represent a link to actual images that can be used as attachment. 
* Framebuffer is another layer of top Images and groups of ImageVies bounds during rendering of a specific Render Pass
*/
namespace Rendering
{
    class RenderPass;
    class ImageView;

    class Framebuffer : public Object
    {
    public:
        explicit Framebuffer(const Application& app, const ImageView& colorImageView, const ImageView& depthImageView,
            const RenderPass& renderPass, VkExtent2D extent);
        ~Framebuffer();

        Framebuffer(const Framebuffer&) = default;
        Framebuffer(Framebuffer&&) = default;
        Framebuffer& operator=(const Framebuffer&) = default;
        Framebuffer& operator=(Framebuffer&&) = default;

        VkFramebuffer getHandle() const { return m_handle; }

    private:
        UniqueHandle<VkFramebuffer> m_handle;
    };
}