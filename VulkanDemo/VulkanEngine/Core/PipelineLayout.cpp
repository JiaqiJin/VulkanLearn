#include "PipelineLayout.h"
#include "DescriptorSetLayout.h"
#include "Device.h"
#include <array>
#include <stdexcept>

namespace Rendering
{
	PipelineLayout::PipelineLayout(const Device& device, const DescriptorSetLayout& descriptorSetLayout)
		: m_device(device)
	{
		std::array setLayouts{ descriptorSetLayout.getHandle() };

		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
		pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutCreateInfo.setLayoutCount = static_cast<uint32_t>(setLayouts.size());
		pipelineLayoutCreateInfo.pSetLayouts = setLayouts.data();
		pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
		pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

		if (vkCreatePipelineLayout(m_device.getHandle(), &pipelineLayoutCreateInfo, nullptr, &m_handle.get()) != VK_SUCCESS)
			throw std::runtime_error("failed to create pipeline layout!");
	}

	PipelineLayout::~PipelineLayout()
	{
		vkDestroyPipelineLayout(m_device.getHandle(), m_handle, nullptr);
	}
}