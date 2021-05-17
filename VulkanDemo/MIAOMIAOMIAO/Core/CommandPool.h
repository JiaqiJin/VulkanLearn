#pragma once

#include "Vulkan.h"

namespace Rendering
{
	class Device;

	class CommandPool final
	{
	public:
		NON_COPIABLE(CommandPool)

		CommandPool(const Device& device, uint32_t familyIndex);
		~CommandPool();

		VkCommandPool Get() const
		{
			return commandPool;
		}

		const Device& GetDevice() const
		{
			return device;
		}

	private:
		const Device& device;
		VkCommandPool commandPool{};
	};
}