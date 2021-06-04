#pragma once

#include "Buffer.h"

#include <memory>
#include <vector>

namespace RHI
{
	class Device;
	// Staging buffer : CPU accessible memory to upload data.
	// We use a buffer copy command to move  the data from staging buffer to other buffers(like vertex buffer, index buffer etc).
	class StagingBuffer : public Buffer
	{
	public:
		StagingBuffer(const std::shared_ptr<Device>& pDevice, uint32_t numBytes);
		StagingBuffer(const std::shared_ptr<Device>& pDevice, uint32_t numBytes, bool ReadableStagingBuffer);

	private:
		void* m_pData;
	};
}