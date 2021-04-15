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

	class Shader
	{
	public:
		Shader(const Device& device ,std::vector<ShaderKey> moduleKey);
		//CompiledShader compiler() const;
		std::vector<VkPipelineShaderStageCreateInfo> createShaderStageCreateInfo() const;
	private:
		void addShaders();
	private:
		const Device& m_device;
		std::vector<ShaderKey> m_moduleKey;
		std::vector<ShaderModule> m_shaderModules;
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