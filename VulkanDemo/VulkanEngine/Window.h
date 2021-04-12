#pragma once

#include <string>
#include <vector>
#include <functional>

struct GLFWwindow;

namespace Rendering
{
	class Window
	{
	public:
        Window(int width, int height, std::string const& title);
        ~Window();

        Window(Window const&) = delete;
        Window(Window&&) = delete;
        Window& operator=(Window const&) = delete;
        Window& operator=(Window&&) = delete;

        GLFWwindow* getHandle() const { return m_handle; }

        int getWidth() const { return m_width; }
        int getHeight() const { return m_height; }

        std::vector<char const*> const& getRequiredInstanceExtensions() const { return m_requiredInstanceExtensions; }

        template <typename Func>
        void startEventLoop(Func&& onUpdate)
        {
            while (!shouldClose())
            {
                pollEvents();
                onUpdate();
            }
        }

        template<typename Func>
        void addResizeCallback(Func&& callback)
        {
            m_resizeCallbacks.emplace_back(std::forward<Func>(callback));
        }

    private:
        void createWindow(std::string const& title);

        static void framebufferResizeCallback(GLFWwindow* window, int width, int height) noexcept;
        void onFramebufferResized(int width, int height);
        void queryRequiredInstanceExtensions();

        bool shouldClose() const;
        void pollEvents() const;

    private:
        GLFWwindow* m_handle = nullptr;

        int m_width = -1;
        int m_height = -1;

        std::vector<char const*> m_requiredInstanceExtensions;

        std::vector<std::function<void(int, int)>> m_resizeCallbacks;
    };
}