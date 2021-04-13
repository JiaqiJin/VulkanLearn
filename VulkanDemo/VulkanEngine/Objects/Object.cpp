#include "Object.h"
#include "../Application.h"

namespace Rendering
{
	Object::Object(const Application& app)
		: m_app(app)
	{

	}

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

	const SwapChainSupportDetails& Object::getPhysicalSwapChainSupportDetails() const
	{
		return m_app.getSwapChainSupportDetails();
	}
}
