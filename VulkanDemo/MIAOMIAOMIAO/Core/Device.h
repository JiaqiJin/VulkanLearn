#pragma once

#include "Vulkan.h"

#include <optional>
#include <vector>

namespace Rendering
{
	struct QueueFamily
	{
		VkQueue& queue;
		std::optional<uint32_t> family;

		explicit QueueFamily(VkQueue& queue) : queue(queue) { }
	};

	class Surface;

	class Device
	{
	public:
		NON_COPIABLE(Device);

		Device(VkPhysicalDevice physicalDevice, const Surface& surface);
		~Device();

		void WaitIdle() const;

		VkDevice Get() const { return m_device; }

		VkPhysicalDevice GetPhysical() const { return m_physicalDevice; }

		const class Surface& GetSurface() const { return m_surface; }

	private:
		std::vector<QueueFamily> FindQueueFamilies(VkPhysicalDevice device);
		static bool CheckDeviceExtensionSupport(VkPhysicalDevice physicalDevice);


	private:
		static const std::vector<const char*> RequiredExtensions;
		const Surface& m_surface;
		VkPhysicalDevice m_physicalDevice{};
		VkDevice m_device;

	public:
		uint32_t GraphicsFamilyIndex{};
		uint32_t PresentFamilyIndex{};

		VkQueue GraphicsQueue{};
		VkQueue PresentQueue{};
	};
}