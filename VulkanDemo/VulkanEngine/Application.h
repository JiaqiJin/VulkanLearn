#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <optional>

#include "Window.h"

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

class Application
{
public:
	Application()
	{
		m_window = std::make_unique<Rendering::Window>(WIDTH, HEIGHT, "Vulkan Demo");
	}

	void run()
	{
		m_window->startEventLoop([this]() { drawFrame(); });
	}

private:
	void drawFrame()
	{

	}

private:
	std::unique_ptr<Rendering::Window> m_window;
};