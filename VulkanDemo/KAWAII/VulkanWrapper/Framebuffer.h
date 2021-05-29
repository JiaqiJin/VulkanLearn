#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include <vector>

namespace RHI
{
	class Device;
	class RenderPass;

	class FrameBuffer
	{
	public:
		FrameBuffer(const std::shared_ptr<Device> pDevice);
		~FrameBuffer();

		bool Init();

		// Getters
		VkFramebuffer GetDeviceHandle() { return m_framebuffer; }
		VkFramebufferCreateInfo GetFramebufferInfo() const { return m_info; }
	private:
		std::shared_ptr<Device> m_pDevice;

		VkFramebuffer m_framebuffer;
		VkFramebufferCreateInfo m_info;
	};
}