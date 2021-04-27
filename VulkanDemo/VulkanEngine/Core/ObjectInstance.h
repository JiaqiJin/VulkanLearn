#pragma once

#include <vulkan/vulkan.h>
#include "../Objects/Object.h"
#include <memory>
#include <vector>

namespace Rendering
{
    class Buffer;
    class DeviceMemory;
    class DescriptorSets;
    class Texture;
    class Sampler;
    class DescriptorSetLayout;
    class PipelineLayout;
    class SceneObject;
    class Swapchain;
    class Pipeline;

    class ObjectInstance : public Object
    {
    public:
        ObjectInstance(const Application& app, const std::shared_ptr<SceneObject>& sceneObject,
            const DescriptorSetLayout& setLayout, VkDeviceSize uniformBufferSize);

        const SceneObject& getSceneObject() const { return *m_sceneObject; }

        const std::unique_ptr<DeviceMemory>& getUniformBufferMemory(std::size_t imageIndex) const { return m_uniformBuffersMemory[imageIndex]; }
        const std::unique_ptr<DescriptorSets>& getDescriptorSets() const { return m_descriptorSets; }

        void onSwapchainCreated(const Swapchain& swapchain);

        void copyToUniformBuffer(std::size_t index, void const* sourcePointer, std::size_t sourceSize) const;
        void bindDescriptorSet(VkCommandBuffer commandBuffer, std::size_t imageIndex, const PipelineLayout& pipelineLayout) const;

        void setPipeline(std::unique_ptr<Pipeline> pipeline) { m_pipeline = std::move(pipeline); }
        bool hasPipeline() const { return m_pipeline != nullptr; }
        void bindPipeline(VkCommandBuffer commandBuffer) const;

    public:
        static void resetBoundDescriptorSet() { ms_boundDescriptorSet = VK_NULL_HANDLE; }

    private:
        std::shared_ptr<SceneObject> m_sceneObject;
        const DescriptorSetLayout& m_setLayout;
        VkDeviceSize m_uniformBufferSize;

        std::size_t m_currentImageCount = 0;
        std::vector<std::unique_ptr<Buffer>> m_uniformBuffers;
        std::vector<std::unique_ptr<DeviceMemory>> m_uniformBuffersMemory;
        std::unique_ptr<DescriptorSets> m_descriptorSets;

        std::unique_ptr<Pipeline> m_pipeline;

    private:
        static VkDescriptorSet ms_boundDescriptorSet;
    };
}