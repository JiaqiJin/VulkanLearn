#pragma once

#include <vulkan/vulkan.h>
#include "UniqueHandle.h"
#include "../Objects/Object.h"
/*
* Wrapper class for VkRenderPass
* Tell to vulkan framebuffer to use while rendering (color and depth buffer, samples to use). 
* A single render pass consist multiple Subpasses and attachment references
*/
namespace Rendering
{
    class Swapchain;

    class RenderPass : public Object
    {
    public:
        explicit RenderPass(const Application& app, const Swapchain& swapchain);
        ~RenderPass();

        RenderPass(const RenderPass&) = default;
        RenderPass(RenderPass&&) = default;
        RenderPass& operator=(const RenderPass&) = default;
        RenderPass& operator=(RenderPass&&) = default;

        VkRenderPass getHandle() const { return m_handle; }
        VkFormat getDepthFormat() const { return m_depthFormat; }

    private:
        UniqueHandle<VkRenderPass> m_handle;
        VkFormat m_depthFormat;
    };
}