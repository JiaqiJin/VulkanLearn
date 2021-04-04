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
	void createGraphicsPipelines();

	VRaii<VkShaderModule> createShaderModule(const std::vector<char>& code);
private:
	VContext vulkanContext;
	VUtility utility{ vulkanContext };

	QueueFamilyIndices queueFamilyIndices;

	vk::Device device;

	VkDevice graphicsDevice;
	VkPhysicalDevice physicalDevice;

	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	VRaii<VkSwapchainKHR> swapChain;
	VRaii<VkPipelineLayout> pipelineLayout;

private:
	void initialize()
	{
		createSwapChain();
		createGraphicsPipelines();
	}
};