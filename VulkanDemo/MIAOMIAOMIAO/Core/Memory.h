#pragma once

#include "Vulkan.h"

namespace Rendering
{
	class Device;

	class Memory
	{
	public:
		NON_COPIABLE(Memory);

		Memory(const Device& device, VkMemoryRequirements requirements, VkMemoryPropertyFlags properties);
		~Memory();

		void Fill(const void* data, size_t size) const;
		void UnMap() const;
		void* Map(size_t offset, size_t size) const;

		const VkDeviceMemory& Get() const
		{
			return memory;
		}

		const Device& GetDevice() const
		{
			return device;
		}

	private:
		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;

		const Device& device;
		VkDeviceMemory memory;
	};
}