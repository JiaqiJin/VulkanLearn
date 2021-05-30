#include "ShaderModule.h"
#include "Device.h"

#include "../Common/Macro.h"
#include "../Common/Logger.h"

#include <fstream>

namespace RHI
{
	ShaderModule::ShaderModule(const std::shared_ptr<Device>& pDevice, const std::wstring& path, ShaderType type, const std::string& entryName)
		:m_device(pDevice), m_entryName(entryName), m_shaderType(type), m_shaderPath(path)
	{
		if (!Init(path))
			K_ERROR("Error Initialize ShaderModule");
	}

	bool ShaderModule::Init(const std::wstring& path)
	{
		// Loading a shader 
		std::ifstream ifs;
		ifs.open(path, std::ios::binary);
		if (ifs.fail())
			return false;

		std::vector<char> buffer;
		buffer.assign(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());

		VkShaderModuleCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		info.codeSize = buffer.size();
		info.pCode = (uint32_t*)buffer.data();
		CHECK_VK_ERROR(vkCreateShaderModule(m_device->GetDeviceHandle(), &info, nullptr, &m_shaderModule));

		switch (m_shaderType)
		{
		case ShaderTypeVertex: m_shaderStage = VK_SHADER_STAGE_VERTEX_BIT; break;
		case ShaderTypeTessellationControl: m_shaderStage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT; break;
		case ShaderTypeTessellationEvaluation: m_shaderStage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT; break;
		case ShaderTypeGeometry: m_shaderStage = VK_SHADER_STAGE_GEOMETRY_BIT; break;
		case ShaderTypeFragment: m_shaderStage = VK_SHADER_STAGE_FRAGMENT_BIT; break;
		case ShaderTypeCompute: m_shaderStage = VK_SHADER_STAGE_COMPUTE_BIT; break;
		default: ASSERTION(false);
		}

		return true;
	}

	ShaderModule::~ShaderModule()
	{
		vkDestroyShaderModule(m_device->GetDeviceHandle(), m_shaderModule, nullptr);
	}
}