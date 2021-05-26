#pragma once

#include <vulkan/vulkan.h>

#include "DeviceObjectBase.h"
#include "PhysicalDevice.h"
#include "Device.h"

#include <vector>
#include <memory>

namespace RHI
{
	class Semaphore;
	class Queue;

	class SwapChain : public DeviceObjectBase<SwapChain>
	{
	public:
		SwapChain(const std::shared_ptr<Device>& pDevice);

		SwapChain(const SwapChain&) = delete;
		SwapChain& operator=(const SwapChain&) = delete;
		SwapChain& operator=(SwapChain&&) = delete;

		~SwapChain();

		const VkSwapchainKHR GetDeviceHandle() const { return m_swapchain; }

	private:
		bool Init(const std::shared_ptr<Device>& pDevice, const std::shared_ptr<SwapChain>& pSelf);

	protected:
		VkSwapchainKHR m_swapchain;
	};
}