#pragma once

#include <vulkan/vulkan.h>
#include "UniqueHandle.h"
#include "../Objects/Object.h"
/*
* Wrapper class for VkDescriptorSetLayout 
* Tell vulkan this descriptor for vertex shader
*/
namespace Rendering
{
    class DescriptorSetLayout : public Object
    {
    public:
        explicit DescriptorSetLayout(const Application& app);
        ~DescriptorSetLayout();

        DescriptorSetLayout(const DescriptorSetLayout&) = default;
        DescriptorSetLayout(DescriptorSetLayout&&) = default;
        DescriptorSetLayout& operator=(const DescriptorSetLayout&) = default;
        DescriptorSetLayout& operator=(DescriptorSetLayout&&) = default;

        VkDescriptorSetLayout getHandle() const { return m_handle; }

    private:
        UniqueHandle<VkDescriptorSetLayout> m_handle;
    };
}