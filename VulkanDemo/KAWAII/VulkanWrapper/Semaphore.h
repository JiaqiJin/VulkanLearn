#pragma once

#include <vulkan/vulkan.h>
#include <memory>

namespace RHI
{
	class Device;

	class Semaphore
	{
	public:
		Semaphore(const std::shared_ptr<Device>& device);

		Semaphore(const Semaphore&) = delete;
		Semaphore(Semaphore&& other) = delete;
		Semaphore& operator=(const Semaphore&) = delete;
		Semaphore& operator=(Semaphore&&) = delete;

		~Semaphore();

		VkSemaphore GetSemaphore() const { return m_semaphore; }

	private:
		bool Init();

	private:
		std::shared_ptr<Device> m_device;
		VkSemaphore m_semaphore;
	};
}