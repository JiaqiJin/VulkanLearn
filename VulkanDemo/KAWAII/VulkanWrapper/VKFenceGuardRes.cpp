#include "VKFenceGuardRes.h"
#include "Semaphore.h"
#include "Fence.h"

namespace RHI
{
	VKFenceGuardRes::VKFenceGuardRes(const std::shared_ptr<Device>& pDevice)
		: m_pDevice(pDevice)
	{

	}

	void VKFenceGuardRes::WaitForRelease()
	{
		if (m_pGuardFence.expired())
			return;

		m_pGuardFence.lock()->Wait();
	}
}