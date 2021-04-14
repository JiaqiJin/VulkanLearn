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
	class Pipeline 
	{
	public:
		Pipeline(); // TODO

		Pipeline(const Pipeline&) = default;
		Pipeline(Pipeline&&) = default;
		Pipeline& operator=(const Pipeline&) = default;
		Pipeline& operator=(Pipeline&&) = default;

		VkPipeline getHandle() const { return m_handle; }

	private:
		UniqueHandle<VkPipeline> m_handle;
	};
}