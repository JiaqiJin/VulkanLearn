#include <iostream>

#include "Core/Window.h"

using namespace Rendering;

int main()
{
	Window window(800, 800, "kawaii");

	while (true)
	{
		window.Run();
	}

	return 0;
}

// Device, RasterizerPipeline