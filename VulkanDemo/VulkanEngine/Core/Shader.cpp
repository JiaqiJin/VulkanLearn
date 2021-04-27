#include "Shader.h"
#include "ShaderModule.h"

#include <iterator>

namespace Rendering
{

    std::vector<VkPipelineShaderStageCreateInfo> CompiledShader::createStageDescriptions() const
    {
        std::vector<VkPipelineShaderStageCreateInfo> stageDescriptions;
        std::transform(m_shaderModules.begin(), m_shaderModules.end(), std::back_inserter(stageDescriptions),
            [](ShaderModule const& shaderModule) { return shaderModule.createStageCreateInfo(); });
        return stageDescriptions;
    }

    Shader::Shader(const Application& app, std::vector<ShaderModule::Key> moduleKeys) : Object(app), m_moduleKeys(std::move(moduleKeys))
    {

    }

    CompiledShader Shader::compile() const
    {
        std::vector<ShaderModule> modules;
        for (ShaderModule::Key const& key : m_moduleKeys)
            modules.emplace_back(getApp(), key);
        return CompiledShader(std::move(modules));
    }

    ShaderBuilder& ShaderBuilder::addStage(ShaderModule::Type type, const std::string& path, std::string entryPoint)
    {
        m_moduleKeys.push_back({ type, path, entryPoint });
        return *this;
    }

    std::unique_ptr<Shader> ShaderBuilder::build(const Application& app) const
    {
        return std::make_unique<Shader>(app, m_moduleKeys);
    }
}