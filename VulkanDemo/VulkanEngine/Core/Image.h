#pragma once

#include <vulkan/vulkan.h>
#include "UniqueHandle.h"

namespace Rendering
{
	class Device;

	class Image
	{
	public:
		Image();
	private:
		UniqueHandle<VkImage> m_handle;
	};
}