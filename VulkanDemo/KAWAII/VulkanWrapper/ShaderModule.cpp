#include "ShaderModule.h"
#include "Device.h"

#include "../Common/Macro.h"
#include "../Common/Logger.h"

#include <fstream>

namespace RHI
{
	ShaderModule::ShaderModule(const std::shared_ptr<Device> pDevice, const std::wstring path, ShaderType type, const std::string entryName)
		:m_device(pDevice)
	{
		if (!Init(m_device, path, type, entryName))
			K_ERROR("Error Initialize ShaderModule");
	}

	bool ShaderModule::Init(const std::shared_ptr<Device>& pDevice, const std::wstring& path, ShaderType type, const std::string& entryName)
	{
		// Loading a shader 
		std::ifstream ifs;
		ifs.open(path, std::ios::binary);
		if (ifs.fail())
			return false;

		std::vector<char> buffer;
		buffer.assign(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());

		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = buffer.size();
		createInfo.pCode = (uint32_t*)buffer.data(); // The bytecode pointer is a uint32_t

		CHECK_VK_ERROR(vkCreateShaderModule(pDevice->GetDeviceHandle(), &createInfo, nullptr, &m_shaderModule));

		m_shaderType = type;

		switch (type)
		{
		case ShaderTypeVertex: m_shaderStage = VK_SHADER_STAGE_VERTEX_BIT; break;
		case ShaderTypeTessellationControl: m_shaderStage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT; break;
		case ShaderTypeTessellationEvaluation: m_shaderStage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT; break;
		case ShaderTypeGeometry: m_shaderStage = VK_SHADER_STAGE_GEOMETRY_BIT; break;
		case ShaderTypeFragment: m_shaderStage = VK_SHADER_STAGE_FRAGMENT_BIT; break;
		case ShaderTypeCompute: m_shaderStage = VK_SHADER_STAGE_COMPUTE_BIT; break;
		default: ASSERTION(false);
		}

		m_entryName = entryName;

		return true;
	}

	ShaderModule::~ShaderModule()
	{
		vkDestroyShaderModule(m_device->GetDeviceHandle(), m_shaderModule, nullptr);
	}
}