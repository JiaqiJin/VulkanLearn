#pragma once

#include <vulkan/vulkan.h>
#include <string>
#include "UniqueHandle.h"
#include "../Objects/Object.h"

namespace Rendering
{
    class ShaderModule : Object
    {
    public:
        enum class Type
        {
            Vertex,
            Geometry,
            Fragment,
        };

        struct Key
        {
            Type type = Type::Vertex;
            std::string path;
            std::string entryPoint;
        };

    public:
        explicit ShaderModule(const Application& app, const Key& key);
        ~ShaderModule();

        ShaderModule(ShaderModule&&) = default;
        ShaderModule& operator=(ShaderModule&&) = default;

        VkPipelineShaderStageCreateInfo createStageCreateInfo() const;

        VkShaderModule getHandle() const { return m_handle; }

    private:
        UniqueHandle<VkShaderModule> m_handle;

        Key m_key;
    };
}