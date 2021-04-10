#include "Device.h"
#include "PhysicalDevice.h"
#include "QueueFamily.h"
#include "Queue.h"
#include <set>
#include <stdexcept>

namespace Rendering
{
	Device::Device()
	{
		
	}

	Device::~Device()
	{
		vkDestroyDevice(m_device, nullptr);
	}
}