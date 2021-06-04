#include "BufferBase.h"
#include "Device.h"

namespace RHI
{
	BufferBase::BufferBase(const std::shared_ptr<Device>& pDevice)
		: VKGPUSyncRes(pDevice)
	{

	}

}