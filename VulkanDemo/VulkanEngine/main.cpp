#include "WindowApplication.h"
// https://gavinkg.github.io/ILearnVulkanFromScratch-CN/
// https://gpuopen.com/learn/understanding-vulkan-objects/
int main()
{
	try
	{
		WindowApplication app;
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