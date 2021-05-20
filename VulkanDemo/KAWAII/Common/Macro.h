#pragma once

#include <assert.h>

#include <iostream>

#define EXTENSION_VULKAN_SURFACE "VK_KHR_surface"  
#define EXTENSION_VULKAN_SURFACE_WIN32 "VK_KHR_win32_surface"
#define EXTENSION_VULKAN_VALIDATION_LAYER "VK_LAYER_KHRONOS_validation"
#define EXTENSION_VULKAN_DEBUG_REPORT "VK_EXT_debug_report"
#define EXTENSION_VULKAN_SWAPCHAIN "VK_KHR_swapchain"
#define EXTENSION_SHADER_DRAW_PARAMETERS "VK_KHR_shader_draw_parameters"
#define EXTENSION_VULKAN_DRAW_INDIRECT_COUNT "VK_KHR_draw_indirect_count"
#define PROJECT_NAME "KAWAII"

#define UINT64_MAX       0xffffffffffffffffui64

#define TO_STRING(x) #x

#if defined(_WIN32)
#if defined(_DEBUG)
#define CHECK_VK_ERROR(vkExpress) { \
	VkResult result = vkExpress; \
	assert(result == VK_SUCCESS); \
	} 
#define RETURN_FALSE_VK_RESULT(vkExpress) { \
	VkResult result = vkExpress;	\
	if (result != VK_SUCCESS)	{	\
		std::cout << "Vulkan call failed, error code:" << result << std::endl;	\
		return false;	\
	}	\
}	
#define ASSERTION(express) assert(express);
#else
#define CHECK_VK_ERROR(vkExpress) vkExpress;
#define RETURN_FALSE_VK_RESULT(vkExpress) vkExpress;
#define CHECK_ERROR(vkExpress) vkExpress;
#define ASSERTION(express)
#endif
#endif

#define GET_INSTANCE_PROC_ADDR(inst, entrypoint)                        \
{                                                                       \
    m_fp##entrypoint = (PFN_vk##entrypoint) vkGetInstanceProcAddr(inst, "vk"#entrypoint); \
    if (m_fp##entrypoint == NULL)                                         \
	{																    \
        exit(1);                                                        \
    }                                                                   \
}

#define GET_DEVICE_PROC_ADDR(dev, entrypoint)                           \
{                                                                       \
    m_fp##entrypoint = (PFN_vk##entrypoint) vkGetDeviceProcAddr(dev, "vk"#entrypoint);   \
    if (m_fp##entrypoint == NULL)                                         \
	{																    \
        exit(1);                                                        \
    }                                                                   \
}

#define SAFE_DELETE(x) if ((x) != nullptr) { delete (x); (x) = nullptr; }

#define CREATE_OBJ(type, pRet) type* pRet = new type; \
if (!pRet || !pRet->Init()) \
{ \
	SAFE_DELETE(pRet); \
	return nullptr; \
}

#define EQUAL(type, x, y) ((((x) - (std::numeric_limits<type>::epsilon())) <= (y)) && (((x) + (std::numeric_limits<type>::epsilon())) >= (y)))

#define CONVERT2SINGLEVAL(src, dst, val) (dst.val = (float)src.val);
#define CONVERT2SINGLE(src, dst, val) (dst.val = src.val.SinglePrecision());

#define KEY_ESCAPE VK_ESCAPE 
#define KEY_F1 VK_F1
#define KEY_F2 VK_F2
#define KEY_F3 VK_F3
#define KEY_F4 VK_F4
#define KEY_F5 VK_F5
#define KEY_Q 0x51
#define KEY_E 0x45
#define KEY_W 0x57
#define KEY_A 0x41
#define KEY_S 0x53
#define KEY_D 0x44
#define KEY_P 0x50
#define KEY_SPACE 0x20
#define KEY_KPADD 0x6B
#define KEY_KPSUB 0x6D
#define KEY_B 0x42
#define KEY_F 0x46
#define KEY_L 0x4C
#define KEY_G 0x47
#define KEY_K 0x4B
#define KEY_N 0x4E
#define KEY_O 0x4F
#define KEY_T 0x54
#define KEY_R 0x52