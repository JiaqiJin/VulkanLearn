#pragma once

#include <vulkan/vulkan.h>

namespace Rendering
{
	class QueueFamily;

	class Queue
	{
	public:
		Queue(VkQueue handle, const QueueFamily& queueFamily);

		VkQueue getHandle() const { return m_handle; }
		const QueueFamily& getFamily() const { return m_family; }

		void waitIdle() const;
	private:
		VkQueue m_handle = VK_NULL_HANDLE;
		const QueueFamily& m_family;
	};
}