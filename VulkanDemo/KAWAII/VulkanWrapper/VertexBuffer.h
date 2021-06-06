#pragma once

#include "Buffer.h"

#include <memory>
#include <vector>

namespace RHI
{
	class Device;

	class VertexBuffer : public Buffer
	{
	public:
		VertexBuffer(const std::shared_ptr<Device> pDevice, uint32_t numBytes);

	private:

	};
}