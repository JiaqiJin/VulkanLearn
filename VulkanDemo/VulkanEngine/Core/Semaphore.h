#pragma once

#include <vulkan/vulkan.h>
#include "UniqueHandle.h"
#include "../Objects/Object.h"

namespace Rendering
{
    class Semaphore : public Object
    {
    public:
        Semaphore(Application const& app);
        ~Semaphore();

        Semaphore(const Semaphore&) = default;
        Semaphore(Semaphore&&) = default;
        Semaphore& operator=(const Semaphore&) = default;
        Semaphore& operator=(Semaphore&&) = default;

        VkSemaphore getHandle() const { return m_handle; }

    private:
        UniqueHandle<VkSemaphore> m_handle;
    };
}