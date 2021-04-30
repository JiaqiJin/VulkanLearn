#pragma once

#include "Vulkan.h"

#include <string>
#include <vector>

namespace Rendering
{
	class Device;

	class Shader
	{
	public:
		NON_COPIABLE(Shader);

		Shader(const Device& device, const std::string& filename);
		~Shader();

		VkShaderModule Get() const
		{
			return shader;
		}

		VkPipelineShaderStageCreateInfo CreateShaderStage(VkShaderStageFlagBits stage) const;

	private:
		static std::vector<char> Load(const std::string& filename);
		void CreateShaderModule(const std::vector<char>& code);

		const Device& device;
		VkShaderModule shader{};
	};
}