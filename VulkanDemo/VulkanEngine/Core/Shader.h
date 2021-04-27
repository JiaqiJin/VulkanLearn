#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include "ShaderModule.h"
#include "../Objects/Object.h"

struct VkPipelineShaderStageCreateInfo;

namespace Rendering
{
    class CompiledShader
    {
    public:
        CompiledShader(std::vector<ShaderModule> shaderModules) : m_shaderModules(std::move(shaderModules)) {}
        std::vector<VkPipelineShaderStageCreateInfo> createStageDescriptions() const;

    private:
        std::vector<ShaderModule> m_shaderModules;
    };

    class Shader : Object
    {
    public:
        Shader(const Application& app, std::vector<ShaderModule::Key> moduleKeys);

        CompiledShader compile() const;

    private:
        std::vector<ShaderModule::Key> m_moduleKeys;
    };

    class ShaderBuilder
    {
    public:
        ShaderBuilder& addStage(ShaderModule::Type type, const std::string& path, std::string entryPoint = "main");
        std::unique_ptr<Shader> build(const Application& app) const;

    private:
        std::vector<ShaderModule::Key> m_moduleKeys;
    };
}