#include "DescriptorSet.h"
#include "DescriptorPool.h"
#include "DescriptorSetLayout.h"
#include "Buffer.h"
#include "Texture.h"
#include "Sampler.h"
#include "Device.h"
#include <array>
#include <stdexcept>

#include <array>
#include <stdexcept>

namespace Rendering
{
    DescriptorSets::DescriptorSets(Application const& app, std::size_t size, DescriptorSetLayout const& layout)
        : Object(app)
        , m_pool(std::make_unique<DescriptorPool>(app, size))
    {
        std::vector<VkDescriptorSetLayout> layouts(size, layout.getHandle());

        VkDescriptorSetAllocateInfo descriptorSetAllocInfo{};
        descriptorSetAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        descriptorSetAllocInfo.descriptorPool = m_pool->getHandle();
        descriptorSetAllocInfo.descriptorSetCount = static_cast<uint32_t>(size);
        descriptorSetAllocInfo.pSetLayouts = layouts.data();

        m_handles.resize(size);
        if (vkAllocateDescriptorSets(getDevice().getHandle(), &descriptorSetAllocInfo, m_handles.data()) != VK_SUCCESS)
            throw std::runtime_error("failed to allocate descriptor sets!");
    }

    DescriptorSets::~DescriptorSets()
    {
      
    }

    void DescriptorSets::update(std::size_t index, Buffer const& uniformBuffer, std::shared_ptr<Texture> const& texture, std::shared_ptr<Sampler> const& sampler)
    {
        VkDescriptorSet setHandle = m_handles[index];

        std::vector<VkWriteDescriptorSet> descriptorWrites;

        {
            VkWriteDescriptorSet& descriptorWrite = descriptorWrites.emplace_back();

            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = uniformBuffer.getHandle();
            bufferInfo.offset = 0;
            bufferInfo.range = uniformBuffer.getSize();

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

        vkUpdateDescriptorSets(getDevice().getHandle(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }

    std::size_t DescriptorSets::getSize() const
    {
        return m_pool->getSize();
    }
}