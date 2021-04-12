#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <optional>

#include "Window.h"
#include "Application.h"

#ifdef _DEBUG
bool const VALIDATION_ENABLED = true;
#else
bool const VALIDATION_ENABLED = false;
#endif

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

class WindowApplication
{
public:
	WindowApplication()
	{
		m_window = std::make_unique<Rendering::Window>(WIDTH, HEIGHT, "Vulkan Demo");
		m_window->addResizeCallback([this](int, int) { onFramebufferResized(); });
		m_application = std::make_unique<Rendering::Application>("Vulkan demo", VALIDATION_ENABLED, *m_window);
	}

	void run()
	{
		m_window->startEventLoop([this]() { drawFrame(); });
	}

	const Rendering::Application& getApp() { return *m_application; }

private:
	void drawFrame()
	{

	}

	void onFramebufferResized()
	{
		m_application->onSurfaceChanged();
	}

private:
	std::unique_ptr<Rendering::Window> m_window;

	std::unique_ptr<Rendering::Application> m_application;
};