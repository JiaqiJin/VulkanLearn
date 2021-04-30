#include "RasterizerGraphicsPipeline.h"

#include "Shader.h"
#include "Device.h"
// TODO
namespace Rendering
{
	RasterizerGraphicsPipeline::RasterizerGraphicsPipeline(const Device& device, const SwapChain& swapChain)
		: m_device(device), m_swapChain(swapChain)
	{
		// LOAD SHADERS
		const Shader vertexShader(m_device, "Vertex.vert.spv");
		const Shader fragShader(m_device, "Fragment.frag.spv");

		VkPipelineShaderStageCreateInfo shaderStages[] =
		{
			vertexShader.CreateShaderStage(VK_SHADER_STAGE_VERTEX_BIT),
			fragShader.CreateShaderStage(VK_SHADER_STAGE_FRAGMENT_BIT)
		};



		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			 
		VK_CHECK(vkCreateGraphicsPipelines(device.Get(), nullptr, 1, &pipelineInfo, nullptr, &m_pipeline),"Create graphics pipeline");
	}

	RasterizerGraphicsPipeline::~RasterizerGraphicsPipeline()
	{
		if (m_pipeline != nullptr)
		{
			vkDestroyPipeline(m_device.Get(), m_pipeline, nullptr);
			m_pipeline = nullptr;
		}
	}
}