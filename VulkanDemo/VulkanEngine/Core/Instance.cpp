#include "Instance.h"
#include "PhysicalDevice.h"
#include "Utils.h"

#include <stdexcept>

namespace Rendering
{
	std::vector<VkLayerProperties> getAvailableLayers()
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		return availableLayers;
	}

	std::vector<VkExtensionProperties> getAvailableExtensions()
	{
		uint32_t count = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(count);
		vkEnumerateInstanceExtensionProperties(nullptr, &count, availableExtensions.data());

		return availableExtensions;
	}

	Instance::Instance(std::string const& appName, std::vector<char const*> extensions, 
		bool enableValidation, bool enableApiDump)
	{
		m_availableLayers = getAvailableLayers();
		m_availableLayerNames.reserve(m_availableLayers.size());
		for (const auto& layer : m_availableLayers)
		{
			m_availableLayerNames.push_back(layer.layerName);
		}

		m_availableExtensions = getAvailableExtensions();
		m_availableExtensionNames.reserve(m_availableExtensions.size());
		for (const auto& extension : m_availableExtensions)
		{
			m_availableExtensionNames.push_back(extension.extensionName);
		}

		createInstance(appName, extensions, enableValidation, enableApiDump);
	}

	Instance::~Instance()
	{
		vkDestroyInstance(m_instance, nullptr);
	}

	void Instance::createInstance(std::string const& appName, std::vector<char const*> extensions, 
		bool enableValidation, bool enableApiDump)
	{
		std::vector<char const*> requestedLayers;
		if (enableValidation)
			requestedLayers.push_back("VK_LAYER_KHRONOS_validation");
		if (enableApiDump)
			requestedLayers.push_back("VK_LAYER_LUNARG_api_dump");

		std::vector<char const*> requestedExtensions = extensions;

		if (!Utils::checkSupportOption(m_availableLayerNames, requestedLayers))
			throw std::runtime_error("validation layers requested, but not available!");

		if (!Utils::checkSupportOption(m_availableExtensionNames, requestedExtensions))
			throw std::runtime_error("extension layers requested, but not available!");

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = appName.c_str();
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(requestedExtensions.size());
		createInfo.ppEnabledExtensionNames = requestedExtensions.data();
		createInfo.enabledLayerCount = static_cast<uint32_t>(requestedLayers.size());
		createInfo.ppEnabledLayerNames = requestedLayers.data();

		if (vkCreateInstance(&createInfo, nullptr, &m_instance.get()) != VK_SUCCESS)
			throw std::runtime_error("Failed to create Vulkan instance");
	}
}