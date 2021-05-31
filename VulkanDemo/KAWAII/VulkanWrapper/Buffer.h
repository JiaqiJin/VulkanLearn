#pragma once

#include <vulkan/vulkan.h>

#include "BufferBase.h"

#include <memory>
#include <vector>

namespace RHI
{
	class Buffer : public BufferBase
	{
	public:
		Buffer(const std::shared_ptr<Device>& pDevice, const VkBufferCreateInfo& info, uint32_t memoryPropertyFlag);

		virtual ~Buffer();

	protected:
		VkBuffer m_buffer = 0;

	};
}