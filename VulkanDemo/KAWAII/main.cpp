#include <Windows.h>
#include "Application/Application.h"
#include <wrl.h>

#pragma comment(lib, "runtimeobject.lib")

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Application::GetInstance()->HandleMsg(hWnd, uMsg, wParam, lParam);
	return (DefWindowProc(hWnd, uMsg, wParam, lParam));
}

//int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
//{
//	Microsoft::WRL::Wrappers::RoInitializeWrapper InitializeWinRT(RO_INIT_MULTITHREADED);
//	//ASSERT_SUCCEEDED(InitializeWinRT);
//
//	HINSTANCE hInst = GetModuleHandle(0);
//
//	AppEntry::GetInstance()->InitVulkan(hInstance, WndProc);
//	AppEntry::GetInstance()->Update();
//	AppEntry::Free();
//	return 0;
//}

int main(int argc, const char* argv[])
{
	Microsoft::WRL::Wrappers::RoInitializeWrapper InitializeWinRT(RO_INIT_MULTITHREADED);

	HINSTANCE hInst = GetModuleHandle(0);

	Application::GetInstance()->InitVulkan(hInst, WndProc);
	Application::GetInstance()->Update();
	Application::Free();

	return 0;
}
