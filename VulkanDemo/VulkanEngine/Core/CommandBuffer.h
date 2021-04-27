#pragma once

#include <vulkan/vulkan.h>
#include <memory>

namespace Rendering
{
    class CommandBuffers;
    class Semaphore;
    class Fence;
    class Queue;

    class CommandBuffer
    {
    public:
        CommandBuffer(const std::shared_ptr<CommandBuffers>& container, std::size_t index);

        VkCommandBuffer const& getHandle() const;

        void reset() const;
        void begin(bool oneTime = true) const;
        void end() const;
        void submit(Queue const& queue, Semaphore const* signalSemaphore, Semaphore const* waitSemaphore, Fence const* signalFence) const;

    private:
        std::shared_ptr<CommandBuffers> m_container;
        std::size_t m_index;
    };
}