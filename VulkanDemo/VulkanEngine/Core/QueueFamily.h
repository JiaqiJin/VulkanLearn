#pragma once

#include <vulkan/vulkan.h>

namespace Rendering
{
	class PhysicalDevice;

	class QueueFamily
	{
	public:
		QueueFamily(uint32_t index, VkQueueFamilyProperties properties);

		uint32_t getIndex() const { return m_index; }
		VkQueueFamilyProperties const& getProperties() const { return m_properties; }
	private:
		uint32_t m_index;
		VkQueueFamilyProperties m_properties;
	};

	class QueueFamilyIndices
	{
	public:
		QueueFamilyIndices(Rendering::PhysicalDevice& physicalDevice);

		QueueFamily const& getGraphicsQueueFamily() const { return *m_graphicsQueueFamily; }
		QueueFamily const& getPresentQueueFamily() const { return *m_presentQueueFamily; }

		bool isValid() const { return m_graphicsQueueFamily && m_presentQueueFamily; }

	private:
		QueueFamily const* m_graphicsQueueFamily = nullptr; // const * beacuse we need changed value
		QueueFamily const* m_presentQueueFamily = nullptr;;
	};
}