#include "PhysicalDevice.h"
#include "Utils.h"

namespace Rendering
{
	PhysicalDevice::PhysicalDevice(VkPhysicalDevice device) :
		m_physicalDevice(device)
	{
		init();
	}

	PhysicalDevice::PhysicalDevice(PhysicalDevice&& other)
	{
		std::swap(other.m_physicalDevice, m_physicalDevice);

		init();
	}

	void PhysicalDevice::init()
	{
		queryAvailableExtensions();
		queryProperties();
		queryFeatures();
		queryQueueFamilyProperties();
	}

	void PhysicalDevice::queryProperties()
	{
		vkGetPhysicalDeviceProperties(m_physicalDevice, &m_properties);
	}

	void PhysicalDevice::queryFeatures()
	{
		vkGetPhysicalDeviceFeatures(m_physicalDevice, &m_deviceFeatures);
	}

	void PhysicalDevice::queryAvailableExtensions()
	{
		uint32_t count;
		vkEnumerateDeviceExtensionProperties(m_physicalDevice, nullptr, &count, nullptr);
		m_availableExtensions.resize(count);
		vkEnumerateDeviceExtensionProperties(m_physicalDevice, nullptr, &count, m_availableExtensions.data());

		for (const auto& extension : m_availableExtensions)
			m_availableExtensionNames.push_back(extension.extensionName);
	}

	void PhysicalDevice::queryQueueFamilyProperties()
	{
		std::vector<VkQueueFamilyProperties> properties;

		uint32_t count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &count, nullptr);
		properties.resize(count);
		vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &count, properties.data());

		m_queueFamilies.reserve(count);
		for (uint32_t i = 0; i < count; i++)
			m_queueFamilies.emplace_back(i, properties[i]);
	}
}