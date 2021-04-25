#pragma once
#include <vulkan/vulkan.h>
#include "UniqueHandle.h"

/*
* Wrapper class for graphics pipeline
* Pipeline stages : Input assembler, Vertex shader, Tessellation, Geomerty shader, Rasterization Fragment Shader, Color blending
* You need recreate pipeline from scratch if any shaders have been changed (need create a number of pipelines to represente all  
* different combinations of states of rendering operation)
*/
namespace Rendering
{
	class Shader;
	class Device;
	class ShaderModule;
	class RenderPass;
	class VertexLayout;
	class PipelineLayout;

	class Pipeline 
	{
	public:
		Pipeline(const Device& device, const PipelineLayout& pipelineLayout, const RenderPass& renderPass, VkExtent2D extent, const Shader& shader, const VertexLayout& vertexLayout); // TODO

		Pipeline(const Pipeline&) = default;
		Pipeline(Pipeline&&) = default;
		Pipeline& operator=(const Pipeline&) = default;
		Pipeline& operator=(Pipeline&&) = default;

		VkPipeline getHandle() const { return m_handle; }

		static void resetBoundPipeline() { ms_boundPipeline = nullptr; }

	private:
		UniqueHandle<VkPipeline> m_handle;
		const Device& m_device;

	private:
		static const Pipeline* ms_boundPipeline;
	};
}