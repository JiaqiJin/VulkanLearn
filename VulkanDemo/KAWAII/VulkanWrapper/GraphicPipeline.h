#pragma once

#include "PipelineBase.h"
#include <memory>
#include <vector>

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
		bool GraphicPipeline::Init();

	private:
		std::vector<VkPipelineColorBlendAttachmentState> m_blendStatesInfo; // Color blending
		std::vector<VkDynamicState> m_dynamicStates;
		std::vector<VkPipelineShaderStageCreateInfo> m_shaderStageInfo;
		std::vector<VkVertexInputBindingDescription> m_vertexBindingsInfo;
		std::vector<VkVertexInputAttributeDescription>	m_vertexAttributesInfo;

		VkPipelineColorBlendStateCreateInfo m_blendCreateInfo; // blending state
		VkPipelineVertexInputStateCreateInfo m_vertexInputCreateInfo; // vertex input
		VkGraphicsPipelineCreateInfo m_info; // Pipeline Info
		VkPipelineInputAssemblyStateCreateInfo m_assemblyCreateInfo; // Input assembly
		VkPipelineViewportStateCreateInfo m_viewportStateCreateInfo; // Viewports
		VkPipelineRasterizationStateCreateInfo m_rasterizerCreateInfo; // Rasterizer
		VkPipelineMultisampleStateCreateInfo m_multiSampleCreateInfo; // Multisampling
		VkPipelineDepthStencilStateCreateInfo m_depthStencilCreateInfo; // Depth and stencil testing
		VkPipelineDynamicStateCreateInfo m_dynamicStatesCreateInfo; // Dynamic state
	};
}