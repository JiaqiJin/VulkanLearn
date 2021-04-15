#pragma once

#include <vulkan/vulkan.h>
#include "UniqueHandle.h"

/*
* Wrapper class for VkRenderPass
* Tell to vulkan framebuffer to use while rendering (color and depth buffer, samples to use). 
* A single render pass consist multiple Subpasses and attachment references
*/
namespace Rendering
{
	class Device;
	class Swapchain;
	class PhysicalDevice;

	class RenderPass
	{
	public:
		RenderPass(const Device& device, const PhysicalDevice& physicalDevice, const Swapchain& swapChain);
		~RenderPass();

		RenderPass(const RenderPass&) = default;
		RenderPass(RenderPass&&) = default;
		RenderPass& operator=(const RenderPass&) = default;
		RenderPass& operator=(RenderPass&&) = default;

		VkRenderPass getHandle() const { return m_handle; }
		
	private:
		UniqueHandle<VkRenderPass> m_handle;
		const Device& m_device;
		VkFormat m_depthFormat;
	};
}