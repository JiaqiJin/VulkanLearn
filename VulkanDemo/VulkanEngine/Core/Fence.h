#pragma once

#include <vulkan/vulkan.h>
#include "UniqueHandle.h"

namespace Rendering
{
    class Device;

    class Fence 
    {
    public:
        Fence(const Device& device);
        ~Fence();

        Fence(const Fence&) = default;
        Fence(Fence&&) = default;
        Fence& operator=(const Fence&) = default;
        Fence& operator=(Fence&&) = default;

        void wait() const;
        void reset() const;
        bool isSignaled() const;

        VkFence getHandle() const { return m_handle; }

    private:
        UniqueHandle<VkFence> m_handle;

        const Device& m_device;
    };
}