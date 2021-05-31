#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <memory>

namespace RHI
{
	class Device;
	class CommandPool;
	class RenderPass;
	class FrameBuffer;
	class PipelineBase;
	class PipelineLayout;

	class CommandBuffer
	{
	public:
		static const uint32_t MAX_INDIRECT_DRAW_COUNT = 256;

		enum class CBLevel
		{
			PRIMARY,//  Can be submitted to a queue for execution, but cannot be called from other command buffers.
			SECONDARY, // Cannot be submitted directly, but can be called from primary command buffers.
			COUNT
		};

		typedef struct _DrawCmdData
		{
			std::shared_ptr<RenderPass> pRenderPass;
			std::shared_ptr<PipelineBase> pPipeline;
			std::shared_ptr<FrameBuffer> pFrameBuffer;
			// Index Buffer, Vertex Buffer TODO

			std::vector<VkClearValue> clearValues;
		}DrawCmdData;

	public:
		CommandBuffer(const std::shared_ptr<Device>& pDevice, const std::shared_ptr<CommandPool>& pCmdPool, CBLevel level);

		~CommandBuffer();

		// Command buffer recording
		void StartPrimaryRecording();
		void EndPrimaryRecording();
		// Use secondary command buffers to allow multi-threaded render pass construction.
		void StartSecondaryRecording(const std::shared_ptr<RenderPass>& pRenderPass, uint32_t subpassIndex, const std::shared_ptr<FrameBuffer>& pFrameBuffer);
		void EndSecondaryRecording();

		// Starting a render pass with Basic drawing commands
		void PrepareNormalDrawCommands(const DrawCmdData& data);

		void BindPipeline(const std::shared_ptr<PipelineBase>& pPipeline);

		void BeginRenderPass(const std::shared_ptr<FrameBuffer>& pFrameBuffer, const std::shared_ptr<RenderPass>& pRenderPass, const std::vector<VkClearValue>& clearValues, bool includeSecondary = false);
		void EndRenderPass();

		// https://vkguide.dev/docs/chapter-3/push_constants/ 
		// Push constants let us send a small amount of data (it has a limited size) to the shader
		void PushConstant(const std::shared_ptr<PipelineLayout>& pPipelineLayout, VkShaderStageFlags shaderFlag, uint32_t offset, uint32_t size, const void* pData);

		void SetViewports(const std::vector<VkViewport>& viewports);
		void SetScissors(const std::vector<VkRect2D>& scissors);

		void Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);

		void NextSubpass();

		void Execute(const std::vector<std::shared_ptr<CommandBuffer>>& cmdBuffers);

		void Dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);

		// Gettes
		VkCommandBuffer GetDeviceHandle() const { return m_commandBuffer; }
		VkCommandBufferAllocateInfo GetAllocateInfo() const { return m_info; }
		std::shared_ptr<CommandPool> GetCommandPool() const { return m_pCommandPool; }
		CBLevel GetCommandBufferLevel() const { return m_level; }

	private:
		bool Init(const VkCommandBufferAllocateInfo& info);

	private:
		VkCommandBuffer m_commandBuffer;
		VkCommandBufferAllocateInfo m_info;

		std::shared_ptr<Device> m_pDevice;
		std::shared_ptr<CommandPool> m_pCommandPool;

		DrawCmdData m_drawCmdData;
		CBLevel m_level;
		friend class CommandPool;
	};
}