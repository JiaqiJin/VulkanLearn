#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <memory>
#include <cstddef>
#include "UniqueHandle.h"

namespace Rendering
{
    class Device;
    class DescriptorPool;
    class DescriptorSetLayout;
    class Buffer;
    class Texture;
    class Sampler;

    class DescriptorSets
    {
    public:
        DescriptorSets(const Device& device, size_t size, const DescriptorSetLayout& layout);
        ~DescriptorSets();

        DescriptorSets(const DescriptorSets&) = default;
        DescriptorSets(DescriptorSets&&) = default;
        DescriptorSets& operator=(const DescriptorSets&) = default;
        DescriptorSets& operator=(DescriptorSets&&) = default;

        void update(std::size_t index, const Buffer& uniformBuffer, 
            const std::shared_ptr<Texture>& texture, const std::shared_ptr<Sampler>& sampler);

        const std::vector<VkDescriptorSet>& getHandles() const { return m_handles; }
        std::size_t getSize() const;

    private:
        std::vector<VkDescriptorSet> m_handles;
        std::unique_ptr<DescriptorPool> m_pool;

        const Device& m_device;
    };
}