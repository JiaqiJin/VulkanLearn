#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include "ShaderModule.h"

namespace RHI
{
	class RenderPass;
	class PipelineLayout;
	class Device;

	class PipelineBase
	{
	public:
		PipelineBase(std::shared_ptr<Device> pDevice);
		virtual ~PipelineBase();

		VkPipeline GetDeviceHandle() const { return m_pipeline; }
		std::shared_ptr<PipelineLayout> GetPipelineLayout() const { return m_pPipelineLayout; }
		virtual VkPipelineBindPoint GetPipelineBindingPoint() const = 0;
		virtual uint32_t GetSubpassIndex() const { return 0; }

	protected:
		bool Init(const std::shared_ptr<Device>& pDevice, const std::shared_ptr<PipelineLayout>& pPipelineLayout);

		virtual VkPipeline CreatePipeline() = 0;

	protected:
		static const uint32_t ENTRY_NAME_LENGTH = 64;

		std::shared_ptr<Device> m_device;
		VkPipeline m_pipeline;
		std::shared_ptr<PipelineLayout> m_pPipelineLayout;
		VkPipelineBindPoint m_pipelineBindingPoint;
	};
}