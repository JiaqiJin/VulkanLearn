#pragma once
#include <memory>
#include <vulkan/vulkan.h>
#include "../Common/Macro.h"

namespace RHI
{
	class Instance
	{
	public:
		~Instance();

		VkInstance GetInstance() { return m_vulkanInst; }
		const VkInstance GetInstance() const { return m_vulkanInst; }

		bool Init(const VkInstanceCreateInfo& createInfo);

	public:
		static std::shared_ptr<Instance> Create(const VkInstanceCreateInfo& createInfo);

	private:
		VkInstance m_vulkanInst;
	};
}