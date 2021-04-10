#include "Queue.h"

namespace Rendering
{
    Queue::Queue(VkQueue handle, QueueFamily& queueFamily)
        : m_handle(handle)
        , m_family(queueFamily)
    {

    }

    void Queue::waitIdle() const
    {
        vkQueueWaitIdle(m_handle);
    }
}