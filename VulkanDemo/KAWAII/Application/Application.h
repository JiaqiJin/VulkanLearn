#pragma once
#include <vulkan/vulkan.h>

#include "../Common/Singleton.h"

#include "../VulkanWrapper/Instance.h"
#include "../VulkanWrapper/PhysicalDevice.h"
#include "../VulkanWrapper/Device.h"

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
};