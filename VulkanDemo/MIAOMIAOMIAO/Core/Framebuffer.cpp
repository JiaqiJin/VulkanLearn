#include "Framebuffer.h"
#include "ImageView.h"
#include "Framebuffer.h"
#include "SwapChain.h"
#include "Device.h"
#include "DepthBuffer.h"

#include <array>

namespace Rendering
{
	Framebuffer::Framebuffer(const ImageView& imageView, const SwapChain& swapChain, const DepthBuffer& depthBuffer, VkRenderPass rendePass)
		: m_imageView(imageView)
	{
		std::array<VkImageView, 2> attachments =
		{
			imageView.Get(),
			depthBuffer.GetImageView().Get()
		};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = rendePass;
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = swapChain.Extent.width;
		framebufferInfo.height = swapChain.Extent.height;
		framebufferInfo.layers = 1;

		VK_CHECK(vkCreateFramebuffer(m_imageView.GetDevice().Get(), &framebufferInfo, nullptr, &m_framebuffer), "Create framebuffer");
	}

	Framebuffer::~Framebuffer()
	{
		if (m_framebuffer != nullptr)
		{
			vkDestroyFramebuffer(m_imageView.GetDevice().Get(), m_framebuffer, nullptr);
			m_framebuffer = nullptr;
		}
	}
}