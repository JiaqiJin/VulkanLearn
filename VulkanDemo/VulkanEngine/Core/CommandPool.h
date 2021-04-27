#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "UniqueHandle.h"
#include "../Objects/Object.h"

/*
* Wrapper class for VkCommandPool 
* Command pool manage the memory that is used to store the buffers and command buffers are allocated from them
*/
namespace Rendering
{
    class Device;
    class QueueFamily;
    class CommandBuffer;
    class Queue;

    class CommandPool : Object
    {
    public:
        explicit CommandPool(Application const& app);
        ~CommandPool();

        CommandBuffer createCommandBuffer() const;
        std::vector<CommandBuffer> createCommandBuffers(std::size_t size) const;

        CommandPool(const CommandPool&) = default;
        CommandPool(CommandPool&&) = default;
        CommandPool& operator=(const CommandPool&) = default;
        CommandPool& operator=(CommandPool&&) = default;

        VkCommandPool getHandle() const { return m_handle; }
        const QueueFamily& getQueueFamily() const { return m_queueFamily; }

        void reset() const;

    private:
        UniqueHandle<VkCommandPool> m_handle;

        const QueueFamily& m_queueFamily;
    };
}