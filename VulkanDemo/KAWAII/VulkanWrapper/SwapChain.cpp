#include "SwapChain.h"
#include "Device.h"

namespace RHI
{
	SwapChain::SwapChain(const std::shared_ptr<Device>& pDevice)
	{
		std::shared_ptr<SwapChain> pSwapChain = std::make_shared<SwapChain>(pDevice);
		if (!Init(pDevice, pSwapChain))
			K_ERROR("Error Initialize Swap Chain");
	}

	bool SwapChain::Init(const std::shared_ptr<Device>& pDevice, const std::shared_ptr<SwapChain>& pSelf)
	{
		if (!DeviceObjectBase::Init(pDevice, pSelf))
			return false;

		VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;
		VkSurfaceTransformFlagsKHR preTransform;
		if (m_pDevice->GetPhysicalDevice()->GetSurfaceCap().supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
		{
			preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		}
		else
		{
			preTransform = m_pDevice->GetPhysicalDevice()->GetSurfaceCap().currentTransform;
		}

		VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
		swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainCreateInfo.surface = m_pDevice->GetPhysicalDevice()->GetSurfaceHandle();
		swapchainCreateInfo.minImageCount = 3;
		swapchainCreateInfo.presentMode = swapchainPresentMode;
		swapchainCreateInfo.preTransform = (VkSurfaceTransformFlagBitsKHR)preTransform;
		swapchainCreateInfo.clipped = true;
		swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swapchainCreateInfo.imageArrayLayers = 1;
		swapchainCreateInfo.imageExtent.width = m_pDevice->GetPhysicalDevice()->GetSurfaceCap().currentExtent.width;
		swapchainCreateInfo.imageExtent.height = m_pDevice->GetPhysicalDevice()->GetSurfaceCap().currentExtent.height;
		swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchainCreateInfo.imageFormat = m_pDevice->GetPhysicalDevice()->GetSurfaceFormat().format;
		swapchainCreateInfo.imageColorSpace = m_pDevice->GetPhysicalDevice()->GetSurfaceFormat().colorSpace;
		swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

		RETURN_FALSE_VK_RESULT(vkCreateSwapchainKHR(m_pDevice->GetDeviceHandle(), &swapchainCreateInfo, nullptr, &m_swapchain));
		// TODO
		return true;
	}

	SwapChain::~SwapChain()
	{
		if (m_pDevice.get())
			vkDestroySwapchainKHR(m_pDevice->GetDeviceHandle(), m_swapchain, nullptr);
	}
}