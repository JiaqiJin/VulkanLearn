#pragma once

#include "DeviceObjectBase.h"

namespace RHI
{
	class Queue;
	class VKFenceGuardRes;

	class Fence : public DeviceObjectBase<Fence>
	{
	public:
		enum FenceState
		{
			READ_FOR_USE,
			READ_FOR_SIGNAL,
			SIGNALED,
			COUNT
		};

	public:
		Fence(const std::shared_ptr<Device>& pDevice);

		~Fence();

		bool Reset();
		bool Wait();

		VkFence GetDeviceHandle() const { return m_fence; }
		FenceState GetFenceState() const { return m_fenceState; }

	private:
		bool Init(const std::shared_ptr<Device>& pDevice, const std::shared_ptr<Fence>& pSelf);

	private:
		VkFence m_fence;
		FenceState m_fenceState;

	};
}