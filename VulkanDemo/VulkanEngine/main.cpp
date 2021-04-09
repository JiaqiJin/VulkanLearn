#include "Application.h"

int main()
{
	try
	{
		Application app;
		app.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		std::getchar();

		return EXIT_FAILURE;
	}

	// temporary to catch Vulkan errors
	std::getchar();
	return EXIT_SUCCESS;
}