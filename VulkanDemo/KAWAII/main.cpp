#include <windows.h>
#include "ApplicationEntry/ApplicationEntry.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	AppEntry::GetInstance()->HandleMsg(hWnd, uMsg, wParam, lParam);
	return (DefWindowProc(hWnd, uMsg, wParam, lParam));
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	AppEntry::GetInstance()->InitVulkan(hInstance, WndProc);
	AppEntry::GetInstance()->Update();
	AppEntry::Free();
	return 0;
}