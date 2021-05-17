#pragma once

#include "Vulkan.h"

namespace Rendering
{
	class Device;

	class Fence
	{
	public:
		NON_COPIABLE(Fence)

		Fence(const class Device& device);
		~Fence();

		const Device& GetDevice() const
		{
			return device;
		}

		const VkFence& Get() const
		{
			return fence;
		}

		void Reset() const;
		void Wait(uint64_t timeout) const;
	private:
		const Device& device;
		VkFence fence{};
	};
}