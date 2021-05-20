#pragma once

#include "../Common/Singleton.h"

#include <vector>
#include <memory>
#include <Windows.h>

class AppEntry : public Singleton<AppEntry>
{
public:
	void InitVulkan(HINSTANCE hInstance, WNDPROC wndproc);
	void SetupWindow(HINSTANCE hInstance, WNDPROC wndproc);
	void HandleMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void Update();
private:
	HINSTANCE m_hPlatformInst;
	HWND m_hWindow;
};