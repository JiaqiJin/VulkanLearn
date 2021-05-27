#include "GraphicPipeline.h"
#include "Device.h"
#include "PipelineLayout.h"

#include "../Common/Macro.h"
#include "../Common/Logger.h"

namespace RHI
{
	GraphicPipeline::GraphicPipeline(const std::shared_ptr<Device> pDevice, const VkGraphicsPipelineCreateInfo info)
		: PipelineBase(pDevice), m_info(info)
	{
		if(!Init())
			K_ERROR("Error Initialize Graphic Pipeline");
	}

	bool GraphicPipeline::Init()
	{
		// Shader 
		m_shaderStageInfo.resize(m_info.stageCount);
		for (uint32_t i = 0; i < m_info.stageCount; i++)
			m_shaderStageInfo[i] = m_info.pStages[i];
		m_info.pStages = m_shaderStageInfo.data();

		// Vertex Buffer
		m_vertexBindingsInfo.resize(m_info.pVertexInputState->vertexBindingDescriptionCount);
		for (uint32_t i = 0; i < m_info.pVertexInputState->vertexBindingDescriptionCount; i++)
			m_vertexBindingsInfo[i] = m_info.pVertexInputState->pVertexBindingDescriptions[i];

		m_vertexAttributesInfo.resize(m_info.pVertexInputState->vertexAttributeDescriptionCount);
		for (uint32_t i = 0; i < m_info.pVertexInputState->vertexAttributeDescriptionCount; i++)
			m_vertexAttributesInfo[i] = m_info.pVertexInputState->pVertexAttributeDescriptions[i];

		// Vertex input
		m_vertexInputCreateInfo = *m_info.pVertexInputState;
		m_vertexInputCreateInfo.pVertexBindingDescriptions = m_vertexBindingsInfo.data();
		m_vertexInputCreateInfo.pVertexAttributeDescriptions = m_vertexAttributesInfo.data();
		m_info.pVertexInputState = &m_vertexInputCreateInfo;
		
		// Input assembly : what king of geometry to draw and if primitive restar shoul be enable
		m_assemblyCreateInfo = *m_info.pInputAssemblyState;
		m_info.pInputAssemblyState = &m_assemblyCreateInfo;

		// Multisampling
		m_multiSampleCreateInfo = *m_info.pMultisampleState;
		m_info.pMultisampleState = &m_multiSampleCreateInfo;

		// Rasterizer : Takes the geometry that is shaped by the vertices from the vertex shader 
		// and turns it into fragments to be colored by the fragment shader. 
		m_rasterizerCreateInfo = *m_info.pRasterizationState;
		m_info.pRasterizationState = &m_rasterizerCreateInfo;

		// Force viewport & scissor state to be dynamic
		m_viewportStateCreateInfo = *m_info.pViewportState;
		m_viewportStateCreateInfo.viewportCount = 1;
		m_viewportStateCreateInfo.pScissors = nullptr;
		m_viewportStateCreateInfo.scissorCount = 1;
		m_viewportStateCreateInfo.pViewports = nullptr;
		m_info.pViewportState = &m_viewportStateCreateInfo;

		m_dynamicStates =
		{
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		m_dynamicStatesCreateInfo = *m_info.pDynamicState;
		m_dynamicStatesCreateInfo.dynamicStateCount = (uint32_t)m_dynamicStates.size();
		m_dynamicStatesCreateInfo.pDynamicStates = m_dynamicStates.data();
		m_info.pDynamicState = &m_dynamicStatesCreateInfo;


		// Blend State
		for (uint32_t i = 0; i < m_info.pColorBlendState->attachmentCount; i++)
			m_blendStatesInfo.push_back(m_info.pColorBlendState->pAttachments[i]);

		if (!PipelineBase::Init(m_device, m_pPipelineLayout))
			return false;

		return true;
	}

	VkPipeline GraphicPipeline::CreatePipeline()
	{
		VkPipeline pipeline;
		CHECK_VK_ERROR(vkCreateGraphicsPipelines(m_device->GetDeviceHandle(), 0, 1, &m_info, nullptr, &pipeline));
		return pipeline;
	}

	GraphicPipeline::~GraphicPipeline()
	{
		for (uint32_t i = 0; i < m_shaderStageInfo.size(); i++)
			delete[] m_shaderStageInfo[i].pName;
	}
}