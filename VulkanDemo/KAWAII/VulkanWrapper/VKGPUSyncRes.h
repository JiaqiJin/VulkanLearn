#pragma once

#include <vulkan/vulkan.h>
#include "VKFenceGuardRes.h"
#include <set>
#include <memory>
#include <vector>

namespace RHI
{
	class Semaphore;
	class CommandBuffer;

	class VKGPUSyncRes : public VKFenceGuardRes
	{
	public:
		VKGPUSyncRes(const std::shared_ptr<Device>& pDevice);

	private:
		std::set<std::shared_ptr<Semaphore>> m_guardSemaphores;
	};
}