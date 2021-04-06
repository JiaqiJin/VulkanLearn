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
	void createSwapChainImageViews();
	void createRenderPasses();
	void createGraphicsPipelines();
	void createFrameBuffers();

	VRaii<VkShaderModule> createShaderModule(const std::vector<char>& code);
private:
	VContext vulkanContext;
	VUtility utility{ vulkanContext };

	QueueFamilyIndices queueFamilyIndices;

	vk::Device device;

	VkDevice graphicsDevice;
	VkPhysicalDevice physicalDevice;

	std::vector<VkImage> swapChainImages;
	std::vector<VkImageView> swapChainImageViews;
	std::vector<VRaii<vk::Framebuffer>> swapChainFramebuffers;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	VRaii<VkSwapchainKHR> swapChain;
	VRaii<VkPipelineLayout> pipelineLayout;

	VRaii<vk::RenderPass> renderPass;
	VRaii<VkPipelineLayout> pipelineLayout;

private:
	void initialize()
	{
		createSwapChain();
		createSwapChainImageViews();
		createRenderPasses();
		createGraphicsPipelines();
		createFrameBuffers();
	}
};