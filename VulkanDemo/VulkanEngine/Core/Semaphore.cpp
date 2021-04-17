#include "Semaphore.h"
#include "Device.h"
#include <stdexcept>

namespace Rendering
{
	Semaphore::Semaphore(const Device& device)
		:m_device(device)
	{
		VkSemaphoreCreateInfo semaphoreCreateInfo{};
		semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		if (vkCreateSemaphore(m_device.getHandle(), &semaphoreCreateInfo, nullptr, &m_handle.get()) != VK_SUCCESS)
			throw std::runtime_error("failed to create semaphore!");
	}

	Semaphore::~Semaphore()
	{
		vkDestroySemaphore(m_device.getHandle(), m_handle, nullptr);
	}
}