#include "GraphicPipeline.h"
#include "Device.h"
#include "PipelineLayout.h"

#include "../Common/Macro.h"
#include "../Common/Logger.h"

namespace RHI
{
	GraphicPipeline::GraphicPipeline(const std::shared_ptr<Device> pDevice, const VkGraphicsPipelineCreateInfo info)
		: PipelineBase(pDevice)
	{
		if(!Init(pDevice, info))
			K_ERROR("Error Initialize Graphic Pipeline");
	}

	bool GraphicPipeline::Init(const std::shared_ptr<Device>& pDevice, const VkGraphicsPipelineCreateInfo& info)
	{

		if (!PipelineBase::Init(pDevice, m_pPipelineLayout))
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

	}
}