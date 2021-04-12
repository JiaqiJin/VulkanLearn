#include "Window.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

static Rendering::Window* getAppFromWindow(GLFWwindow* window) noexcept
{
    return reinterpret_cast<Rendering::Window*>(glfwGetWindowUserPointer(window));
}

namespace Rendering
{
    Window::Window(int width, int height, std::string const& title)
    {
        m_width = width;
        m_height = height;

        glfwInit();

        createWindow(title);
        glfwSetFramebufferSizeCallback(m_handle, Window::framebufferResizeCallback);
        queryRequiredInstanceExtensions();
    }

    Window::~Window()
    {
        glfwDestroyWindow(m_handle);
        glfwTerminate();
    }

    void Window::createWindow(std::string const& title)
    {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        m_handle = glfwCreateWindow(m_width, m_height, title.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(m_handle, this);
    }

    void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height) noexcept
    {
        if (auto app = ::getAppFromWindow(window))
            app->onFramebufferResized(width, height);
    }

    void Window::onFramebufferResized(int width, int height)
    {
        m_width = width;
        m_height = height;

        for (auto const& callback : m_resizeCallbacks)
        {
            if (callback)
                callback(width, height);
        }
    }

    void Window::queryRequiredInstanceExtensions()
    {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        m_requiredInstanceExtensions.assign(glfwExtensions, glfwExtensions + glfwExtensionCount);
    }

    void Window::pollEvents() const
    {
        glfwPollEvents();
    }

    bool Window::shouldClose() const
    {
        return glfwWindowShouldClose(m_handle);
    }

}