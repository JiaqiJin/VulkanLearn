#include "DescriptorSet.h"
#include "DescriptorPool.h"
#include "DescriptorSetLayout.h"
#include "Device.h"
#include "Buffer.h"
#include "Texture.h"
#include "Sampler.h"

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

	void DescriptorSets::update(std::size_t index, const Buffer& uniformBuffer, 
		const std::shared_ptr<Texture>& texture, const std::shared_ptr<Sampler>& sampler)
	{
		VkDescriptorSet setHandle = m_handles[index];

		std::vector<VkWriteDescriptorSet> descriptorWrites;

		{
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = uniformBuffer.getHandle();
			bufferInfo.offset = 0;
			bufferInfo.range = uniformBuffer.getSize();

			VkWriteDescriptorSet& descriptorWrite = descriptorWrites.emplace_back();
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = setHandle;
			descriptorWrite.dstBinding = 0;
			descriptorWrite.dstArrayElement = 0;
			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.pBufferInfo = &bufferInfo;
		}

		if (texture && sampler)
		{
			VkWriteDescriptorSet& descriptorWrite = descriptorWrites.emplace_back();

			VkDescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = texture->getImageViewHandle();
			imageInfo.sampler = sampler->getHandle();

			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = setHandle;
			descriptorWrite.dstBinding = 1;
			descriptorWrite.dstArrayElement = 0;
			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.pImageInfo = &imageInfo;
		}

		vkUpdateDescriptorSets(m_device.getHandle(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}

	std::size_t DescriptorSets::getSize() const
	{
		return m_pool->getSize();
	}
}