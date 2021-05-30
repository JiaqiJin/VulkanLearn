#include "Fence.h"
#include "Device.h"

#include "../Common/Macro.h"
#include "../Common/Logger.h"

namespace RHI
{
	Fence::Fence(const std::shared_ptr<Device>& pDevice)
		: m_device(pDevice)
	{
		if (!Init())
			K_ERROR("Error Initialize Fence");
	}

	bool Fence::Init()
	{

		VkFenceCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		CHECK_VK_ERROR(vkCreateFence(m_device->GetDeviceHandle(), &info, nullptr, &m_fence));
		m_fenceState = FenceState::READ_FOR_USE;
		return true;
	}

	Fence::~Fence()
	{
		vkDestroyFence(m_device->GetDeviceHandle(), m_fence, nullptr);
	}

	// We manually need to restore the fence to the unsignaled state by resetting it with the vkResetFences call.
	bool Fence::Reset()
	{
		if (m_fenceState != FenceState::SIGNALED)
			return false;

		CHECK_VK_ERROR(vkResetFences(m_device->GetDeviceHandle(), 1, &m_fence));
		m_fenceState = FenceState::READ_FOR_USE;

		return true;
	}

	// The vkWaitForFences function takes an array of fences and waits for either any or all of them to be signaled before returning
	bool Fence::Wait()
	{
		if (m_fenceState != FenceState::READ_FOR_SIGNAL)
			return false;

		CHECK_VK_ERROR(vkWaitForFences(m_device->GetDeviceHandle(), 1, &m_fence, VK_TRUE, UINT64_MAX));
		m_fenceState = FenceState::SIGNALED;

		// TODO

		return true;
	}
}