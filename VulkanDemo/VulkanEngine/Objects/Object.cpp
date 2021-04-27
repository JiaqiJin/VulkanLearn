#include "Object.h"
#include "../Application.h"

namespace Rendering
{
	
	const Instance& Object::getInstance() const
	{
		return m_app.getInstance();
	}

	const Surface& Object::getSurface() const
	{
		return m_app.getSurface();
	}

	const Device& Object::getDevice() const
	{
		return m_app.getDevice();
	}

	const PhysicalDevice& Object::getPhysicalDevice() const
	{
		return m_app.getPhysicalDevice();
	}

	const PhysicalDeviceSurfaceParameters& Object::getPhysicalDeviceSurfaceParameters() const
	{
		return getApp().getPhysicalDeviceSurfaceParameters();
	}
}
