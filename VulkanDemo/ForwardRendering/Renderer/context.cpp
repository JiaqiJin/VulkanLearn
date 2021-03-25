#include "context.h"

#include <GLFW/glfw3.h>

#include <unordered_set>
#include <iostream>
#include <cstring>
#include <set>

#ifdef NDEBUG
// if not debugging
const bool ENABLE_VALIDATION_LAYERS = false;
#else
const bool ENABLE_VALIDATION_LAYERS = true;
#endif

const std::vector<const char*> VALIDATION_LAYERS = {
	"VK_LAYER_LUNARG_standard_validation"
};

const std::vector<const char*> DEVICE_EXTENSIONS = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

QueueFamilyIndices QueueFamilyIndices::findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queuefamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queuefamilies.data());

	int i = 0;
	for (const auto& queueFamily : queuefamilies)
	{
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;
		}

		if (indices.isComplete()) {
			break;
		}

		i++;
	}

	return indices;
}

VContext::VContext(GLFWwindow* window)
{
	if (!window)
	{
		throw std::runtime_error("invalid window");
	}

	this->window = window;

	initVulkan();
}

VContext::~VContext() = default;

// the debug callback function that Vulkan runs
VkBool32 debugCallback(
	VkDebugReportFlagsEXT flags,
	VkDebugReportObjectTypeEXT objType,
	uint64_t obj,
	size_t location,
	int32_t code,
	const char* layerPrefix,
	const char* msg,
	void* userData)
{
	std::cerr << "validation layer: " << msg << std::endl;
	return VK_FALSE;
}

bool checkValidationLayerSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : VALIDATION_LAYERS)
	{
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers)
		{
			if (strcmp(layerName, layerProperties.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}

std::vector<const char*> getRequiredExtensions()
{
	std::vector<const char*> extensions;

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	for (unsigned int i = 0; i < glfwExtensionCount; i++)
	{
		extensions.push_back(glfwExtensions[i]);
	}

	if (ENABLE_VALIDATION_LAYERS)
	{
		extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}

	return extensions;
	//free(&extensions);
}

VkResult VContext::CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback)
{
	auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
	if (func != nullptr)
	{
		return func(instance, pCreateInfo, pAllocator, pCallback);
	}
	else
	{
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void VContext::DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator)
{
	auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
	if (func != nullptr) {
		func(instance, callback, pAllocator);
	}
}

void VContext::createInstance()
{
	if (ENABLE_VALIDATION_LAYERS && !checkValidationLayerSupport())
	{
		throw std::runtime_error("validation layers requested, but not available!");
	}


	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	auto glfwExtensions = getRequiredExtensions();

	// Vulkan support extension
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
	std::unordered_set<std::string> supportedExtensionNames;
	for (const auto& extension : extensions)
	{
		supportedExtensionNames.insert(std::string(extension.extensionName));
	}

	// Print vulkan extension
	std::cout << "available extensions:" << std::endl;
	for (auto& name : supportedExtensionNames)
	{
		std::cout << "\t" << name << std::endl;
	}

	// Check for and print any unsupported extension
	for (const auto& extension_name : glfwExtensions)
	{
		std::string name(extension_name);
		if (supportedExtensionNames.count(name) <= 0)
		{
			std::cout << "unsupported extension required by GLFW: " << name << std::endl;
		}
	}

	if (ENABLE_VALIDATION_LAYERS) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
		createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	createInfo.enabledExtensionCount = static_cast<uint32_t>(glfwExtensions.size());
	createInfo.ppEnabledExtensionNames = glfwExtensions.data();

	instance = VRaii<vk::Instance>{
		vk::createInstance(createInfo, nullptr),
		[](vk::Instance& obj)
		{
			obj.destroy();
		}
	};
}

void VContext::setupDebugCallback()
{
	if (!ENABLE_VALIDATION_LAYERS) return;

	VkDebugReportCallbackCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	createInfo.pfnCallback = (PFN_vkDebugReportCallbackEXT)debugCallback;

	VkDebugReportCallbackEXT temp_callback;
	if (CreateDebugReportCallbackEXT(instance.get(), &createInfo, nullptr, &temp_callback) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to set up debug callback!");
	}

	callback = VRaii<vk::DebugReportCallbackEXT>{
		temp_callback,
		[instance = this->instance.get()](auto& obj)
		{
			DestroyDebugReportCallbackEXT(instance, obj, nullptr);
		}
	};
}

bool checkDeviceExtensionSupport(VkPhysicalDevice device)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> avaliableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, avaliableExtensions.data());

	std::set<std::string> required_extensions(DEVICE_EXTENSIONS.begin(), DEVICE_EXTENSIONS.end());

	for (const auto& extension : avaliableExtensions)
	{
		required_extensions.erase(extension.extensionName);
	}

	return required_extensions.empty();
}

bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR window_surface)
{
	QueueFamilyIndices indices = QueueFamilyIndices::findQueueFamilies(device, static_cast<VkSurfaceKHR>(window_surface));

	bool extensions_supported = checkDeviceExtensionSupport(device);

	return indices.isComplete() && extensions_supported;
}

void VContext::pickPhysicalDevice()
{
	VkPhysicalDevice localPhysicDevice = VK_NULL_HANDLE;
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance.get(), &deviceCount, nullptr);

	if (deviceCount == 0) {
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance.get(), &deviceCount, devices.data());
}

void VContext::findQueueFamilyIndices()
{
	queue_family_indices = QueueFamilyIndices::findQueueFamilies(physicalDevice, window_surface.get());

	if (!queue_family_indices.isComplete())
	{
		throw std::runtime_error("Queue family indices not complete!");
	}
}

void VContext::createLogicalDevice() // TODO change for more 
{
	QueueFamilyIndices indices = QueueFamilyIndices::findQueueFamilies(physicalDevice, static_cast<VkSurfaceKHR>(window_surface.get()));

	VkDeviceQueueCreateInfo queueCreateInfo{};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
	queueCreateInfo.queueCount = 1;

	float queuePriority = 1.0f;
	queueCreateInfo.pQueuePriorities = &queuePriority;

	VkPhysicalDeviceFeatures deviceFeatures{};

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = &queueCreateInfo;
	createInfo.queueCreateInfoCount = 1;

	createInfo.pEnabledFeatures = &deviceFeatures;

	if (ENABLE_VALIDATION_LAYERS)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
		createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	createInfo.enabledExtensionCount = static_cast<uint32_t>(DEVICE_EXTENSIONS.size());
	createInfo.ppEnabledExtensionNames = DEVICE_EXTENSIONS.data();

	VkDevice temp_device;
	if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &temp_device) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create logical device!");
	}

	graphics_device = VRaii<vk::Device>{
		temp_device,
		[](vk::Device& device)
		{
			device.destroy();
		}
	};

	auto device = graphics_device.get();

	vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
}