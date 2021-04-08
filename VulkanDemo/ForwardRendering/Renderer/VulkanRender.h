#pragma once

#include "context.h"
#include "raii.h"
#include "VulkanUtil.h"

class VulkanRender
{
public:
	VulkanRender(GLFWwindow* window);
	void Draw(float deltatime);
	
private:
	void createSwapChain();
	void createSwapChainImageViews();
	void createRenderPasses();
	void createGraphicsPipelines();
	void createFrameBuffers();
	void createGraphicsCommandBuffers();
	void createSemaphores();

	void drawFrame();
	VRaii<VkShaderModule> createShaderModule(const std::vector<char>& code);
private:
	VContext vulkanContext;
	VUtility utility{ vulkanContext };

	QueueFamilyIndices queueFamilyIndices;

	vk::Device device;

	VkDevice graphicsDevice;
	VkPhysicalDevice physicalDevice;
	vk::Queue graphicsQueue;
	vk::Queue presentQueue;

	std::vector<VkImage> swapChainImages;
	std::vector<VRaii<vk::ImageView>> swapChainImageViews;
	std::vector<VRaii<vk::Framebuffer>> swapChainFramebuffers;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	VRaii<VkSwapchainKHR> swapChain;
	VRaii<VkPipeline> graphicsPipeline;
	VRaii<VkPipelineLayout> pipelineLayout;
	VRaii<vk::RenderPass> renderPass;

	vk::CommandPool graphicsCommandPool;
	std::vector<VkCommandBuffer> commandBuffers;

	VRaii<vk::Semaphore> imageAvailableSemaphore;
	VRaii<vk::Semaphore> renderFinishedSemaphore;
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