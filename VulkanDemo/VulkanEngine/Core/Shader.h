#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include "ShaderModule.h"

struct VkPipelineShaderStageCreateInfo;

namespace Rendering
{
	class Device;

	class CompiledShader
	{
	public:
		CompiledShader(std::vector<ShaderModule> shaderModules)
			: m_shaderModules(std::move(shaderModules)) {}

		std::vector<VkPipelineShaderStageCreateInfo> createShaderStageCreateInfo() const;

	private:
		std::vector<ShaderModule> m_shaderModules;
	};

	class Shader
	{
	public:
		Shader(const Device& device ,std::vector<ShaderKey> moduleKey);
		CompiledShader compiler() const;

	private:
		const Device& m_device;
		std::vector<ShaderKey> m_moduleKey;
	};

	class ShaderBuilder
	{
	public:
		ShaderBuilder& addShader(Rendering::ShaderType type, const std::string& path, const std::string& entryPoint = "main");
		std::unique_ptr<Shader> buildShader(const Device& device);
	private:
		std::vector<ShaderKey> m_moduleKeys;
	};
}