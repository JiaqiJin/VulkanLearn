#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <memory>

namespace RHI
{
	class Device;
	class CommandPool;

	class CommandBuffer
	{
	public:
		static const uint32_t MAX_INDIRECT_DRAW_COUNT = 256;

		enum class CBLevel
		{
			PRIMARY,
			SECONDARY,
			COUNT
		};

	public:
		CommandBuffer(const std::shared_ptr<Device> pDevice, const std::shared_ptr<CommandPool> pCmdPool, CBLevel level);

		~CommandBuffer();

	private:
		VkCommandBuffer m_commandBuffer;
		VkCommandBufferAllocateInfo m_info;

		std::shared_ptr<Device> m_pDevice;
		std::shared_ptr<CommandPool> m_pCommandPool;

		friend class CommandPool;
	};
}