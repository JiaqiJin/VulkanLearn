#pragma once
#include <vulkan/vulkan.h>

#include "../Common/Singleton.h"
#include "../VulkanWrapper/GlobalDeviceObjects.h"
#include "../VulkanWrapper/Instance.h"
#include "../VulkanWrapper/PhysicalDevice.h"
#include "../VulkanWrapper/Device.h"
#include "../VulkanWrapper/Semaphore.h"
#include "../VulkanWrapper/CommandBuffer.h"
#include "../VulkanWrapper/CommandPool.h"
#include "../VulkanWrapper/Queue.h"
#include "../VulkanWrapper/Fence.h"

#include <vector>
#include <memory>
#include <Windows.h>



class Application : public Singleton<Application>
{
public:
	void InitVulkan(HINSTANCE hInstance, WNDPROC wndproc);
	void SetupWindow(HINSTANCE hInstance, WNDPROC wndproc);
	void HandleMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void InitVulkanInstance();
	void InitPhysicalDevice(HINSTANCE hInstance, HWND hWnd);
	void InitVulkanDevice();

	void Update();

public:
	std::shared_ptr<RHI::Instance> m_pVulkanInstance;
	std::shared_ptr<RHI::PhysicalDevice> m_pPhysicalDevice;
	std::shared_ptr<RHI::Device> m_pDevice;

private:
	HINSTANCE m_hPlatformInst;
	HWND m_hWindow;

	// Test
private:
	//void testObject();
	//std::shared_ptr<RHI::Queue> m_pQueue;
	//std::shared_ptr<RHI::CommandBuffer> m_CommandBuffer;
	//std::shared_ptr<RHI::CommandPool> m_CommandPool;
	//std::shared_ptr<RHI::Fence> m_fence;
};