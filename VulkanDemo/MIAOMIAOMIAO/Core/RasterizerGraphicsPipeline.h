#pragma once

#include "Vulkan.h"

#include <memory>
#include <vector>

namespace Rendering
{
	class Device;
	class SwapChain;
	class RenderPass;

	class RasterizerGraphicsPipeline
	{
	public:
		RasterizerGraphicsPipeline(const Device& device, const SwapChain& swapChain);
		~RasterizerGraphicsPipeline();

		VkPipeline GetPipeline() const
		{
			return m_pipeline;
		}

		VkRenderPass GetRenderPass() const;

	private:
		const Device& m_device;
		const SwapChain& m_swapChain;
		VkPipeline m_pipeline{};

		std::unique_ptr<RenderPass> m_renderPass;
	};
}