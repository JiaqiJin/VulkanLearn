#pragma once

#include <vulkan/vulkan.h>

#include "PhysicalDevice.h"

#include <vector>
#include <memory>

namespace RHI
{
	class Semaphore;
	class Queue;
	class Device;

	class SwapChain
	{
	public:
		SwapChain(const std::shared_ptr<Device>& pDevice);

		SwapChain(const SwapChain&) = delete;
		SwapChain& operator=(const SwapChain&) = delete;
		SwapChain& operator=(SwapChain&&) = delete;

		~SwapChain();

		const VkSwapchainKHR GetDeviceHandle() const { return m_swapchain; }

	private:
		bool Init(const std::shared_ptr<Device>& pDevice);

	private:
		std::shared_ptr<Device> m_device;
		VkSwapchainKHR m_swapchain;
	};
}