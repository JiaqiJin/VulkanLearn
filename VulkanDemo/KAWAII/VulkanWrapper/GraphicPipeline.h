#pragma once

#include "PipelineBase.h"

namespace RHI
{
	// Stages of the graphics pipeline
	class GraphicPipeline : public PipelineBase
	{
	public:
		GraphicPipeline(const std::shared_ptr<Device> pDevice, const VkGraphicsPipelineCreateInfo info);

		GraphicPipeline(const GraphicPipeline&) = delete;
		GraphicPipeline& operator=(const GraphicPipeline&) = delete;
		GraphicPipeline& operator=(GraphicPipeline&&) = delete;

		~GraphicPipeline();

		virtual VkPipeline CreatePipeline() override;

	private:
		bool GraphicPipeline::Init(const std::shared_ptr<Device>& pDevice, const VkGraphicsPipelineCreateInfo& info);

	private:
		VkPipelineVertexInputStateCreateInfo m_vertexInputCreateInfo; // vertex input

		VkGraphicsPipelineCreateInfo m_info; // Pipeline Info
	};
}