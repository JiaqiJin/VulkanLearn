#include "Device.h"
#include "Queue.h"
#include "PhysicalDeviceSurface.h"
#include "QueueFamily.h"
#include "PhysicalDevice.h"
#include <set>
#include <stdexcept>

namespace Rendering
{
	Device::Device(const PhysicalDeviceSurfaceContainer& physicalDeviceSurfaceContainer, std::vector<const char*> const& extensions)
	{
		const PhysicalDevice& physicalDevice = physicalDeviceSurfaceContainer.getPhysicalDevice();

		const QueueFamilyIndices indices = physicalDeviceSurfaceContainer.getParameters().getQueueFamilyIndices();

		std::set<const QueueFamily*> uniqueQueueFamilies = { &indices.getGraphicsQueueFamily(), &indices.getPresentQueueFamily() };

        std::vector<float> queuePriorities = { 1.0f };

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        queueCreateInfos.reserve(uniqueQueueFamilies.size());
        for (const QueueFamily* queueFamily : uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo& queueCreateInfo = queueCreateInfos.emplace_back();
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily->getIndex();
            queueCreateInfo.queueCount = static_cast<uint32_t>(queuePriorities.size());
            queueCreateInfo.pQueuePriorities = queuePriorities.data();
        }

        VkPhysicalDeviceFeatures deviceFeatures{};
        deviceFeatures.samplerAnisotropy = VK_TRUE;
        deviceFeatures.geometryShader = VK_TRUE;

        VkDeviceCreateInfo deviceCreateInfo{};
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
        deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
        deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        deviceCreateInfo.ppEnabledExtensionNames = extensions.data();

        if (vkCreateDevice(physicalDevice.getHandle(), &deviceCreateInfo, nullptr, &m_handle.get()) != VK_SUCCESS)
            throw std::runtime_error("failed to create logical device!");

        for (const QueueFamily* queueFamily : uniqueQueueFamilies)
        {
            VkQueue handle = VK_NULL_HANDLE;
            vkGetDeviceQueue(m_handle, queueFamily->getIndex(), 0, &handle);
            const Queue& queue = m_queues.emplace_back(handle, *queueFamily);

            if (queueFamily->getIndex() == indices.getGraphicsQueueFamily().getIndex())
                m_graphicsQueue = &queue;
            if (queueFamily->getIndex() == indices.getPresentQueueFamily().getIndex())
                m_presentQueue = &queue;
        }

        if (m_graphicsQueue == nullptr || m_presentQueue == nullptr)
            throw std::runtime_error("failed to get device queues!");
	}

	Device::~Device()
	{
        vkDestroyDevice(m_handle, nullptr);
	}

    void Device::waitIdle() const
    {
        vkDeviceWaitIdle(m_handle);
    }
}