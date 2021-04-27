#pragma once

#include <vulkan/vulkan.h>
#include "Core/UniqueHandle.h"

namespace Rendering
{
    class Instance;
    class Window;

    class VulkanSurfaceCreator
    {
    public:
        static UniqueHandle<VkSurfaceKHR> createVulkanSurface(const Instance& instance, const Window& window);
    };
}