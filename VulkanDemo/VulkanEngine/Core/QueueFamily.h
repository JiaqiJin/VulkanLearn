#pragma once

#include <vulkan/vulkan.h>

namespace Rendering
{
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
		QueueFamilyIndices();

		QueueFamily& getGraphicsQueueFamily() const { return *m_graphicsQueueFamily; }
		QueueFamily& getPresentQueueFamily() const { return *m_presentQueueFamily; }
	private:
		QueueFamily* m_graphicsQueueFamily = nullptr;
		QueueFamily* m_presentQueueFamily = nullptr;
	};
}