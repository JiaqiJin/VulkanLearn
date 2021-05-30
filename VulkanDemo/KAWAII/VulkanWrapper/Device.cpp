#include "Device.h"
#include "Instance.h"
#include "PhysicalDevice.h"
#include "../Common/Logger.h"
#include <set>
#include <array>

namespace RHI
{
	Device::Device(const std::shared_ptr<Instance>& pInstance, const std::shared_ptr<PhysicalDevice>& pPhyisicalDevice)
		: m_pPhysicalDevice(pPhyisicalDevice), m_pVulkanInst(pInstance)
	{
		if (!Init())
			K_ERROR("Error Initialize Device");
	}

	bool Device::Init()
	{
		std::set<uint32_t> queueIndexSet;
		queueIndexSet.insert(m_pPhysicalDevice->GetQueueFamilyIndex(PhysicalDevice::QueueFamily::ALL_ROUND));
		queueIndexSet.insert(m_pPhysicalDevice->GetQueueFamilyIndex(PhysicalDevice::QueueFamily::COMPUTE));
		queueIndexSet.insert(m_pPhysicalDevice->GetQueueFamilyIndex(PhysicalDevice::QueueFamily::TRASFER));

		std::array<float, 1> queueProperties = { 0.0f };
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		for (auto queueIndex : queueIndexSet)
		{
			VkDeviceQueueCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			info.queueFamilyIndex = queueIndex;
			info.queueCount = (uint32_t)queueProperties.size();
			info.pQueuePriorities = queueProperties.data();
			queueCreateInfos.push_back(info);
		}

		VkDeviceCreateInfo deviceCreateInfo = {};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.queueCreateInfoCount = (uint32_t)queueCreateInfos.size();
		deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
		std::vector<const char*> extensions = { EXTENSION_VULKAN_SWAPCHAIN, EXTENSION_SHADER_DRAW_PARAMETERS, EXTENSION_VULKAN_DRAW_INDIRECT_COUNT };
		deviceCreateInfo.enabledExtensionCount = (uint32_t)extensions.size();
		deviceCreateInfo.ppEnabledExtensionNames = extensions.data();

		VkPhysicalDeviceFeatures enabledFeatures = {};
		enabledFeatures.drawIndirectFirstInstance = 1;
		enabledFeatures.multiDrawIndirect = 1;
		enabledFeatures.fullDrawIndexUint32 = 1;
		enabledFeatures.vertexPipelineStoresAndAtomics = 1;
		enabledFeatures.fragmentStoresAndAtomics = 1;
		enabledFeatures.depthBiasClamp = 1;
		deviceCreateInfo.pEnabledFeatures = &enabledFeatures;

		// Vulkan 1.2 feature
		VkPhysicalDeviceVulkan12Features vulkan12Features = {};
		vulkan12Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
		vulkan12Features.drawIndirectCount = true;
		deviceCreateInfo.pNext = (void*)&vulkan12Features;

		RETURN_FALSE_VK_RESULT(vkCreateDevice(m_pPhysicalDevice->GetDeviceHandle(), &deviceCreateInfo, nullptr, &m_device));

		GET_DEVICE_PROC_ADDR(m_device, CmdDrawIndexedIndirectCountKHR);

		return true;
	}

	Device::~Device()
	{
		vkDestroyDevice(m_device, nullptr);
	}
}