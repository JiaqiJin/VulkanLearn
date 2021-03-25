#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan.hpp>

#include <vector>
#include <optional>

#include "raii.h"

struct GLFWwindow;

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily; // graphyc commands queue family

	bool isComplete()
	{
		return graphicsFamily.has_value();
	}

	static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

};

class VContext
{
public:
	VContext(GLFWwindow* window);
	~VContext();

	VContext(VContext&&) = delete;
	VContext& operator= (VContext&&) = delete;
	VContext(const VContext&) = delete;
	VContext& operator= (const VContext&) = delete;

private:
	static void DestroyDebugReportCallbackEXT(VkInstance instance, 
		VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);

	static VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);

	void createInstance();
	void setupDebugCallback();
	void pickPhysicalDevice();
	void findQueueFamilyIndices();
	void createLogicalDevice();

	void initVulkan()
	{
		createInstance();
		setupDebugCallback();
		pickPhysicalDevice();
		findQueueFamilyIndices();
		createLogicalDevice();
	}

private:
	GLFWwindow* window;

	VRaii<vk::Instance> instance;
	VRaii<vk::DebugReportCallbackEXT> callback;
	VRaii<vk::Device> graphics_device;
	VRaii<vk::SurfaceKHR> window_surface;

	VkPhysicalDevice physicalDevice;

	VkQueue graphicsQueue;

	QueueFamilyIndices queue_family_indices;
};