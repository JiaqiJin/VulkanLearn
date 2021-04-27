#include "Window.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

static Rendering::Window* getAppFromWindow(GLFWwindow* window) noexcept
{
    return reinterpret_cast<Rendering::Window*>(glfwGetWindowUserPointer(window));
}

namespace Rendering
{
    Window::Action getAction(int glfwAction)
    {
        static std::vector<Window::Action> const actions = []() {
            std::vector<Window::Action> result;
            result.resize(3);
            result[GLFW_RELEASE] = Window::Action::Release;
            result[GLFW_PRESS] = Window::Action::Press;
            result[GLFW_REPEAT] = Window::Action::Repeat;
            return result;
        }();

        return actions[static_cast<std::size_t>(glfwAction)];
    }

    Window::Modifiers getModifiers(int mods)
    {
        Window::Modifiers modifiers = Window::Modifiers::None;

        if (mods & GLFW_MOD_CONTROL)
            modifiers = modifiers | Window::Modifiers::Ctrl;
        if (mods & GLFW_MOD_SHIFT)
            modifiers = modifiers | Window::Modifiers::Shift;
        if (mods & GLFW_MOD_ALT)
            modifiers = modifiers | Window::Modifiers::Alt;

        return modifiers;
    }

    Window::Window(int width, int height, std::string const& title)
    {
        m_width = width;
        m_height = height;

        glfwInit();

        createWindow(title);
        glfwSetFramebufferSizeCallback(m_handle, Window::framebufferResizeCallback);
        glfwSetKeyCallback(m_handle, Window::keyCallback);
        glfwSetMouseButtonCallback(m_handle, Window::mouseButtonCallback);
        glfwSetCursorPosCallback(m_handle, Window::cursorPositionCallback);
        queryRequiredInstanceExtensions();
    }

    Window::~Window()
    {
        glfwDestroyWindow(m_handle);
        glfwTerminate();
    }

    void Window::waitUntilInForeground() const
    {
        int width = 0, height = 0;
        glfwGetFramebufferSize(m_handle, &width, &height);
        while (width == 0 || height == 0)
        {
            glfwGetFramebufferSize(m_handle, &width, &height);
            glfwWaitEvents();
        }
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

    void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) noexcept
    {
        if (auto app = ::getAppFromWindow(window))
            app->onKey(key, scancode, action, mods);
    }

    void Window::onKey(int glfwKey, int, int glfwAction, int glfwMods)
    {
        Action action = getAction(glfwAction);

        if (action == Action::Repeat)
            return;

        char c = '\0';
        Key key = Key::Unknown;
        if (glfwKey >= 0 && glfwKey < std::numeric_limits<char>::max())
        {
            key = Key::Char;
            c = static_cast<char>(glfwKey);
        }

        Modifiers mods = getModifiers(glfwMods);

        for (auto const& callback : m_keyCallbacks)
            if (callback)
                callback(action, key, c, mods);
    }

    void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) noexcept
    {
        if (auto app = ::getAppFromWindow(window))
            app->onMouseButton(button, action, mods);
    }

    void Window::onMouseButton(int glfwButton, int glfwAction, int)
    {
        if (m_canCaptureCursor && glfwButton == GLFW_MOUSE_BUTTON_LEFT)
        {
            Action action = getAction(glfwAction);

            m_cursorCaptured = action == Action::Press;
        }
    }

    void Window::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) noexcept
    {
        if (auto app = ::getAppFromWindow(window))
            app->onCursorPosition(xpos, ypos);
    }

    void Window::onCursorPosition(double xpos, double ypos)
    {
        glm::vec2 pos = { static_cast<float>(xpos), static_cast<float>(ypos) };
        glm::vec2 delta = pos - m_lastCursorPosition;
        m_lastCursorPosition = pos;

        if (m_cursorCaptured)
        {
            for (auto const& callback : m_mouseMoveCallbacks)
                if (callback)
                    callback(delta);
        }
    }

    bool Window::shouldClose() const
    {
        return glfwWindowShouldClose(m_handle);
    }

}