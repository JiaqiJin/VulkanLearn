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
	std::optional<uint32_t> presentFamily;	// QueueFamily for the presentation of surface.

	bool isComplete()
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}

	static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> present_modes;

	static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
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

	auto getQueueFamilyIndices() const
	{
		return queue_family_indices;
	}

	VkPhysicalDevice getPhysicalDevice() const
	{
		return physicalDevice;
	}

	const VkPhysicalDeviceProperties& getPhysicalDeviceProperties() const
	{
		return physicalDeviceProperties;
	}

	vk::Device getDevice() const
	{
		return graphicsDevice.get();
	}

	VkQueue getGraphicsQueue() const
	{
		return graphicsQueue;
	}

	VkQueue getPresentQueue() const
	{
		return presentQueue;
	}

	VkSurfaceKHR getWindowSurface() const
	{
		return windowSurface.get();
	}

private:
	static void DestroyDebugReportCallbackEXT(VkInstance instance, 
		VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);

	static VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);

	void createInstance();
	void setupDebugCallback();
	void createWindowSurface();
	void pickPhysicalDevice();
	void findQueueFamilyIndices();
	void createLogicalDevice();

	void initVulkan()
	{
		createInstance();
		setupDebugCallback();
		createWindowSurface();
		pickPhysicalDevice();
		findQueueFamilyIndices();
		createLogicalDevice();
	}

private:
	GLFWwindow* window;

	VRaii<vk::Instance> instance;
	VRaii<vk::DebugReportCallbackEXT> callback;
	VRaii<vk::Device> graphicsDevice;
	VRaii<vk::SurfaceKHR> windowSurface;

	VkPhysicalDevice physicalDevice;
	VkPhysicalDeviceProperties physicalDeviceProperties;

	VkQueue graphicsQueue;
	VkQueue presentQueue;

	QueueFamilyIndices queue_family_indices;
};