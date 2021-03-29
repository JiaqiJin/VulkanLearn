#pragma once

#include "context.h"
#include "raii.h"
#include "VulkanUtil.h"

class VulkanRender
{
public:
	VulkanRender(GLFWwindow* window);

private:
	void createSwapChain();

private:
	VContext vulkanContext;
	VUtility utility{ vulkanContext };

	vk::Device device;

	VkDevice graphicsDevice;
	VkPhysicalDevice physicalDevice;

	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	VRaii<VkSwapchainKHR> swapChain;
};