#include "GraphicPipeline.h"
#include "Device.h"
#include "PipelineLayout.h"
#include "RenderPass.h"

#include "../Common/Macro.h"
#include "../Common/Logger.h"

namespace RHI
{
	GraphicPipeline::GraphicPipeline(const std::shared_ptr<Device>& pDevice,
		const VkGraphicsPipelineCreateInfo info,
		const std::vector<std::shared_ptr<ShaderModule>> shaders,
		const std::shared_ptr<RenderPass>& pRenderPass,
		const std::shared_ptr<PipelineLayout>& pPipelineLayout)
		: PipelineBase(pDevice, pPipelineLayout),
		m_info(info),
		m_shaders(shaders),
		m_pRenderPass(pRenderPass)
	{
		//m_pPipelineLayout = pPipelineLayout;

		m_info.renderPass = pRenderPass->GetDeviceHandle();
		m_info.layout = pPipelineLayout->GetDeviceHandle();

		std::vector<VkPipelineShaderStageCreateInfo> stages(shaders.size());
		for (uint32_t i = 0; i < stages.size(); i++)
		{
			stages[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			stages[i].stage = shaders[i]->GetShaderStage();
			stages[i].module = shaders[i]->GetDeviceHandle();

			char* pEntryName = new char[ENTRY_NAME_LENGTH];
			strcpy_s(pEntryName, ENTRY_NAME_LENGTH, shaders[i]->GetEntryName().c_str());
			stages[i].pName = pEntryName;
		}

		m_info.stageCount = (uint32_t)stages.size();
		m_info.pStages = stages.data();

		if(!Init())
			K_ERROR("Error Initialize Graphic Pipeline");
	}

	bool GraphicPipeline::Init()
	{
		// Blend stage
		for (uint32_t i = 0; i < m_info.pColorBlendState->attachmentCount; i++)
			m_blendStatesInfo.push_back(m_info.pColorBlendState->pAttachments[i]);

		m_blendCreateInfo = *m_info.pColorBlendState;
		m_blendCreateInfo.pAttachments = m_blendStatesInfo.data();
		m_info.pColorBlendState = &m_blendCreateInfo;

		// Depth Stencil stage
		m_depthStencilCreateInfo = *m_info.pDepthStencilState;
		m_info.pDepthStencilState = &m_depthStencilCreateInfo;

		// Input state
		m_assemblyCreateInfo = *m_info.pInputAssemblyState;
		m_info.pInputAssemblyState = &m_assemblyCreateInfo;

		// Multisampling 
		m_multiSampleCreateInfo = *m_info.pMultisampleState;
		m_info.pMultisampleState = &m_multiSampleCreateInfo;

		// Rasterization
		m_rasterizerCreateInfo = *m_info.pRasterizationState;
		m_info.pRasterizationState = &m_rasterizerCreateInfo;

		// Force viewport & scissor state to be dynamic
		m_viewportStateCreateInfo = *m_info.pViewportState;
		m_viewportStateCreateInfo.viewportCount = 1;
		m_viewportStateCreateInfo.pScissors = nullptr;
		m_viewportStateCreateInfo.scissorCount = 1;
		m_viewportStateCreateInfo.pViewports = nullptr;

		// Dynamic stage
		m_dynamicStates =
		{
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		m_dynamicStatesCreateInfo = *m_info.pDynamicState;
		m_dynamicStatesCreateInfo.dynamicStateCount = (uint32_t)m_dynamicStates.size();
		m_dynamicStatesCreateInfo.pDynamicStates = m_dynamicStates.data();
		m_info.pDynamicState = &m_dynamicStatesCreateInfo;

		// Shader stage
		m_shaderStageInfo.resize(m_info.stageCount);
		for (uint32_t i = 0; i < m_info.stageCount; i++)
			m_shaderStageInfo[i] = m_info.pStages[i];
		m_info.pStages = m_shaderStageInfo.data();

		// Vertex stage
		m_vertexBindingsInfo.resize(m_info.pVertexInputState->vertexBindingDescriptionCount);
		for (uint32_t i = 0; i < m_info.pVertexInputState->vertexBindingDescriptionCount; i++)
			m_vertexBindingsInfo[i] = m_info.pVertexInputState->pVertexBindingDescriptions[i];

		m_vertexAttributesInfo.resize(m_info.pVertexInputState->vertexAttributeDescriptionCount);
		for (uint32_t i = 0; i < m_info.pVertexInputState->vertexAttributeDescriptionCount; i++)
			m_vertexAttributesInfo[i] = m_info.pVertexInputState->pVertexAttributeDescriptions[i];

		m_vertexInputCreateInfo = *m_info.pVertexInputState;
		m_vertexInputCreateInfo.pVertexBindingDescriptions = m_vertexBindingsInfo.data();
		m_vertexInputCreateInfo.pVertexAttributeDescriptions = m_vertexAttributesInfo.data();
		m_info.pVertexInputState = &m_vertexInputCreateInfo;

		// Init
		if (!PipelineBase::Init())
			return false;

		return true;
	}

	VkPipeline GraphicPipeline::CreatePipeline()
	{
		VkPipeline pipeline;
		CHECK_VK_ERROR(vkCreateGraphicsPipelines(m_device->GetDeviceHandle(), 0, 1, &m_info, nullptr, &pipeline));
		return pipeline;
	}

	void GraphicPipeline::FillupPipelineCreateInfo(VkGraphicsPipelineCreateInfo& info, const std::vector<std::shared_ptr<ShaderModule>> shaders,
		const std::shared_ptr<RenderPass>& pRenderPass, const std::shared_ptr<PipelineLayout>& pPipelineLayout)
	{
		info.renderPass = pRenderPass->GetDeviceHandle();
		info.layout = pPipelineLayout->GetDeviceHandle();

		std::vector<VkPipelineShaderStageCreateInfo> stages(shaders.size());
		for (uint32_t i = 0; i < stages.size(); i++)
		{
			stages[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			stages[i].stage = shaders[i]->GetShaderStage();
			stages[i].module = shaders[i]->GetDeviceHandle();

			char* pEntryName = new char[ENTRY_NAME_LENGTH];
			strcpy_s(pEntryName, ENTRY_NAME_LENGTH, shaders[i]->GetEntryName().c_str());
			stages[i].pName = pEntryName;
		}
		info.stageCount = (uint32_t)stages.size();
		info.pStages = stages.data();
	}

	GraphicPipeline::~GraphicPipeline()
	{
		for (uint32_t i = 0; i < m_shaderStageInfo.size(); i++)
			delete[] m_shaderStageInfo[i].pName;
	}
}