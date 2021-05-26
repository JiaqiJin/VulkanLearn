#include "Semaphore.h"
#include "Device.h"
#include "../Common/Logger.h"

namespace RHI
{
	Semaphore::Semaphore(const std::shared_ptr<Device> device)
	{
		std::shared_ptr<Semaphore> pSemaphore = std::make_shared<Semaphore>(device);
		if (!Init(device, pSemaphore))
			K_ERROR("Error Initialize Semaphore");
			//std::cout << "Error Initialize Semaphore" << std::endl;
	}

	bool Semaphore::Init(const std::shared_ptr<Device>& pDevice, const std::shared_ptr<Semaphore>& pSelf)
	{
		if (!DeviceObjectBase::Init(pDevice, pSelf))
			return false;

			VkSemaphoreCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
			CHECK_VK_ERROR(vkCreateSemaphore(GetDevice()->GetDeviceHandle(), &info, nullptr, &m_semaphore));

		return true;
	}

	Semaphore::~Semaphore()
	{
		vkDestroySemaphore(GetDevice()->GetDeviceHandle(), m_semaphore, nullptr);
	}

}