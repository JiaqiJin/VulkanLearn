#include "Framebuffer.h"
#include "Device.h"
#include "RenderPass.h"

#include "../Common/Macro.h"
#include "../Common/Logger.h"

namespace RHI
{
	FrameBuffer::FrameBuffer(const std::shared_ptr<Device>& pDevice)
		: m_pDevice(std::move(pDevice))
	{
		if(!Init())
			K_ERROR("Error Initialize FrameBuffer");
	}

	bool FrameBuffer::Init()
	{
		RETURN_FALSE_VK_RESULT(vkCreateFramebuffer(m_pDevice->GetDeviceHandle(), &m_info, nullptr, &m_framebuffer));
	}

	FrameBuffer::~FrameBuffer()
	{
		vkDestroyFramebuffer(m_pDevice->GetDeviceHandle(), m_framebuffer, nullptr);
	}
}