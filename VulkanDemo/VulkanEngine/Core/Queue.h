#pragma once

#include <vulkan/vulkan.h>

namespace Rendering
{
	class QueueFamily;

	class Queue
	{
	public:
		Queue(VkQueue handle, QueueFamily& family);

		VkQueue const& getHandle() const { return m_handle; }
		QueueFamily const& getFamily() const { return m_family; }

		void waitIdle() const;

	private:
		VkQueue m_handle = VK_NULL_HANDLE;
		QueueFamily& m_family;
	};
}