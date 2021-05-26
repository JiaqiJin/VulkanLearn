#include "Fence.h"

namespace RHI
{
	Fence::Fence(const std::shared_ptr<Device>& pDevice)
	{
		std::shared_ptr<Fence> pFence = std::make_shared<Fence>(pDevice);
		if (!Init(pDevice, pFence))
			K_ERROR("Error Initialize Fence");
	}

	bool Fence::Init(const std::shared_ptr<Device>& pDevice, const std::shared_ptr<Fence>& pSelf)
	{
		if (!DeviceObjectBase::Init(pDevice, pSelf))
			return false;

		VkFenceCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		CHECK_VK_ERROR(vkCreateFence(GetDevice()->GetDeviceHandle(), &info, nullptr, &m_fence));
		m_fenceState = FenceState::READ_FOR_USE;
		return true;
	}

	Fence::~Fence()
	{
		vkDestroyFence(GetDevice()->GetDeviceHandle(), m_fence, nullptr);
	}

	// We manually need to restore the fence to the unsignaled state by resetting it with the vkResetFences call.
	bool Fence::Reset()
	{
		if (m_fenceState != FenceState::SIGNALED)
			return false;

		CHECK_VK_ERROR(vkResetFences(m_pDevice->GetDeviceHandle(), 1, &m_fence));
		m_fenceState = FenceState::READ_FOR_USE;

		return true;
	}

	// The vkWaitForFences function takes an array of fences and waits for either any or all of them to be signaled before returning
	bool Fence::Wait()
	{
		if (m_fenceState != FenceState::READ_FOR_SIGNAL)
			return false;

		CHECK_VK_ERROR(vkWaitForFences(GetDevice()->GetDeviceHandle(), 1, &m_fence, VK_TRUE, UINT64_MAX));
		m_fenceState = FenceState::SIGNALED;

		// TODO

		return true;
	}
}