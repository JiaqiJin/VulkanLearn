#pragma once

#include <vector>
#include <functional>
#include <memory>

#include "../Objects/Object.h"
#include "Semaphore.h"
#include "Fence.h"

namespace Rendering
{
    class CommandBuffers;
    class Swapchain;
    class RenderPass;
    class Pipeline;
    class Image;
    class ImageView;
    class Shader;
    class CommandPool;
    class CommandBuffer;

	class Renderer : Object
	{
    public:
        Renderer(Application const& app);
        ~Renderer();
    private:
        struct FrameResources
        {
            FrameResources(Application const& app);
            Rendering::Semaphore imageAvailableSemaphore;
            Rendering::Semaphore renderFinishedSemaphore;
            Rendering::Fence inFlightFence;
        };
	};
}