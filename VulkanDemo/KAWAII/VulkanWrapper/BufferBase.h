#pragma once

#include <vulkan/vulkan.h>
#include "VKGPUSyncRes.h"
#include <memory>
#include <vector>

namespace RHI
{
	class Device;

	class BufferBase : public VKGPUSyncRes
	{
	public:
		BufferBase(const std::shared_ptr<Device>& pDevice);
		virtual ~BufferBase() = default;

		// Getters
		VkPipelineStageFlags GetAccessStages() const { return m_accessStages; }
		VkAccessFlags GetAccessFlags() const { return m_accessFlags; }
		//virtual const VkBufferCreateInfo& GetBufferInfo() const = 0;
		VkBufferCreateInfo GetBufferInfo() const { return m_info; }
		virtual VkBuffer GetDeviceHandle() const = 0;

		bool Init(const VkBufferCreateInfo& info);

	protected:
		VkBufferCreateInfo m_info;
		VkPipelineStageFlags m_accessStages;
		VkAccessFlags m_accessFlags;
	};
}