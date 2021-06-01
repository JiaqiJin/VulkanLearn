#pragma once
#include <memory>
#include <vulkan/vulkan.h>
#include "../Common/Macro.h"

namespace RHI
{
	class Instance
	{
	public:
		Instance(VkInstanceCreateInfo createInfo);
		~Instance();

		Instance(const Instance&) = delete;
		Instance(Instance&&) = delete;
		Instance& operator=(const Instance&) = delete;
		Instance& operator=(Instance&&) = delete;

		VkInstance GetDeviceHandle() { return m_vulkanInst; }
		const VkInstance GetDeviceHandle() const { return m_vulkanInst; }

		bool Init(const VkInstanceCreateInfo&);

	private:
		VkInstance	m_vulkanInst;

#ifdef _DEBUG
		PFN_vkCreateDebugReportCallbackEXT				m_fpCreateDebugReportCallbackEXT;
		PFN_vkDebugReportMessageEXT						m_fpDebugReportMessageEXT;
		PFN_vkDestroyDebugReportCallbackEXT				m_fpDestroyDebugReportCallbackEXT;
		VkDebugReportCallbackEXT						m_debugCallback;
#endif
	};
}