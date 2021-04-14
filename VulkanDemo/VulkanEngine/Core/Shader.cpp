#include "Shader.h"
#include "ShaderModule.h"

#include <iterator>

namespace Rendering
{
	std::vector<VkPipelineShaderStageCreateInfo> CompiledShader::createShaderStageCreateInfo() const
	{
		std::vector<VkPipelineShaderStageCreateInfo> createInfo{};
		std::transform(m_shaderModules.begin(), m_shaderModules.end(), std::back_inserter(createInfo),
			[](ShaderModule const& shaderModule) { return shaderModule.createStageCreateInfo(); });
		return createInfo;
	}

	Shader::Shader(const Device& device, std::vector<ShaderKey> moduleKey)
		: m_device(device)
		, m_moduleKey(std::move(moduleKey))
	{
		
	}

	CompiledShader Shader::compiler() const
	{
		std::vector<ShaderModule> modules;
		for (const ShaderKey key : m_moduleKey)
		{
			modules.emplace_back(m_device, key);
		}
		return CompiledShader(std::move(modules));
	}

	ShaderBuilder& ShaderBuilder::addShader(Rendering::ShaderType type, const std::string& path, const std::string& entryPoint)
	{
		m_moduleKeys.push_back({ type , path, entryPoint });
		return*this;
	}

	std::unique_ptr<Shader> ShaderBuilder::buildShader(const Device& device)
	{
		return std::make_unique<Shader>(device, m_moduleKeys);
	}
}