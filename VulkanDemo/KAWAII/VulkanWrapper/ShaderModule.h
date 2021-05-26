#pragma once

#include "DeviceObjectBase.h"

namespace RHI
{
	class ShaderModule : public DeviceObjectBase<ShaderModule>
	{
	public:
		enum ShaderType
		{
			ShaderTypeVertex,
			ShaderTypeTessellationControl,
			ShaderTypeTessellationEvaluation,
			ShaderTypeGeometry,
			ShaderTypeFragment,
			ShaderTypeCompute,
			ShaderTypeCount
		};

	public:
		ShaderModule(const std::shared_ptr<Device> pDevice, const std::wstring path, ShaderType type, const std::string entryName);
		~ShaderModule();

		VkShaderModule GetDeviceHandle() const { return m_shaderModule; }
		std::wstring GetShaderPath() const { return m_shaderPath; }
		ShaderType GetShaderType() const { return m_shaderType; }
		VkShaderStageFlagBits GetShaderStage() const { return m_shaderStage; }
		std::string GetEntryName() const { return m_entryName; }

	protected:
		bool Init(const std::shared_ptr<Device>& pDevice, const std::shared_ptr<ShaderModule>& pSelf, 
			const std::wstring& path, ShaderType type, const std::string& entryName);

	protected:
		VkShaderModule m_shaderModule;
		std::wstring m_shaderPath;
		ShaderType m_shaderType;
		VkShaderStageFlagBits m_shaderStage;
		std::string m_entryName;
	};
}