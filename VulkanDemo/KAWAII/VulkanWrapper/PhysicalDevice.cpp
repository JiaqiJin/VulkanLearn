#include "PhysicalDevice.h"
#include "../Common/Macro.h"

namespace RHI
{
	PhysicalDevice::~PhysicalDevice()
	{
		if (m_instance.get())
			vkDestroySurfaceKHR(m_instance->GetInstance(), m_surface, nullptr);
	}

	PhysicalDevice::PhysicalDevice(const std::shared_ptr<Instance>& instance, HINSTANCE hInst, HWND hWnd)
	{
		if (!Init(instance, hInst, hWnd))
			std::cout << "Error Initialize the physical device";
	}

	bool PhysicalDevice::Init(const std::shared_ptr<Instance> instance, HINSTANCE hInst, HWND hWnd)
	{
		uint32_t gpuCount = 0;
		vkEnumeratePhysicalDevices(instance->GetInstance(), &gpuCount, nullptr);
		if (gpuCount == 0)
			return false;

		std::vector<VkPhysicalDevice> physicalDevices;
		physicalDevices.resize(gpuCount);
		vkEnumeratePhysicalDevices(instance->GetInstance(), &gpuCount, physicalDevices.data());

		m_physicalDevice = physicalDevices[0];

		// Queue families properties
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, nullptr);
		if (queueFamilyCount == 0)
			return false;

		m_queueProperties.resize(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, m_queueProperties.data());

		// Get physcal devices properties
		vkGetPhysicalDeviceProperties(m_physicalDevice, &m_physicalDeviceProperties);
		vkGetPhysicalDeviceFeatures(m_physicalDevice, &m_physicalDeviceFeatures);
		vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &m_physicalDeviceMemoryProperties);

		m_queueFamilyIndices[(uint32_t)QueueFamily::ALL_ROUND] = -1;
		for (uint32_t i = 0; i < m_queueProperties.size(); i++)
		{
			// I assume default graphic queue provides present, compute and transfer too
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

		// No dedicated compute queue? Alright then, get one family that is capable of dealing with compute
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

		// No dedicated transfer queue? Alright then, get one family that is capable of dealing with transfer
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

		GET_INSTANCE_PROC_ADDR(instance->GetInstance(), GetPhysicalDeviceSurfaceCapabilitiesKHR);
		GET_INSTANCE_PROC_ADDR(instance->GetInstance(), GetPhysicalDeviceSurfaceFormatsKHR);
		GET_INSTANCE_PROC_ADDR(instance->GetInstance(), GetPhysicalDeviceSurfacePresentModesKHR);
		GET_INSTANCE_PROC_ADDR(instance->GetInstance(), GetPhysicalDeviceSurfaceSupportKHR);
		GET_INSTANCE_PROC_ADDR(instance->GetInstance(), CreateSwapchainKHR);
		GET_INSTANCE_PROC_ADDR(instance->GetInstance(), CreateWin32SurfaceKHR);
		GET_INSTANCE_PROC_ADDR(instance->GetInstance(), DestroySurfaceKHR);

		VkWin32SurfaceCreateInfoKHR surfaceInfo = {};
		surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		surfaceInfo.hinstance = hInst;
		surfaceInfo.hwnd = hWnd;

		RETURN_FALSE_VK_RESULT(m_fpCreateWin32SurfaceKHR(instance->GetInstance(), &surfaceInfo, nullptr, &m_surface));

		std::vector<VkBool32> supports;
		supports.resize(m_queueProperties.size());
		for (uint32_t i = 0; i < m_queueProperties.size(); i++)
		{
			RETURN_FALSE_VK_RESULT(m_fpGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, i, m_surface, &supports[i]));
		}
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

		m_instance = instance;
		return true;
	}
}