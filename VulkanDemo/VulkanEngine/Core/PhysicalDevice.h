#pragma once

#include <vulkan/vulkan.h>
#include "QueueFamily.h"
#include "Utils.h"
#include <vector>

namespace Rendering
{
	class PhysicalDevice
	{
	public:
		explicit PhysicalDevice(VkPhysicalDevice handle);

		VkPhysicalDevice const& getPhysicalDevice() const { return m_physicalDevice; }

		VkPhysicalDeviceProperties const& getProperties() const { return m_properties; }
		VkPhysicalDeviceFeatures const& getFeatures() const { return m_deviceFeatures; }
		std::vector<QueueFamily> const& getQueueFamilies() const { return m_queueFamilies; }

		bool ExtensionsSupported(std::vector<char const*> const& requestedExtensions) const
		{
			return Utils::checkSupportOption(m_availableExtensionNames, requestedExtensions);
		}

	private:
		void init();

		void queryProperties();
		void queryFeatures();
		void queryAvailableExtensions();
		void queryQueueFamilyProperties();

	private:
		VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;

		std::vector<VkExtensionProperties> m_availableExtensions;
		std::vector<char const*> m_availableExtensionNames;

		// device suitable check
		VkPhysicalDeviceProperties m_properties;
		VkPhysicalDeviceFeatures m_deviceFeatures;
		std::vector<QueueFamily> m_queueFamilies;
	};
}