#pragma once

#include <vector>
#include <functional>
#include <memory>

#include "../Objects/Object.h"
#include "Semaphore.h"
#include "Fence.h"
#include <vector>
#include <functional>
#include "CommandBuffer.h"
#include "Timer.h"
#include "Camera.h"
#include "Light.h"

namespace Rendering
{
    class CommandBuffers;
    class Swapchain;
    class RenderPass;
    class PipelineLayout;
    class Pipeline;
    class Image;
    class DeviceMemory;
    class ImageView;
    class DescriptorSetLayout;
    class Mesh;
    class ObjectInstance;
    class SceneObject;
    class Shader;
    class CommandPool;
    class CommandBuffer;
    class VertexLayout;

    class Renderer : Object
    {
    public:
        Renderer(const Application& app);
        ~Renderer();

        const Swapchain& getSwapchain() const { return *m_swapchain; }
        const RenderPass& getRenderPass() const { return *m_renderPass; }

        template<typename Func>
        void setWaitUntilWindowInForegroundCallback(Func&& func)
        {
            m_waitUntilWindowInForeground = func;
        }

        void addObject(std::shared_ptr<SceneObject> const& object);
        void setLight(std::shared_ptr<Light> const& light) { m_light = light; }

        Camera& getCamera() { return m_camera; }
        const Camera& getCamera() const { return m_camera; }

        void onFramebufferResized();
        void draw();

        float getAspect() const;
        float getLastFenceTime() const { return m_lastFenceTime; }
        float getCumulativeFenceTime() const { return m_cumulativeFenceTime; }
        void resetCumulativeFenceTime() { m_cumulativeFenceTime = 0.0f; }

    private:
        struct FrameResources
        {
            FrameResources(const Application& app);

            Semaphore imageAvailableSemaphore;
            Semaphore renderFinishedSemaphore;
            Fence inFlightFence;

            std::unique_ptr<CommandPool> commandPool;
            CommandBuffer commandBuffer;
        };

    private:
        void createSwapchain();
        void createSyncObjects();
        void recordCommandBuffer(std::size_t imageIndex, const FrameResources& frameResources);
        std::unique_ptr<Pipeline> createPipeline(const Shader& shader, const VertexLayout& vertexLayout);

        void recreateSwapchain();

        void onSwapchainCreated();

        void updateUniformBuffer(uint32_t currentImage);

    private:
        Camera m_camera;

        std::unique_ptr<Swapchain> m_swapchain;
        std::unique_ptr<RenderPass> m_renderPass;
        std::unique_ptr<PipelineLayout> m_pipelineLayout;

        std::unique_ptr<Image> m_depthImage;
        std::unique_ptr<DeviceMemory> m_depthImageMemory;
        std::unique_ptr<ImageView> m_depthImageView;

        std::vector<FrameResources> m_frameResources;

        std::size_t m_nextFrameResourcesIndex = 0;
        bool m_framebufferResized = false;

        std::function<void()> m_waitUntilWindowInForeground;

        std::vector<std::unique_ptr<ObjectInstance>> m_sceneObjects;
        std::shared_ptr<Light> m_light;

        std::unique_ptr<DescriptorSetLayout> m_descriptorSetLayout;

        Timer m_fenceTimer;
        float m_lastFenceTime = 0.0f;
        float m_cumulativeFenceTime = 0.0f;
    };
}