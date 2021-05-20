#include "Instance.h"
#include "../Common/Macro.h"

namespace RHI
{
	Instance::~Instance()
	{
		if (m_vulkanInst)
		{
			vkDestroyInstance(m_vulkanInst, nullptr);
		}
	}

	std::shared_ptr<Instance> Instance::Create(const VkInstanceCreateInfo& createInfo)
	{
		std::shared_ptr<Instance> pInstance = std::make_shared<Instance>();
		if (pInstance.get() && pInstance->Init(createInfo))
			return pInstance;

		return nullptr;
	}

	bool Instance::Init(const VkInstanceCreateInfo& createInfo)
	{
		RETURN_FALSE_VK_RESULT(vkCreateInstance(&createInfo, nullptr, &m_vulkanInst));

		return true;
	}
}
