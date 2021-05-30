#include "Semaphore.h"
#include "Device.h"

#include "../Common/Macro.h"
#include "../Common/Logger.h"

namespace RHI
{
	Semaphore::Semaphore(const std::shared_ptr<Device>& device)
		: m_device(device)
	{
		if (!Init())
			K_ERROR("Error Initialize Semaphore");
			//std::cout << "Error Initialize Semaphore" << std::endl;
	}

	bool Semaphore::Init()
	{
		VkSemaphoreCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		CHECK_VK_ERROR(vkCreateSemaphore(m_device->GetDeviceHandle(), &info, nullptr, &m_semaphore));

		return true;
	}

	Semaphore::~Semaphore()
	{
		vkDestroySemaphore(m_device->GetDeviceHandle(), m_semaphore, nullptr);
	}

}