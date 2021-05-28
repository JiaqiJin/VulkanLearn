#include "DescriptorSetLayout.h"
#include "Device.h"

#include "../Common/Macro.h"
#include "../Common/Logger.h"

namespace RHI
{
	DescriptorSetLayout::DescriptorSetLayout(const std::shared_ptr<Device> pDevice,
		const std::vector<VkDescriptorSetLayoutBinding> dsLayoutBinding)
		: m_pDevice(pDevice), m_descriptorSetLayoutBinding(dsLayoutBinding)
	{
		VkDescriptorSetLayoutCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		createInfo.bindingCount = (uint32_t)m_descriptorSetLayoutBinding.size();
		createInfo.pBindings = m_descriptorSetLayoutBinding.data();
		CHECK_VK_ERROR(vkCreateDescriptorSetLayout(m_pDevice->GetDeviceHandle(), &createInfo, nullptr, &m_descriptorSetLayout));
	}

	DescriptorSetLayout::~DescriptorSetLayout()
	{
		vkDestroyDescriptorSetLayout(m_pDevice->GetDeviceHandle(), m_descriptorSetLayout, nullptr);
	}
}