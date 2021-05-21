#pragma once
#define VK_USE_PLATFORM_WIN32_KHR 1 
#include <vulkan/vulkan.h>

#include "../Common/Singleton.h"

#include "../VulkanWrapper/Instance.h"
#include "../VulkanWrapper/PhysicalDevice.h"

#include <vector>
#include <memory>
#include <Windows.h>

class AppEntry : public Singleton<AppEntry>
{
public:
	void InitVulkan(HINSTANCE hInstance, WNDPROC wndproc);
	void SetupWindow(HINSTANCE hInstance, WNDPROC wndproc);
	void HandleMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void InitVulkanInstance();
	void InitPhysicalDevice(HINSTANCE hInstance, HWND hWnd);

	void Update();

public:
	std::shared_ptr<RHI::Instance> m_pVulkanInstance;
	std::shared_ptr<RHI::PhysicalDevice> m_pPhysicalDevice;

private:
	HINSTANCE m_hPlatformInst;
	HWND m_hWindow;
};