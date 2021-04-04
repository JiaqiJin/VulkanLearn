#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan.hpp>

#include <array>
#include <string>

#include "raii.h"

class VContext;

class VUtility
{
public:
	VUtility(const VContext& context);
	~VUtility() = default;

	VUtility(VUtility&&) = delete;
	VUtility& operator= (VUtility&&) = delete;
	VUtility(const VUtility&) = delete;
	VUtility& operator= (const VUtility&) = delete;

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	void createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectMask, VkImageView* pImageView);
	VRaii<VkImageView> createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectMask);

private:
	const VContext* context;
	VkDevice graphicsDevice;
	vk::Device device;
	VkPhysicalDevice physicalDevice;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
};
