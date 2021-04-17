#include "Renderer.h"

#include <array>

#include "../glm.h"
#include "../Application.h"

#include "Device.h"
#include "CommandBuffers.h"
#include "Queue.h"
#include "Swapchain.h"
#include "RenderPass.h"
#include "Framebuffer.h"
#include "Pipeline.h"
#include "Image.h"
#include "ImageView.h"
#include "Utils.h"
#include "ShaderModule.h"
#include "SwapChainSupport.h"
#include "QueueFamily.h"
#include "CommandPool.h"
#include "CommandBuffers.h"
#include "../imgui/imgui_impl_vulkan.h"
#include "../imgui/imgui.h"


namespace Rendering
{
	Renderer::Renderer(Application const& app)
		: Object(app)
	{

	}

	Renderer::~Renderer() = default;
}