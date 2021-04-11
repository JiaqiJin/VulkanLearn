#include "Queue.h"
#include "QueueFamily.h"

namespace Rendering
{
	Queue::Queue(VkQueue handle, const QueueFamily& queueFamily)
		:m_handle(handle),
		m_family(queueFamily)
	{

	}

	void Queue::waitIdle() const
	{
		vkQueueWaitIdle(m_handle);
	}
}