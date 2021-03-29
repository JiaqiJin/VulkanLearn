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

std::pair<int, int> VContext::getWindowFrameBufferSize() const
{
	int width, height;
	glfwGetFramebufferSize(window, &height, &width);
	return std::make_pair(width, height);
}

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

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
		if (presentSupport) 
		{
			indices.presentFamily = i;
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

void VContext::createWindowSurface()
{
	VkSurfaceKHR temp_surface;
	auto result = glfwCreateWindowSurface(instance.get(), window, nullptr, &temp_surface);

	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create window surface!");
	}

	windowSurface = VRaii<vk::SurfaceKHR>{
		temp_surface,
		[instance = this->instance.get()](auto& surface)
		{
			instance.destroySurfaceKHR(surface);
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

	bool swapChainAdequate = false;
	if (extensions_supported)
	{
		SwapChainSupportDetails swapChainSupport = SwapChainSupportDetails::querySwapChainSupport(device, static_cast<VkSurfaceKHR>(window_surface));
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.present_modes.empty();
	}

	return indices.isComplete() && extensions_supported && swapChainAdequate;
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

	for (const auto& device : devices)
	{
		if (isDeviceSuitable(device, windowSurface.get()))
		{
			localPhysicDevice = device;
			break;
		}
	}

	VkPhysicalDeviceProperties properties;

	if (localPhysicDevice == VK_NULL_HANDLE)
	{
		throw std::runtime_error("Failed to find a suitable GPU!");
	}
	else
	{
		vkGetPhysicalDeviceProperties(localPhysicDevice, &properties);
		std::cout << "Current Device: " << properties.deviceName << std::endl;
	}

	this->physicalDevice = localPhysicDevice;
	this->physicalDeviceProperties = properties;
}

void VContext::findQueueFamilyIndices()
{
	queue_family_indices = QueueFamilyIndices::findQueueFamilies(physicalDevice, windowSurface.get());

	if (!queue_family_indices.isComplete())
	{
		throw std::runtime_error("Queue family indices not complete!");
	}
}

void VContext::createLogicalDevice() // TODO change for more 
{
	QueueFamilyIndices indices = QueueFamilyIndices::findQueueFamilies(physicalDevice, static_cast<VkSurfaceKHR>(windowSurface.get()));

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};

	std::set<uint32_t> uniqueQueueFamilies;
	std::vector<std::vector<float>> queuePriorities; // 2 queues in graphics family, 1 used for light cullingf

	if (indices.graphicsFamily != indices.presentFamily)
	{
		uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };
		queuePriorities = { { 1.0f, 1.0f },{ 1.0f } }; // 2 queues in graphics family, 1 used for light cullingf
	}
	else
	{
		uniqueQueueFamilies = { indices.graphicsFamily.value() };
		queuePriorities = { { 1.0f, 1.0f, 1.0f } };
	}


	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	// Specify used device features
	VkPhysicalDeviceFeatures device_features = {}; // Everything is by default VK_FALSE

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.queueCreateInfoCount = static_cast<uint32_t> (queueCreateInfos.size());
	createInfo.pEnabledFeatures = &device_features;

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

	graphicsDevice = VRaii<vk::Device>{
		temp_device,
		[](vk::Device& device)
		{
			device.destroy();
		}
	};

	auto device = graphicsDevice.get();
	// add compute queue
	vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);

	if (indices.graphicsFamily != indices.presentFamily)
	{
		vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
	}
	else
	{
		vkGetDeviceQueue(device, indices.presentFamily.value(), 2, &presentQueue);
	}
}

SwapChainSupportDetails SwapChainSupportDetails::querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	SwapChainSupportDetails details;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	// Getting supported surface formats
	uint32_t format_count;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, nullptr);
	if (format_count != 0)
	{
		details.formats.resize(format_count);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, details.formats.data());
	}

	// Getting supported present modes
	uint32_t present_mode_count;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, nullptr);
	if (present_mode_count != 0)
	{
		details.present_modes.resize(present_mode_count);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, details.present_modes.data());
	}

	return details;
}