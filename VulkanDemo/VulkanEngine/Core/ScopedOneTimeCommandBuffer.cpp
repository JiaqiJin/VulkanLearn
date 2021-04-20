#include "ScopedOneTimeCommandBuffer.h"
#include "../Application.h"
#include "Device.h"
#include "CommandPool.h"
#include "Queue.h"

namespace Rendering
{
    ScopedOneTimeCommandBuffer::ScopedOneTimeCommandBuffer(Application const& app)
        : Object(app)
        , m_commandBuffer(getApp().getShortLivedCommandPool().createCommandBuffer())
    {
        m_commandBuffer.begin(true);
    }

    ScopedOneTimeCommandBuffer::~ScopedOneTimeCommandBuffer()
    {
        submit();
    }

    void ScopedOneTimeCommandBuffer::submit()
    {
        if (m_submitted)
            return;

        const Queue& queue = getApp().getDevice().getGraphicsQueue();

        m_commandBuffer.end();
        m_commandBuffer.submit(queue, nullptr, nullptr, nullptr);
        queue.waitIdle();

        m_submitted = true;
    }
}