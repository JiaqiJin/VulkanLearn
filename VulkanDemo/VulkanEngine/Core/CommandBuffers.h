#pragma once
#include <vulkan/vulkan.h>
#include <cstddef>
#include <vector>
#include <memory>

namespace Rendering
{
	class Device;
	class CommandPool;
	class Queue;
	class Semaphore;
	class Fence;

	class CommandBuffersContainer
	{
	public:
		CommandBuffersContainer(const Device& device, const CommandPool& commandPool, size_t size);
		~CommandBuffersContainer();

		CommandBuffersContainer(const CommandBuffersContainer&) = delete;
		CommandBuffersContainer(CommandBuffersContainer&&) = delete;
		CommandBuffersContainer& operator=(const CommandBuffersContainer&) = delete;
		CommandBuffersContainer& operator=(CommandBuffersContainer&&) = delete;

		std::size_t getSize() const { return m_handles.size(); }
		const VkCommandBuffer& getHandle(std::size_t index) const { return m_handles[index]; }

	private:
		std::vector<VkCommandBuffer> m_handles;
		CommandPool const& m_commandPool;
		const Device& m_device;
	};

	class CommandBuffer
	{
	public:
		CommandBuffer(const std::shared_ptr<CommandBuffersContainer>& container, std::size_t index);

		const VkCommandBuffer& getHandle() const;
		void reset() const;
		void begin(bool oneTime = true) const;
		void end() const;
		void submit(const Queue& queue, const Semaphore* signalSemaphore, const Semaphore* waitSemaphore, const Fence* signalFence) const;

	private:
		std::shared_ptr<CommandBuffersContainer> m_container;
		std::size_t m_index;
	};
}