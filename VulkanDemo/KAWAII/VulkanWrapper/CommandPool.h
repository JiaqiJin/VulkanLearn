#pragma once

#include <vulkan/vulkan.h>
#include "PhysicalDevice.h"
#include "CommandBuffer.h"
#include <memory>
#include <vector>

namespace RHI
{
	class Fence;	
	class Device;
	class CommandBuffer;
	// VkCommandPool wrapper class, Command pools manage the memory that is used to store the buffers
	// and command buffers are allocated from them. 
	// Each command pool can only allocate command buffer that are submitted on a single type of queue
	class CommandPool : public std::enable_shared_from_this<CommandPool>
	{
	public:
		enum class CBPersistancy
		{
			PERSISTANT,
			TRANSIENT,
			COUNT
		};

	public:
		CommandPool(const std::shared_ptr<Device> pDevice, PhysicalDevice::QueueFamily queueFamily, CBPersistancy persistancy);
		~CommandPool();

		std::shared_ptr<CommandBuffer> AllocateCommandBuffer(CommandBuffer::CBLevel level);
		std::vector<std::shared_ptr<CommandBuffer>> AllocateCommandBuffers(CommandBuffer::CBLevel level, uint32_t count);
		void Reset(const std::shared_ptr<Fence>& pFence);

		// Getters
		VkCommandPool GetDeviceHandle() const { return m_commandPool; }
		const VkCommandPoolCreateInfo& GetInfo() const { return m_info; }

	private:
		bool Init(PhysicalDevice::QueueFamily queueFamily, CBPersistancy persistancy);

	private:
		std::shared_ptr<Device> m_pDevice;
		VkCommandPool m_commandPool;
		VkCommandPoolCreateInfo m_info;
	};
}