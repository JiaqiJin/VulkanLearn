#include "DescriptorSet.h"
#include "DescriptorPool.h"
#include "DescriptorSetLayout.h"
#include "Device.h"

#include <array>
#include <stdexcept>

namespace Rendering
{
	DescriptorSets::DescriptorSets(const Device& device, size_t size, const DescriptorSetLayout& layout)
		:m_device(device) ,
		m_pool(std::make_unique<DescriptorPool>(device, size))
	{
		std::vector<VkDescriptorSetLayout> layouts(size, layout.getHandle());

		VkDescriptorSetAllocateInfo descriptorSetAllocInfo{};
		descriptorSetAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		descriptorSetAllocInfo.descriptorPool = m_pool->getHandle();
		descriptorSetAllocInfo.descriptorSetCount = static_cast<uint32_t>(size);
		descriptorSetAllocInfo.pSetLayouts = layouts.data();

		m_handles.resize(size);

		if (vkAllocateDescriptorSets(m_device.getHandle(), &descriptorSetAllocInfo, m_handles.data()) != VK_SUCCESS)
			throw std::runtime_error("failed to allocate descriptor sets!");
	}

	DescriptorSets::~DescriptorSets()
	{

	}

	std::size_t DescriptorSets::getSize() const
	{
		return m_pool->getSize();
	}
}