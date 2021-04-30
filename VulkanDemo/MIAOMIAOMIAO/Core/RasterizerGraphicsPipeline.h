#pragma once

#include "Vulkan.h"

#include <memory>
#include <vector>

namespace Rendering
{
	class Device;
	class SwapChain;

	class RasterizerGraphicsPipeline
	{
	public:
		RasterizerGraphicsPipeline(const Device& device, const SwapChain& swapChain);
		~RasterizerGraphicsPipeline();

		VkPipeline GetPipeline() const
		{
			return m_pipeline;
		}

	private:
		const Device& m_device;
		const SwapChain& m_swapChain;
		VkPipeline m_pipeline{};
	};
}