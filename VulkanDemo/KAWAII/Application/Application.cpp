#include "Application.h"
#include "../Common/Macro.h"
#include "../Common/Logger.h"
#include "../Math/Math.h"

#include <math.h>
#include <chrono>
#include <sstream>
#include <fstream>
#include <array>

void Application::InitVulkanInstance()
{
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "VulkanLearn";
	appInfo.apiVersion = (((1) << 22) | ((2) << 12) | (154));

	//Need surface extension to create surface from device
	std::vector<const char*> extensions = { EXTENSION_VULKAN_SURFACE };
	std::vector<const char*> layers;
#if defined(_WIN32)
	extensions.push_back(EXTENSION_VULKAN_SURFACE_WIN32);
#endif
#if defined(_DEBUG)
	layers.push_back(EXTENSION_VULKAN_VALIDATION_LAYER);
	extensions.push_back(EXTENSION_VULKAN_DEBUG_REPORT);
#endif
	VkInstanceCreateInfo instCreateInfo = {};
	instCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instCreateInfo.pApplicationInfo = &appInfo;
	instCreateInfo.enabledExtensionCount = (int32_t)extensions.size();
	instCreateInfo.ppEnabledExtensionNames = extensions.data();
	instCreateInfo.enabledLayerCount = (int32_t)layers.size();
	instCreateInfo.ppEnabledLayerNames = layers.data();

	m_pVulkanInstance = std::make_shared<RHI::Instance>(instCreateInfo);
	assert(m_pVulkanInstance != nullptr);
}

void Application::InitPhysicalDevice(HINSTANCE hInstance, HWND hWnd)
{
	m_pPhysicalDevice = std::make_shared<RHI::PhysicalDevice>(m_pVulkanInstance, hInstance, hWnd);
	ASSERTION(m_pPhysicalDevice != nullptr);
	//m_pDevice = std::make_shared<RHI::Device>(m_pVulkanInstance, m_pPhysicalDevice);
}

void Application::InitVulkanDevice()
{
	//m_pDevice = RHI::Device::Create(m_pVulkanInstance, m_pPhysicalDevice);
	m_pDevice = std::make_shared<RHI::Device>(m_pVulkanInstance, m_pPhysicalDevice);
	ASSERTION(m_pDevice != nullptr);
}

void Application::SetupWindow(HINSTANCE hinstance, WNDPROC wndproc)
{
	m_hPlatformInst = hinstance;

	bool fullscreen = false;

	WNDCLASSEX wndClass;

	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = wndproc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hinstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = (PROJECT_NAME);
	wndClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

	if (!RegisterClassEx(&wndClass))
	{
		std::cout << "Could not register window class!\n";
		fflush(stdout);
		exit(1);
	}

	int screenWidth = 1020;//GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = 1080;//GetSystemMetrics(SM_CYSCREEN);

	if (fullscreen)
	{
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = screenWidth;
		dmScreenSettings.dmPelsHeight = screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		// TODO
	}

	DWORD dwExStyle;
	DWORD dwStyle;

	if (fullscreen)
	{
		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	}

	RECT windowRect;
	windowRect.left = 0L;
	windowRect.top = 0L;
	windowRect.right = fullscreen ? (long)screenWidth : (long)screenWidth; // TODO
	windowRect.bottom = fullscreen ? (long)screenHeight : (long)screenHeight;

	AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

	std::string windowTitle = PROJECT_NAME;
	m_hWindow = CreateWindowEx(0,
		(PROJECT_NAME),
		(windowTitle.c_str()),
		dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		0,
		0,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		hinstance,
		NULL);

	if (!fullscreen)
	{
		// Center on screen
		uint32_t x = (GetSystemMetrics(SM_CXSCREEN) - windowRect.right) / 2;
		uint32_t y = (GetSystemMetrics(SM_CYSCREEN) - windowRect.bottom) / 2;
		SetWindowPos(m_hWindow, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}

	if (!m_hWindow)
	{
		printf("Could not create window!\n");
		fflush(stdout);
		exit(1);
	}

	ShowWindow(m_hWindow, SW_SHOW);
	SetForegroundWindow(m_hWindow);
	SetFocus(m_hWindow);
}

void Application::HandleMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Vector2d mouseUV =
	{
		(float)LOWORD(lParam) / 1440, (1024 - (float)HIWORD(lParam)) / 1024
	};

	switch (uMsg)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case KEY_ESCAPE:
			PostQuitMessage(0);
			break;
		default:
			
			break;
		}
		break;
	case WM_KEYUP:
		
		break;
	case WM_LBUTTONDOWN:
		
		break;
	case WM_LBUTTONUP:
	
		break;
	case WM_RBUTTONDOWN:
		
		break;
	case WM_RBUTTONUP:

		break;
	case WM_MOUSEMOVE:
		
		break;
	}
}

void Application::Update()
{
	static uint32_t frameCount = 0;
	static double fpsTimer = 0;
	static std::chrono::time_point<std::chrono::steady_clock> startTime, endTime, initTime;
	MSG msg;
	bool quitMessageReceived = false;
	while (!quitMessageReceived)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				quitMessageReceived = true;
				return;
			}
		}
		auto endTime = std::chrono::high_resolution_clock::now();

		if (fpsTimer > 1000.0)
		{
			std::stringstream ss;
			ss << "Elapsed Time:" << 1000.0 / frameCount;
			SetWindowText(m_hWindow, (ss.str().c_str()));
			fpsTimer = 0.0;
			frameCount = 0;
		}
	}
}

//void Application::testObject()
//{
//	uint32_t i = 1;
//	m_CommandPool = std::make_shared<RHI::CommandPool>(m_pDevice, (RHI::PhysicalDevice::QueueFamily)i, RHI::CommandPool::CBPersistancy::COUNT);
//	//RHI::CommandBuffer::CBLevel::PRIMARY
//	m_CommandBuffer = m_CommandPool->AllocateCommandBuffer(RHI::CommandBuffer::CBLevel::PRIMARY);
//	m_fence = std::make_shared<RHI::Fence>(m_pDevice);
//	m_pQueue = std::make_shared<RHI::Queue>
//		(m_pDevice, (RHI::PhysicalDevice::QueueFamily)i);
//	m_pQueue->SubmitCommandBuffer(m_CommandBuffer, m_fence);
//	std::cout << "HI";
//}

void Application::InitVulkan(HINSTANCE hInstance, WNDPROC wndproc)
{
	Log::Init();
	K_INFO("Init Vulkan Renderer");
	SetupWindow(hInstance, wndproc);
	// TODO Init vulkan components
	InitVulkanInstance();
	InitPhysicalDevice(m_hPlatformInst, m_hWindow);
	InitVulkanDevice();
	RHI::GlobalDeviceObjects::GetInstance()->InitObjects(m_pDevice);

	//testObject();
}