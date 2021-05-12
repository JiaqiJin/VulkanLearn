#pragma once

#include "Vulkan.h"

namespace Rendering
{
	class ImageView;
	class SwapChain;
	class DepthBuffer;

	class Framebuffer
	{
	public:
		NON_COPIABLE(Framebuffer);

		Framebuffer(const ImageView& imageView,	const SwapChain& swapChain, const DepthBuffer& depthBuffer, VkRenderPass renderPass);
	
		~Framebuffer();

		VkFramebuffer Get() const
		{
			return m_framebuffer;
		}

	private:
		VkFramebuffer m_framebuffer{};
		const ImageView& m_imageView;
	};
}