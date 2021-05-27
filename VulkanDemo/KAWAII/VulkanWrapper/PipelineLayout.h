#pragma once

#include <vulkan/vulkan.h>

namespace RHI
{
	class Device;

	class PipelineLayout
	{
	public:
		
	private:
		VkPipelineLayout m_pipelineLayout;

		friend class PipelineBase;
	};
}