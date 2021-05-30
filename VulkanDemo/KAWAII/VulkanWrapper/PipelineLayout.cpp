#include "PipelineLayout.h"
#include "Device.h"
#include "DescriptorSetLayout.h"

#include "../Common/Macro.h"
#include "../Common/Logger.h"

namespace RHI
{
	PipelineLayout::PipelineLayout(const std::shared_ptr<Device>& pDevice, const DescriptorSetLayoutList& descriptorSetLayoutList)
		: m_pDevice(pDevice), m_descriptorSetLayoutList(descriptorSetLayoutList)
	{
		std::vector<VkDescriptorSetLayout> dsLayoutDeviceHandleList = GetDescriptorSetLayoutDeviceHandleList();

		VkPipelineLayoutCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		info.setLayoutCount = (uint32_t)dsLayoutDeviceHandleList.size();
		info.pSetLayouts = dsLayoutDeviceHandleList.data();

		CHECK_VK_ERROR(vkCreatePipelineLayout(m_pDevice->GetDeviceHandle(), &info, nullptr, &m_pipelineLayout));
	}

	PipelineLayout::PipelineLayout(const std::shared_ptr<Device>& pDevice, const DescriptorSetLayoutList& descriptorSetLayoutList,
		const std::vector<VkPushConstantRange>& pushConstsRanges)
		: m_pDevice(pDevice), m_descriptorSetLayoutList(descriptorSetLayoutList)
	{
		std::vector<VkDescriptorSetLayout> dsLayoutDeviceHandleList = GetDescriptorSetLayoutDeviceHandleList();

		VkPipelineLayoutCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		info.setLayoutCount = (uint32_t)dsLayoutDeviceHandleList.size();
		info.pSetLayouts = dsLayoutDeviceHandleList.data();
		info.pushConstantRangeCount = (uint32_t)pushConstsRanges.size();
		info.pPushConstantRanges = pushConstsRanges.data();

		CHECK_VK_ERROR(vkCreatePipelineLayout(m_pDevice->GetDeviceHandle(), &info, nullptr, &m_pipelineLayout));
	}

	const std::vector<VkDescriptorSetLayout> PipelineLayout::GetDescriptorSetLayoutDeviceHandleList()
	{
		std::vector<VkDescriptorSetLayout> descriptorSetLayoutList;
		descriptorSetLayoutList.resize(m_descriptorSetLayoutList.size());
		for (uint32_t i = 0; i < m_descriptorSetLayoutList.size(); i++)
			descriptorSetLayoutList[i] = m_descriptorSetLayoutList[i]->GetDeviceHandle();

		return descriptorSetLayoutList;
	}

	PipelineLayout::~PipelineLayout()
	{
		vkDestroyPipelineLayout(m_pDevice->GetDeviceHandle(), m_pipelineLayout, nullptr);
	}
}
