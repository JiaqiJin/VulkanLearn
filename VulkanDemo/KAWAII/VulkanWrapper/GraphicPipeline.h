#pragma once

#include "PipelineBase.h"
#include <memory>
#include <vector>

namespace RHI
{
	class ShaderModule;
	class RenderPass;
	class PipelineLayout;

	// Stages of the graphics pipeline
	class GraphicPipeline : public PipelineBase
	{
	public:
		GraphicPipeline(const std::shared_ptr<Device>& pDevice,
			const VkGraphicsPipelineCreateInfo info,
			const std::vector<std::shared_ptr<ShaderModule>> shaders,
			const std::shared_ptr<RenderPass>& pRenderPass,
			const std::shared_ptr<PipelineLayout>& pPipelineLayout);

		GraphicPipeline(const GraphicPipeline&) = delete;
		GraphicPipeline& operator=(const GraphicPipeline&) = delete;
		GraphicPipeline& operator=(GraphicPipeline&&) = delete;

		~GraphicPipeline();

		// Getters
		const VkGraphicsPipelineCreateInfo& GetInfo() const { return m_info; }
		std::shared_ptr<RenderPass> GetRenderPass() const { return m_pRenderPass; }
		std::shared_ptr<ShaderModule> GetShader(ShaderModule::ShaderType type) const { return m_shaders[(uint32_t)type]; }
		VkPipelineBindPoint GetPipelineBindingPoint() const override { return VK_PIPELINE_BIND_POINT_GRAPHICS; }
		uint32_t GetSubpassIndex() const override { return m_info.subpass; }

		virtual VkPipeline CreatePipeline() override;

		// filling in the structure (Shader stages, Fixed-function state, Pipeline layout, RenderPass)
		static void FillupPipelineCreateInfo(VkGraphicsPipelineCreateInfo& info, const std::vector<std::shared_ptr<ShaderModule>> shaders,
			const std::shared_ptr<RenderPass>& pRenderPass, const std::shared_ptr<PipelineLayout>& pPipelineLayout);

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
		VkPipelineInputAssemblyStateCreateInfo m_assemblyCreateInfo; // Input assembly
		VkPipelineViewportStateCreateInfo m_viewportStateCreateInfo; // Viewports
		VkPipelineRasterizationStateCreateInfo m_rasterizerCreateInfo; // Rasterizer
		VkPipelineMultisampleStateCreateInfo m_multiSampleCreateInfo; // Multisampling
		VkPipelineDepthStencilStateCreateInfo m_depthStencilCreateInfo; // Depth and stencil testing
		VkPipelineDynamicStateCreateInfo m_dynamicStatesCreateInfo; // Dynamic state

		VkGraphicsPipelineCreateInfo m_info; // Pipeline Info
		std::shared_ptr<RenderPass> m_pRenderPass;
		std::vector<std::shared_ptr<ShaderModule>> m_shaders;
	};
}