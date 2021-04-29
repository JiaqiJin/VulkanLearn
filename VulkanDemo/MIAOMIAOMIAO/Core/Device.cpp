#include "Device.h"
#include "Surface.h"

#include <set>

namespace Rendering
{
	const std::vector<const char*> Device::RequiredExtensions =
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		VK_NV_RAY_TRACING_EXTENSION_NAME,
		// For 1.2.163 version
		//VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
		//VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
		//VK_KHR_PIPELINE_LIBRARY_EXTENSION_NAME,
		//VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
		//VK_KHR_MAINTENANCE3_EXTENSION_NAME,
		//VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME,
	};

	Device::Device(VkPhysicalDevice physicalDevice, const Surface& surface)
		: m_physicalDevice(physicalDevice), m_surface(surface)
	{
		if (!CheckDeviceExtensionSupport(physicalDevice))
		{
			throw std::runtime_error("The device is not suitable!");
		}

		auto indices = FindQueueFamilies(physicalDevice);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

		float queuePriority = 1.0f;

		for (auto index : indices)
		{
			uint32_t queueFamilyIndex = index.family.value();

			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;

			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures{};
		deviceFeatures.fillModeNonSolid = VK_TRUE;
		deviceFeatures.samplerAnisotropy = VK_TRUE;

		VkPhysicalDeviceDescriptorIndexingFeaturesEXT indexingFeatures = {};
		indexingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES_EXT;
		indexingFeatures.runtimeDescriptorArray = VK_TRUE;

		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pNext = &indexingFeatures;
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledLayerCount = 0;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(RequiredExtensions.size());
		createInfo.ppEnabledExtensionNames = RequiredExtensions.data();

		VK_CHECK(vkCreateDevice(physicalDevice, &createInfo, nullptr, &m_device), "Create Vulkan logical device");

		for (auto index : indices)
		{
			uint32_t family = index.family.value();
			VkBool32 presentSupport = VK_FALSE;
			vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, family, surface.Get(), &presentSupport);
			if (presentSupport)
			{
				vkGetDeviceQueue(m_device, family, 0, &PresentQueue);
				PresentFamilyIndex = family;
				break;
			}
		}

		for (auto index : indices)
		{
			uint32_t family = index.family.value();
			vkGetDeviceQueue(m_device, family, 0, &index.queue);
		}
	}

	Device::~Device()
	{
		if (m_device != nullptr)
		{
			vkDestroyDevice(m_device, nullptr);
			m_device = nullptr;
		}
	}

	std::vector<QueueFamily> Device::FindQueueFamilies(VkPhysicalDevice device)
	{
		std::vector<QueueFamily> indices;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies)
		{
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				QueueFamily index{ GraphicsQueue };
				index.family = i;
				GraphicsFamilyIndex = i;
				indices.push_back(index);
			}
		}
		// TODO
		return indices;
	}

	bool Device::CheckDeviceExtensionSupport(VkPhysicalDevice physicalDevice)
	{
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(RequiredExtensions.begin(), RequiredExtensions.end());

		for (const auto& extension : availableExtensions)
		{
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	void Device::WaitIdle() const
	{
		VK_CHECK(vkDeviceWaitIdle(m_device), "Wait for a device to become idle");
	}
}