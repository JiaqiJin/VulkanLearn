#include "CommandBuffers.h"
#include "Device.h"
#include "CommandPool.h"

namespace Rendering
{
	CommandBuffersContainer::CommandBuffersContainer(const Device& device, const CommandPool& commandPool, size_t size)
		: m_device(device)
		, m_commandPool(commandPool)
	{
		VkCommandBufferAllocateInfo commandBufferAllocateInfo{};
		commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		commandBufferAllocateInfo.commandPool = m_commandPool.getHandle();
		commandBufferAllocateInfo.commandBufferCount = static_cast<uint32_t>(size);

		m_handles.resize(size);
		vkAllocateCommandBuffers(m_device.getHandle(), &commandBufferAllocateInfo, m_handles.data());
	}

	CommandBuffersContainer::~CommandBuffersContainer()
	{
		vkFreeCommandBuffers(m_device.getHandle(), m_commandPool.getHandle(), static_cast<uint32_t>(m_handles.size()), m_handles.data());
	}

	// Command Buffer
	CommandBuffer::CommandBuffer(const std::shared_ptr<CommandBuffersContainer>& container, std::size_t index)
		: m_container(container)
		, m_index(index)
	{

	}

	const VkCommandBuffer& CommandBuffer::getHandle() const
	{
		return m_container->getHandle(m_index);
	}
}