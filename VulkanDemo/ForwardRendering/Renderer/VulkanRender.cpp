#include "VulkanRender.h"


VulkanRender::VulkanRender(GLFWwindow* window)
	:vulkanContext(window)
{
	physicalDevice = vulkanContext.getPhysicalDevice();
}

void VulkanRender::createSwapChain()
{
	auto swapChainSupport = SwapChainSupportDetails::querySwapChainSupport(physicalDevice, vulkanContext.getWindowSurface());

	VkSurfaceFormatKHR surfaceFormat = utility.chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = utility.chooseSwapPresentMode(swapChainSupport.present_modes);
	VkExtent2D extent = utility.chooseSwapExtent(swapChainSupport.capabilities);

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) 
	{
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = vulkanContext.getWindowSurface();
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = QueueFamilyIndices::findQueueFamilies(physicalDevice, vulkanContext.getWindowSurface());
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };


	if (indices.graphicsFamily != indices.presentFamily) 
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else 
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0; // Optional
		createInfo.pQueueFamilyIndices = nullptr; // Optional
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform; // not doing any transformation
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; // ignore alpha channel (for blending with other windows)

	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;

	//createInfo.oldSwapchain = VK_NULL_HANDLE;
	auto old_swapChain = std::move(swapChain);
	createInfo.oldSwapchain = old_swapChain.get();

	VkSwapchainKHR tempSwapChain;
	if (vkCreateSwapchainKHR(graphicsDevice, &createInfo, nullptr, &tempSwapChain) != VK_SUCCESS) 
	{
		throw std::runtime_error("failed to create swap chain!");
	}

	swapChain = VRaii<VkSwapchainKHR>(
		tempSwapChain,
		[device = this->device](auto& obj){device.destroySwapchainKHR(obj); }
	);

	uint32_t image_cout;
	vkGetSwapchainImagesKHR(device, swapChain.get(), &image_cout, nullptr);
	swapChainImages.resize(image_cout);
	vkGetSwapchainImagesKHR(device, swapChain.get(), &image_cout, swapChainImages.data());

	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;
}