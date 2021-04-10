#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <string>

#include "UniqueHandle.h"

namespace Rendering
{
	class Instance
	{
	public:
		Instance(std::string const& appName, std::vector<char const*> extensions, bool enableValidation, bool enableApiDump);
		~Instance();

		Instance(Instance const&) = default;
		Instance(Instance&&) = default;
		Instance& operator=(Instance const&) = default;
		Instance& operator=(Instance&&) = default;

		VkInstance getInstance() const { return m_instance; }

	private:
		void createInstance(std::string const& appName, std::vector<char const*> extensions, bool enableValidation, bool enableApiDump);

	private:
		UniqueHandle<VkInstance> m_instance;

		std::vector<VkLayerProperties> m_availableLayers;
		std::vector<char const*> m_availableLayerNames;

		std::vector<VkExtensionProperties> m_availableExtensions;
		std::vector<char const*> m_availableExtensionNames;
	};
}