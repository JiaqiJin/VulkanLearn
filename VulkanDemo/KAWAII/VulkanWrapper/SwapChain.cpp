#include "SwapChain.h"
#include "Device.h"
#include "Fence.h"

#include "../Common/Macro.h"
#include "../Common/Logger.h"

namespace RHI
{
	SwapChain::SwapChain(const std::shared_ptr<Device>& pDevice)
		: m_device(pDevice)
	{
		if (!Init())
			K_ERROR("Error Initialize Swap Chain");
	}

	bool SwapChain::Init()
	{
		GET_DEVICE_PROC_ADDR(m_device->GetDeviceHandle(), CreateSwapchainKHR);
		GET_DEVICE_PROC_ADDR(m_device->GetDeviceHandle(), DestroySwapchainKHR);
		GET_DEVICE_PROC_ADDR(m_device->GetDeviceHandle(), GetSwapchainImagesKHR);
		GET_DEVICE_PROC_ADDR(m_device->GetDeviceHandle(), AcquireNextImageKHR);
		GET_DEVICE_PROC_ADDR(m_device->GetDeviceHandle(), QueuePresentKHR);

		//Prefer mailbox mode if present, it's the lowest latency non-tearing present  mode
		VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;
		//for (size_t i = 0; i < m_pDevice->GetPhysicalDevice()->GetPresentModes().size(); i++)
		//{
		//	if (m_pDevice->GetPhysicalDevice()->GetPresentModes()[i] == VK_PRESENT_MODE_MAILBOX_KHR)
		//	{
		//		swapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
		//		break;
		//	}
		//	if ((swapchainPresentMode != VK_PRESENT_MODE_MAILBOX_KHR) && (m_pDevice->GetPhysicalDevice()->GetPresentModes()[i] == VK_PRESENT_MODE_IMMEDIATE_KHR))
		//	{
		//		swapchainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
		//	}
		//}

		//Don't know what's this
		VkSurfaceTransformFlagsKHR preTransform;
		if (m_device->GetPhysicalDevice()->GetSurfaceCap().supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
		{
			preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		}
		else
		{
			preTransform = m_device->GetPhysicalDevice()->GetSurfaceCap().currentTransform;
		}

		VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
		swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainCreateInfo.surface = m_device->GetPhysicalDevice()->GetSurfaceHandle();
		swapchainCreateInfo.minImageCount = 3;
		swapchainCreateInfo.presentMode = swapchainPresentMode;
		swapchainCreateInfo.preTransform = (VkSurfaceTransformFlagBitsKHR)preTransform;
		swapchainCreateInfo.clipped = true;
		swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swapchainCreateInfo.imageArrayLayers = 1;
		swapchainCreateInfo.imageExtent.width = m_device->GetPhysicalDevice()->GetSurfaceCap().currentExtent.width;
		swapchainCreateInfo.imageExtent.height = m_device->GetPhysicalDevice()->GetSurfaceCap().currentExtent.height;
		swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchainCreateInfo.imageFormat = m_device->GetPhysicalDevice()->GetSurfaceFormat().format;
		swapchainCreateInfo.imageColorSpace = m_device->GetPhysicalDevice()->GetSurfaceFormat().colorSpace;
		swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

		RETURN_FALSE_VK_RESULT(m_fpCreateSwapchainKHR(m_device->GetDeviceHandle(), &swapchainCreateInfo, nullptr, &m_swapchain));

		return true;
	}

	SwapChain::~SwapChain()
	{
		if (m_device.get())
			vkDestroySwapchainKHR(m_device->GetDeviceHandle(), m_swapchain, nullptr);
	}
}