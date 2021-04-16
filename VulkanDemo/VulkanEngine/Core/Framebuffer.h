#pragma once

#include <vulkan/vulkan.h>
#include "UniqueHandle.h"

/*
* Wrapper class for VkFramebuffer
* Framebuffer represent a link to actual images that can be used as attachment. 
* Framebuffer is another layer of top Images and groups of ImageVies bounds during rendering of a specific Render Pass
*/
namespace Rendering
{
	class RenderPass;
	class ImageView;
	class Device;

	class Framebuffer
	{
	public:
		Framebuffer(const Device& device, ImageView const& colorImageView, ImageView const& depthImageView,
			const RenderPass& renderPass, VkExtent2D extent);
		~Framebuffer();

		Framebuffer(const Framebuffer&) = default;
		Framebuffer(Framebuffer&&) = default;
		Framebuffer& operator=(const Framebuffer&) = default;
		Framebuffer& operator=(Framebuffer&&) = default;

		VkFramebuffer getHandle() const { return m_handle; }
	private:
		UniqueHandle<VkFramebuffer> m_handle;

		const Device& m_device;
	};
}