#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "UniqueHandle.h"

/*
* Wrapper class for VkCommandPool 
* Command pool manage the memory that is used to store the buffers and command buffers are allocated from them
*/
namespace Rendering
{
	class Device;
	class QueueFamily;
	class Queue;
	class CommandBuffer;
	struct SwapChainSupportDetails;

	class CommandPool
	{
	public:
		CommandPool(const Device& device, const SwapChainSupportDetails& detail);
		~CommandPool();

		CommandPool(const CommandPool&) = default;
		CommandPool(CommandPool&&) = default;
		CommandPool& operator=(const CommandPool&) = default;
		CommandPool& operator=(CommandPool&&) = default;

		CommandBuffer createCommandBuffer() const;

		VkCommandPool getHandle() const { return m_handle; }
	private:
		std::vector<CommandBuffer> createCommandBuffers(size_t size) const;

	private:
		UniqueHandle<VkCommandPool> m_handle;

		const Device& m_device;
	};
}