#pragma once
#include <vulkan/vulkan.h>
#include "UniqueHandle.h"
#include "../Objects/Object.h"
/*
* Wrapper class for graphics pipeline
* Pipeline stages : Input assembler, Vertex shader, Tessellation, Geomerty shader, Rasterization Fragment Shader, Color blending
* You need recreate pipeline from scratch if any shaders have been changed (need create a number of pipelines to represente all  
* different combinations of states of rendering operation)
*/
namespace Rendering
{
    class ShaderModule;
    class Shader;
    class PipelineLayout;
    class RenderPass;
    class VertexLayout;

    class Pipeline : public Object
    {
    public:
        explicit Pipeline(const Application& app, const PipelineLayout& layout, const RenderPass& renderPass,
            VkExtent2D extent, const Shader& shader, const VertexLayout& vertexLayout);
        ~Pipeline();

        void bind(VkCommandBuffer commandBuffer) const;

        Pipeline(const Pipeline&) = default;
        Pipeline(Pipeline&&) = default;
        Pipeline& operator=(const Pipeline&) = default;
        Pipeline& operator=(Pipeline&&) = default;

        VkPipeline getHandle() const { return m_handle; }

    public:
        static void resetBoundPipeline() { ms_boundPipeline = nullptr; }

    private:
        UniqueHandle<VkPipeline> m_handle;

    private:
        static Pipeline const* ms_boundPipeline;
    };
}