#pragma once

#include <vulkan/vulkan.h>

#include "CommandBuffers.h"
#include "../Objects/Object.h"

namespace Rendering
{
    class ScopedOneTimeCommandBuffer : Object
    {
    public:
        ScopedOneTimeCommandBuffer(const Application& app);
        ~ScopedOneTimeCommandBuffer();

        ScopedOneTimeCommandBuffer(const ScopedOneTimeCommandBuffer&) = delete;
        ScopedOneTimeCommandBuffer(ScopedOneTimeCommandBuffer&&) = delete;
        ScopedOneTimeCommandBuffer& operator=(const ScopedOneTimeCommandBuffer&) = delete;
        ScopedOneTimeCommandBuffer& operator=(ScopedOneTimeCommandBuffer&&) = delete;

        VkCommandBuffer const& getHandle() const { return m_commandBuffer.getHandle(); }

        void submit();

    private:
        CommandBuffer m_commandBuffer;

        bool m_submitted = false;
    };
}