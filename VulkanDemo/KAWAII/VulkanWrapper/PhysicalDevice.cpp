#include "PhysicalDevice.h"
#include "Instance.h"
#include "../Common/Macro.h"
#include "../Common/Logger.h"
namespace RHI
{
	PhysicalDevice::~PhysicalDevice()
	{
		if (m_pVulkanInstance.get())
			vkDestroySurfaceKHR(m_pVulkanInstance->GetDeviceHandle(), m_surface, nullptr);
	}

	PhysicalDevice::PhysicalDevice(const std::shared_ptr<Instance>& instance, HINSTANCE hInst, HWND hWnd)
		: m_pVulkanInstance(instance)
	{
		if (!Init(hInst, hWnd))
			K_ERROR("Error Initialize Physical Device");
	}

	bool PhysicalDevice::Init(HINSTANCE hInst, HWND hWnd)
	{
		//Get an available physical device
		uint32_t gpuCount = 0;
		vkEnumeratePhysicalDevices(m_pVulkanInstance->GetDeviceHandle(), &gpuCount, nullptr);
		if (gpuCount == 0)
			return false;

		std::vector<VkPhysicalDevice> physicalDevices;
		physicalDevices.resize(gpuCount);
		vkEnumeratePhysicalDevices(m_pVulkanInstance->GetDeviceHandle(), &gpuCount, physicalDevices.data());

		//Hard coded, using first physical device
		m_physicalDevice = physicalDevices[0];

		//Get queue properties from physical device
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, nullptr);
		if (queueFamilyCount == 0)
			return false;

		m_queueProperties.resize(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, m_queueProperties.data());

		//Get physical device properties
		vkGetPhysicalDeviceProperties(m_physicalDevice, &m_physicalDeviceProperties);
		vkGetPhysicalDeviceFeatures(m_physicalDevice, &m_physicalDeviceFeatures);
		vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &m_physicalDeviceMemoryProperties);

		//Get depth stencil format
		std::vector<VkFormat> formats =
		{
			VK_FORMAT_D32_SFLOAT_S8_UINT,
			VK_FORMAT_D32_SFLOAT,
			VK_FORMAT_D24_UNORM_S8_UINT,
			VK_FORMAT_D16_UNORM_S8_UINT,
			VK_FORMAT_D16_UNORM
		};

		for (uint32_t i = 0; i < formats.size(); i++)
		{
			VkFormatProperties prop;
			vkGetPhysicalDeviceFormatProperties(m_physicalDevice, formats[i], &prop);
			if (prop.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
			{
				m_depthStencilFormat = formats[i];
				break;
			}
		}

		m_queueFamilyIndices[(uint32_t)QueueFamily::ALL_ROUND] = -1;
		for (uint32_t i = 0; i < m_queueProperties.size(); i++)
		{
			// I assume default graphic queue provides present, compute and transfer too
			// Maybe this needs to change in future
			// I'll leave it for now
			if ((m_queueProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) &&
				(m_queueProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) &&
				(m_queueProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT))
			{
				m_queueFamilyIndices[(uint32_t)QueueFamily::ALL_ROUND] = i;
				break;
			}
		}

		ASSERTION(m_queueFamilyIndices[(uint32_t)QueueFamily::ALL_ROUND] != -1);

		m_queueFamilyIndices[(uint32_t)QueueFamily::COMPUTE] = -1;

		// Find a dedicated compute queue
		for (uint32_t i = 0; i < m_queueProperties.size(); i++)
		{
			// Find a dedicated compute queue
			// Theoractically a dedicated compute queue should have only a compute flag
			if ((m_queueProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) && !(m_queueProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT))
			{
				m_queueFamilyIndices[(uint32_t)QueueFamily::COMPUTE] = i;
				break;
			}
		}

		// No dedicated compute queue? Get one family that is capable of dealing with compute
		if (m_queueFamilyIndices[(uint32_t)QueueFamily::COMPUTE] == -1)
		{
			for (uint32_t i = 0; i < m_queueProperties.size(); i++)
			{
				if (m_queueProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
				{
					m_queueFamilyIndices[(uint32_t)QueueFamily::COMPUTE] = i;
					break;
				}
			}
		}

		ASSERTION(m_queueFamilyIndices[(uint32_t)QueueFamily::COMPUTE] != -1);

		m_queueFamilyIndices[(uint32_t)QueueFamily::TRASFER] = -1;

		// Find a dedicated transfer queue
		for (uint32_t i = 0; i < m_queueProperties.size(); i++)
		{
			// Find a dedicated transfer queue
			// Theoractically a dedicated transfer queue should have only a transfer flag
			if ((m_queueProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT) &&
				!(m_queueProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) &&
				!(m_queueProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT))
			{
				m_queueFamilyIndices[(uint32_t)QueueFamily::TRASFER] = i;
				break;
			}
		}

		if (m_queueFamilyIndices[(uint32_t)QueueFamily::TRASFER] == -1)
		{
			for (uint32_t i = 0; i < m_queueProperties.size(); i++)
			{
				if (m_queueProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
				{
					m_queueFamilyIndices[(uint32_t)QueueFamily::TRASFER] = i;
					break;
				}
			}
		}

		ASSERTION(m_queueFamilyIndices[(uint32_t)QueueFamily::TRASFER] != -1);

		GET_INSTANCE_PROC_ADDR(m_pVulkanInstance->GetDeviceHandle(), GetPhysicalDeviceSurfaceCapabilitiesKHR);
		GET_INSTANCE_PROC_ADDR(m_pVulkanInstance->GetDeviceHandle(), GetPhysicalDeviceSurfaceFormatsKHR);
		GET_INSTANCE_PROC_ADDR(m_pVulkanInstance->GetDeviceHandle(), GetPhysicalDeviceSurfacePresentModesKHR);
		GET_INSTANCE_PROC_ADDR(m_pVulkanInstance->GetDeviceHandle(), GetPhysicalDeviceSurfaceSupportKHR);
		GET_INSTANCE_PROC_ADDR(m_pVulkanInstance->GetDeviceHandle(), CreateSwapchainKHR);

#if defined(_WIN32)
		GET_INSTANCE_PROC_ADDR(m_pVulkanInstance->GetDeviceHandle(), CreateWin32SurfaceKHR);
#endif
		GET_INSTANCE_PROC_ADDR(m_pVulkanInstance->GetDeviceHandle(), DestroySurfaceKHR);
		//return true;
#if defined(_WIN32)
		VkWin32SurfaceCreateInfoKHR surfaceInfo = {};
		surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		surfaceInfo.hinstance = hInst;
		surfaceInfo.hwnd = hWnd;

		RETURN_FALSE_VK_RESULT(m_fpCreateWin32SurfaceKHR(m_pVulkanInstance->GetDeviceHandle(), &surfaceInfo, nullptr, &m_surface));
#endif

		RETURN_FALSE_VK_RESULT(m_fpGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice, m_surface, &m_surfaceCap));

		std::vector<VkBool32> supports;
		supports.resize(m_queueProperties.size());
		for (uint32_t i = 0; i < m_queueProperties.size(); i++)
		{
			RETURN_FALSE_VK_RESULT(m_fpGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, i, m_surface, &supports[i]));
		}

		// Assume all-round queue family is capable of present
#if defined(_DEBUG)
		uint32_t presentQueueIndex = -1;
		for (uint32_t i = 0; i < m_queueProperties.size(); i++)
		{
			if (supports[i])
			{
				presentQueueIndex = i;
				break;
			}
		}
#endif

		ASSERTION(presentQueueIndex != -1);
		ASSERTION(presentQueueIndex == m_queueFamilyIndices[(uint32_t)QueueFamily::ALL_ROUND]);

		uint32_t formatCount;
		RETURN_FALSE_VK_RESULT(m_fpGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &formatCount, nullptr));
		m_surfaceFormats.resize(formatCount);
		RETURN_FALSE_VK_RESULT(m_fpGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &formatCount, m_surfaceFormats.data()));

		uint32_t presentModeCount = -1;
		RETURN_FALSE_VK_RESULT(m_fpGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, m_surface, &presentModeCount, nullptr));
		m_presentModes.resize(presentModeCount);
		RETURN_FALSE_VK_RESULT(m_fpGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, m_surface, &presentModeCount, m_presentModes.data()));

		return true;
	}

	VkFormatProperties PhysicalDevice::GetPhysicalDeviceFormatProperties(VkFormat format) const
	{
		VkFormatProperties formatProp = {};
		vkGetPhysicalDeviceFormatProperties(m_physicalDevice, format, &formatProp);
		return formatProp;
	}
}