#pragma once

#include <vulkan/vulkan.h>
#include "UniqueHandle.h"

/*
* Wrapper class for VkSampler
* Texure usually access throught sampler, which apply filtering transformation to compute the final color
*/
namespace Rendering
{
	class Device;

	class Sampler
	{
	public:
		Sampler(const Device& device);
		~Sampler();

		Sampler(const Sampler&) = default;
		Sampler(Sampler&&) = default;
		Sampler& operator=(const Sampler&) = default;
		Sampler& operator=(Sampler&&) = default;

		VkSampler getHandle() const { return m_handle; }

	private:
		UniqueHandle<VkSampler> m_handle;
		const Device& m_device;
	};
}