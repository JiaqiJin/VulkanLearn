#pragma once

#include <vulkan/vulkan.h>
#include <string>
#include "UniqueHandle.h"

namespace Rendering
{
	class Device;

	enum class ShaderType
	{
		Vertex,
		Geometry,
		Fragment,
	};
	
	struct ShaderKey
	{
		ShaderType type = ShaderType::Vertex;
		std::string path;
		std::string entryPoint;
	};

	class ShaderModule
	{
	public:
		ShaderModule(const Device& device, const ShaderKey& key);
		~ShaderModule();

		ShaderModule(ShaderModule&&) = default;
		ShaderModule& operator=(ShaderModule&&) = default;

		// Hepler function use for assigning to a specific pipeline stage
		VkPipelineShaderStageCreateInfo createStageCreateInfo() const;

		VkShaderModule getHandle() const { return m_handle; }
	private:
		UniqueHandle<VkShaderModule> m_handle;
		const Device& m_device;

		ShaderKey m_key;
	};
}