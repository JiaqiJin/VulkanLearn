#include "PipelineBase.h"
#include "Device.h"

#include "../Common/Macro.h"
#include "../Common/Logger.h"

namespace RHI
{
	PipelineBase::PipelineBase(const std::shared_ptr<Device>& pDevice, const std::shared_ptr<PipelineLayout>& pPipelineLayout)
		: m_device(pDevice), m_pPipelineLayout(pPipelineLayout)
	{

	}

	bool PipelineBase::Init()
	{
		//m_pPipelineLayout = pPipelineLayout;
		m_pipeline = CreatePipeline();

		return true;
	}

	PipelineBase::~PipelineBase()
	{
		vkDestroyPipeline(m_device->GetDeviceHandle(), m_pipeline, nullptr);
	}

}