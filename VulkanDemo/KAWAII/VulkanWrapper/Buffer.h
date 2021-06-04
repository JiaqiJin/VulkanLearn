#pragma once

#include <vulkan/vulkan.h>

#include "BufferBase.h"
#include "DeviceMemoryManager.h"

#include <memory>
#include <vector>

namespace RHI
{
	class Buffer : public BufferBase
	{
	public:
		Buffer(const std::shared_ptr<Device>& pDevice, uint32_t memoryPropertyFlag);

		virtual ~Buffer();

		VkBuffer GetDeviceHandle() const override { return m_buffer; }

		virtual VkMemoryRequirements GetMemoryReqirments() const;

		void BindMemory(VkDeviceMemory memory, uint32_t offset) const;

	protected:
		VkBuffer m_buffer = 0;

	};
}