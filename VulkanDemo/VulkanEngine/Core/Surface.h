#pragma once

#include <vulkan/vulkan.h>
#include "UniqueHandle.h"

/*
* Wrapper class Vulkan surface 
* VkSurfaceKHR represents an abstrat type of surface to present the redered images.
*/
namespace Rendering
{
    class Instance;
    class Window;

    class Surface
    {
    public:
        explicit Surface(const Instance& instance, const Window& window);
        ~Surface();

        Surface(const Surface&) = default;
        Surface(Surface&&) = default;
        Surface& operator=(const Surface&) = default;
        Surface& operator=(Surface&&) = default;

        VkSurfaceKHR getHandle() const { return m_handle; }

        int getWidth() const;
        int getHeight() const;

    private:
        UniqueHandle<VkSurfaceKHR> m_handle;

        const Instance& m_instance;
        const Window& m_window;
    };
}