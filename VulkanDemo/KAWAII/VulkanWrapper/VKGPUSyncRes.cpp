#include "VKGPUSyncRes.h"
#include "Semaphore.h"
#include "CommandBuffer.h"

namespace RHI
{
	VKGPUSyncRes::VKGPUSyncRes(const std::shared_ptr<Device>& pDevice)
		: VKFenceGuardRes(pDevice)
	{

	}
}