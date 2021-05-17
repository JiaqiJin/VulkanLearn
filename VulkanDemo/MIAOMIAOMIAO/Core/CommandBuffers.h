#pragma once

#include "Vulkan.h"
#include <vector>

namespace Rendering
{
	class Device;
	class CommandPool;
	/*
	* Command Buffer : record all of the operations you want to perform
	*/
	class CommandBuffers  final
	{
	public:
		NON_COPIABLE(CommandBuffers)

		CommandBuffers(const CommandPool& commandPool, uint32_t size);
		~CommandBuffers();

		uint32_t Size() const
		{
			return static_cast<uint32_t>(commandBuffers.size());
		}

		const VkCommandBuffer& operator [](const size_t i) const
		{
			return commandBuffers[i];
		}

		const Device& GetDevice() const
		{
			return device;
		}

		// Starting command buffer recording
		VkCommandBuffer Begin(size_t i);
		// End buffer
		void End(size_t);

	private:
		const Device& device;
		const CommandPool& commandPool;
		std::vector<VkCommandBuffer> commandBuffers;
	};
}