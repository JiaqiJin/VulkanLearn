#pragma once

#include <vulkan/vulkan.h>
#include "UniqueHandle.h"
#include "../Objects/Object.h"
/*
* Wrapper class for VkPipelineLayout
* During the pipeline creation which descriptor in the shader will be using
*/
namespace Rendering
{
    class DescriptorSetLayout;

    class PipelineLayout : public Object
    {
    public:
        explicit PipelineLayout(const Application& app, const DescriptorSetLayout& descriptorSetLayout);
        ~PipelineLayout();

        PipelineLayout(const PipelineLayout&) = default;
        PipelineLayout(PipelineLayout&&) = default;
        PipelineLayout& operator=(const PipelineLayout&) = default;
        PipelineLayout& operator=(PipelineLayout&&) = default;

        VkPipelineLayout getHandle() const { return m_handle; }

    private:
        UniqueHandle<VkPipelineLayout> m_handle;
    };
}