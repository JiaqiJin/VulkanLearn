#pragma once

#include "DeviceObjectBase.h"

namespace RHI
{
	class Semaphore : public DeviceObjectBase<Semaphore>
	{
	public:
		Semaphore(const std::shared_ptr<Device> device);

		Semaphore(const Semaphore&) = delete;
		Semaphore(Semaphore&& other) = delete;
		Semaphore& operator=(const Semaphore&) = delete;
		Semaphore& operator=(Semaphore&&) = delete;

		~Semaphore();

		VkSemaphore GetSemaphore() const { return m_semaphore; }

	private:
		bool Init(const std::shared_ptr<Device>& pDevice, const std::shared_ptr<Semaphore>& pSelf);

	protected:
		VkSemaphore m_semaphore;
	};
}