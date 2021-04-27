#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <memory>
#include <cstddef>
#include "UniqueHandle.h"
#include "../Objects/Object.h"

namespace Rendering
{
    class DescriptorPool;
    class DescriptorSetLayout;
    class Buffer;
    class Texture;
    class Sampler;

    class DescriptorSets : public Object
    {
    public:
        explicit DescriptorSets(const Application& app, std::size_t size, const DescriptorSetLayout& layout);
        ~DescriptorSets();

        DescriptorSets(const DescriptorSets&) = delete;
        DescriptorSets(DescriptorSets&&) = delete;
        DescriptorSets& operator=(const DescriptorSets&) = delete;
        DescriptorSets& operator=(DescriptorSets&&) = delete;

        void update(std::size_t index, const Buffer& uniformBuffer, const std::shared_ptr<Texture>& texture, const std::shared_ptr<Sampler>& sampler);

        std::vector<VkDescriptorSet> const& getHandles() const { return m_handles; }
        std::size_t getSize() const;

    private:
        std::unique_ptr<DescriptorPool> m_pool;
        std::vector<VkDescriptorSet> m_handles;
    };
}