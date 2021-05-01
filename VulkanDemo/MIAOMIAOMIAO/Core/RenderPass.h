#pragma once

#include "Vulkan.h"

namespace Rendering
{
	class Device;
	class SwapChain;
	/*
	* RenderPass : Framebuffer attachment that will be use in rendering. Color, depth buffer and sampler to use and how thir should handle
	* Subpasses are subsequent rendering operation that depend on content of framebuffer in the previus passess.
	*/
	class RenderPass
	{
	public:
		NON_COPIABLE(RenderPass);

		RenderPass(const Device& device, const SwapChain& swapChain, bool clearColorBuffer,	bool clearDepthBuffer);
		~RenderPass();

		VkRenderPass Get() const
		{
			return m_renderPass;
		}

	private:
		VkRenderPass m_renderPass;
		const Device& m_device;
	};
}