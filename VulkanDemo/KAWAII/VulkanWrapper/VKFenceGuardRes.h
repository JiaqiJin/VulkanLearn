#pragma once

#include "DeviceObjectBase.h"
#include <set>
#include <memory>
#include <vector>

namespace RHI
{
	class Fence;
	class Semaphore;
	class Device;

	class VKFenceGuardRes 
	{
	public:
		VKFenceGuardRes(const std::shared_ptr<Device>& pDevice);

	protected:
		void GuardByFence(const std::shared_ptr<Fence>& pFence) { m_pGuardFence = pFence; }
		void MarkOccupied() { m_isOccupied = true; }
		void WaitForRelease();
		void Release() { m_isOccupied = false; }
		bool IsOccupied() const { return m_isOccupied; }

	protected:
		bool m_isOccupied;
		std::shared_ptr<Device> m_pDevice;
		std::weak_ptr<Fence> m_pGuardFence;

		friend class Fence;
	};
}