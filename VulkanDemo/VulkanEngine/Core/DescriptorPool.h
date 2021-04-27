#pragma once

#include <vulkan/vulkan.h>
#include <cstddef>
#include "UniqueHandle.h"
#include "../Objects/Object.h"

namespace Rendering
{
    class DescriptorPool : public Object
    {
    public:
        explicit DescriptorPool(const Application& app, std::size_t size);
        ~DescriptorPool();

        DescriptorPool(const DescriptorPool&) = default;
        DescriptorPool(DescriptorPool&&) = default;
        DescriptorPool& operator=(const DescriptorPool&) = default;
        DescriptorPool& operator=(DescriptorPool&&) = default;

        VkDescriptorPool getHandle() const { return m_handle; }
        std::size_t getSize() const { return m_size; }

    private:
        UniqueHandle<VkDescriptorPool> m_handle;
        std::size_t m_size;
    };
}