#pragma once

#include "Vulkan.h"

#include <vector>

namespace Rendering
{
	class Window;

	class Instance
	{
	public:
		NON_COPIABLE(Instance);

		Instance(const Window& window, const std::vector<const char*>& layers);
		~Instance();

		VkInstance Get() const { return m_instance; }

		const Window& GetWindow() const { return m_window; }

	private:
		std::vector<const char*> GetRequiredInstanceExtensions() const;
		bool CheckValidationLayerSupport(const std::vector<const char*>& validationLayers) const;
		void CheckPhysicalDevice();

		const Window& m_window;
		std::vector<VkPhysicalDevice> m_devices;
		VkInstance m_instance;
	};
}