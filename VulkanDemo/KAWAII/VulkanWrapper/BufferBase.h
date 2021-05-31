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
		BufferBase(const std::shared_ptr<Device>& pDevice, const VkBufferCreateInfo& info);
		virtual ~BufferBase() = default;

		// Getters
		VkPipelineStageFlags GetAccessStages() const { return m_accessStages; }
		VkAccessFlags GetAccessFlags() const { return m_accessFlags; }
		const VkBufferCreateInfo& GetBufferInfo() const { return m_info; }

	protected:
		VkBufferCreateInfo m_info;
		VkPipelineStageFlags m_accessStages;
		VkAccessFlags m_accessFlags;
	};
}