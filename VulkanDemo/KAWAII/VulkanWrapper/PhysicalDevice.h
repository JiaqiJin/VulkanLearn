#pragma once
#define VK_USE_PLATFORM_WIN32_KHR 1 
#include "Instance.h"
#include <vulkan/vulkan.h>
#include <memory>
#include <vector>
#include <Windows.h>

namespace RHI
{
	class PhysicalDevice
	{
	public:
		enum class QueueFamily
		{
			ALL_ROUND,
			COMPUTE,
			TRASFER,
			COUNT
		};
		PhysicalDevice(const std::shared_ptr<Instance>& instance, HINSTANCE hInst, HWND hWnd);

		PhysicalDevice(const PhysicalDevice&) = delete;

		PhysicalDevice(PhysicalDevice&&) = delete;

		PhysicalDevice& operator=(const PhysicalDevice&) = delete;

		PhysicalDevice& operator=(PhysicalDevice&&) = delete;

		~PhysicalDevice();

		bool Init(const std::shared_ptr<Instance> instance, HINSTANCE hInst, HWND hWnd);
	private:
		// Instance
		std::shared_ptr<Instance> m_instance;
		// Physical Device 
		VkPhysicalDevice m_physicalDevice;
		VkPhysicalDeviceProperties m_physicalDeviceProperties;
		VkPhysicalDeviceFeatures m_physicalDeviceFeatures;
		VkPhysicalDeviceMemoryProperties m_physicalDeviceMemoryProperties;

		// Queue families
		std::vector<VkQueueFamilyProperties> m_queueProperties;
		uint32_t m_queueFamilyIndices[(uint32_t)QueueFamily::COUNT];

		// Surface
		VkSurfaceKHR m_surface;
		// Depth stenci format
		VkFormat m_depthStencilFormat;

		// Swap Chain
		VkSwapchainKHR m_swapchain;

		// Querying details of swap chain support
		std::vector<VkSurfaceFormatKHR>	m_surfaceFormats;
		std::vector<VkPresentModeKHR> m_presentModes;
		VkSurfaceCapabilitiesKHR m_surfaceCap;

		PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR	m_fpGetPhysicalDeviceSurfaceCapabilitiesKHR;
		PFN_vkGetPhysicalDeviceSurfaceFormatsKHR		m_fpGetPhysicalDeviceSurfaceFormatsKHR;
		PFN_vkGetPhysicalDeviceSurfacePresentModesKHR	m_fpGetPhysicalDeviceSurfacePresentModesKHR;
		PFN_vkGetPhysicalDeviceSurfaceSupportKHR		m_fpGetPhysicalDeviceSurfaceSupportKHR;
		PFN_vkCreateSwapchainKHR						m_fpCreateSwapchainKHR;
		PFN_vkCreateWin32SurfaceKHR						m_fpCreateWin32SurfaceKHR;
		PFN_vkDestroySurfaceKHR							m_fpDestroySurfaceKHR;
	};
}