#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <memory>
#include "UniqueHandle.h"
#include "../Objects/Object.h"
/*
* Wrapper class for VkSwapchainKHR
* 
*/
namespace Rendering
{

    class Image;
    class ImageView;
    class RenderPass;
    class Framebuffer;

    class Swapchain : public Object
    {
    public:
        Swapchain(const Application& app);
        ~Swapchain();

        Swapchain(const Swapchain&) = default;
        Swapchain(Swapchain&&) = default;
        Swapchain& operator=(const Swapchain&) = default;
        Swapchain& operator=(Swapchain&&) = default;

        void createFramebuffers(const RenderPass& renderPass, const ImageView& depthImageView);

        VkSwapchainKHR getHandle() const { return m_handle; }
        VkExtent2D getExtent() const { return m_extent; }
        VkSurfaceFormatKHR getSurfaceFormat() const { return m_surfaceFormat; }

        std::size_t getImageCount() const { return m_images.size(); }
        const std::vector<std::unique_ptr<Image>>& getImages() const { return m_images; }
        const std::vector<std::unique_ptr<ImageView>>& getImageViews() const { return m_imageViews; }
        const std::vector<std::unique_ptr<Framebuffer>>& getFramebuffers() const { return m_framebuffers; }

    private:
        void createSwapchain();
        void retrieveImages();
        void createImageViews();

    private:
        UniqueHandle<VkSwapchainKHR> m_handle;

        VkExtent2D m_extent;
        VkSurfaceFormatKHR m_surfaceFormat;

        std::vector<std::unique_ptr<Image>> m_images;
        std::vector<std::unique_ptr<ImageView>> m_imageViews;
        std::vector<std::unique_ptr<Framebuffer>> m_framebuffers;
    };
}