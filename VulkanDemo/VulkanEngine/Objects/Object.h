#pragma once

namespace Rendering
{
	class Application;

	class Instance;
	class Surface;
	class Device;
	class PhysicalDevice;
	struct SwapChainSupportDetails;

	class Object
	{
	public:
		Object(const Application& app);

		const Application& getApp() const { return m_app; }

		const Instance& getInstance() const;
		const Surface& getSurface() const;
		const Device& getDevice() const;
		const PhysicalDevice& getPhysicalDevice() const;
		const SwapChainSupportDetails& getPhysicalSwapChainSupportDetails() const;
	private:
		const Application& m_app;
	};
}