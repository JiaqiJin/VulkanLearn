#pragma once

#include <vulkan/vulkan.h>

#include <memory>

namespace RHI
{
	class Device;
	class Queue;
	class VKFenceGuardRes;

	class Fence 
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
		bool Init(const std::shared_ptr<Device>& pDevice);

	private:
		VkFence m_fence;
		FenceState m_fenceState;
		std::shared_ptr<Device> m_device;
		friend class Queue;
	};
}