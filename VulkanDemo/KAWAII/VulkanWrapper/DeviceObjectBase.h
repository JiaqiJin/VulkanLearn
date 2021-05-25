#pragma once
#include "../Base/Base.h"
#include "Device.h"

namespace RHI
{
	template <class T>
	class DeviceObjectBase : public SelfRefBase<T>
	{
	public:
		virtual bool Init(const std::shared_ptr<Device>& pDevice, const std::shared_ptr<T>& pSelf)
		{
			if (!SelfRefBase::Init(pSelf))
				return false;

			if (!pDevice.get())
				return false;

			m_pDevice = pDevice;
			return true;
		}

		virtual ~DeviceObjectBase() {}

	protected:
		const std::shared_ptr<Device> GetDevice() const { return m_pDevice; }

	protected:
		std::shared_ptr<Device> m_pDevice;
	};
}