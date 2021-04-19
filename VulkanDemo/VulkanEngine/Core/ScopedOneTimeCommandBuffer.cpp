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
        
    }

    ScopedOneTimeCommandBuffer::~ScopedOneTimeCommandBuffer()
    {

    }
}