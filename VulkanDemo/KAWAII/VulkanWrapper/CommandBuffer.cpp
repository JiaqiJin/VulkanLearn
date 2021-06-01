#include "CommandBuffer.h"
#include "CommandPool.h"
#include "Device.h"
#include "RenderPass.h"
#include "Framebuffer.h"
#include "PipelineBase.h"
#include "PipelineLayout.h"

#include "../Common/Macro.h"
#include "../Common/Logger.h"

namespace RHI
{
	CommandBuffer::CommandBuffer(const std::shared_ptr<Device>& pDevice, const std::shared_ptr<CommandPool>& pCmdPool, CBLevel level)
		: m_pDevice(pDevice), m_pCommandPool(pCmdPool), m_level(level)
	{
		m_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		m_info.commandPool = pCmdPool->GetDeviceHandle();
		m_info.commandBufferCount = 1;
		switch (level)
		{
		case RHI::CommandBuffer::CBLevel::PRIMARY:
			m_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			break;
		case RHI::CommandBuffer::CBLevel::SECONDARY:
			m_info.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
			break;
		default:
			ASSERTION(false);
			K_ERROR("CommandBuffer INFO Level ERROR TYPE");
			break;
		}

		if(!Init(m_info))
			K_ERROR("Error Initialize Command Buffer");
	}

	bool CommandBuffer::Init(const VkCommandBufferAllocateInfo& info)
	{
		//m_info = info;
		CHECK_VK_ERROR(vkAllocateCommandBuffers(m_pDevice->GetDeviceHandle(), &m_info, &m_commandBuffer));

		return true;
	}

	void CommandBuffer::StartPrimaryRecording()
	{
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = m_pCommandPool->GetInfo().flags & VK_COMMAND_POOL_CREATE_TRANSIENT_BIT ? VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT : VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		CHECK_VK_ERROR(vkBeginCommandBuffer(m_commandBuffer, &beginInfo));
	}

	void CommandBuffer::EndPrimaryRecording()
	{
		CHECK_VK_ERROR(vkEndCommandBuffer(m_commandBuffer));
	}

	// https://github.com/KhronosGroup/Vulkan-Samples/blob/master/samples/performance/command_buffer_usage/command_buffer_usage_tutorial.md
	void CommandBuffer::StartSecondaryRecording(const std::shared_ptr<RenderPass>& pRenderPass, 
		uint32_t subpassIndex, const std::shared_ptr<FrameBuffer>& pFrameBuffer)
	{
		VkCommandBufferInheritanceInfo inheritanceInfo = {};
		inheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
		inheritanceInfo.renderPass = pRenderPass->GetDeviceHandle();
		inheritanceInfo.subpass = subpassIndex;
		inheritanceInfo.framebuffer = pFrameBuffer->GetDeviceHandle();

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT | (m_pCommandPool->GetInfo().flags & VK_COMMAND_POOL_CREATE_TRANSIENT_BIT ? VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT : VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);
		beginInfo.pInheritanceInfo = &inheritanceInfo;
		CHECK_VK_ERROR(vkBeginCommandBuffer(m_commandBuffer, &beginInfo));
	}

	void CommandBuffer::EndSecondaryRecording()
	{
		CHECK_VK_ERROR(vkEndCommandBuffer(m_commandBuffer));
	}

	// Basic drawing commands
	void CommandBuffer::PrepareNormalDrawCommands(const DrawCmdData& data)
	{
		VkCommandBufferBeginInfo cmdBeginInfo = {};
		cmdBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		CHECK_VK_ERROR(vkBeginCommandBuffer(m_commandBuffer, &cmdBeginInfo));

		// Starting a render pass
		VkRenderPassBeginInfo renderPassBeginInfo = {};
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.clearValueCount = (uint32_t)data.clearValues.size();
		renderPassBeginInfo.pClearValues = data.clearValues.data();
		renderPassBeginInfo.renderPass = data.pRenderPass->GetDeviceHandle();
		renderPassBeginInfo.framebuffer = data.pFrameBuffer->GetDeviceHandle();
		renderPassBeginInfo.renderArea.extent.width = m_pDevice->GetPhysicalDevice()->GetSurfaceCap().currentExtent.width;
		renderPassBeginInfo.renderArea.extent.height = m_pDevice->GetPhysicalDevice()->GetSurfaceCap().currentExtent.height;
		renderPassBeginInfo.renderArea.offset.x = 0;
		renderPassBeginInfo.renderArea.offset.y = 0;

		vkCmdBeginRenderPass(m_commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport =
		{
			0, 0,
			(float)m_pDevice->GetPhysicalDevice()->GetSurfaceCap().currentExtent.width, (float)m_pDevice->GetPhysicalDevice()->GetSurfaceCap().currentExtent.height,
			0, 1
		};

		VkRect2D scissorRect =
		{
			0, 0,
			m_pDevice->GetPhysicalDevice()->GetSurfaceCap().currentExtent.width, m_pDevice->GetPhysicalDevice()->GetSurfaceCap().currentExtent.height
		};

		vkCmdSetViewport(GetDeviceHandle(), 0, 1, &viewport);
		vkCmdSetScissor(GetDeviceHandle(), 0, 1, &scissorRect);

		// TODO

		m_drawCmdData = data;
	}

	void CommandBuffer::BindPipeline(const std::shared_ptr<PipelineBase>& pPipeline)
	{
		vkCmdBindPipeline(GetDeviceHandle(), pPipeline->GetPipelineBindingPoint(), pPipeline->GetDeviceHandle());
	}

	void CommandBuffer::BeginRenderPass(const std::shared_ptr<FrameBuffer>& pFrameBuffer,
		const std::shared_ptr<RenderPass>& pRenderPass, const std::vector<VkClearValue>& clearValues, bool includeSecondary)
	{
		VkRenderPassBeginInfo renderPassBeginInfo = {};
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.clearValueCount = (uint32_t)clearValues.size();
		renderPassBeginInfo.pClearValues = clearValues.data();
		renderPassBeginInfo.renderPass = pRenderPass->GetDeviceHandle();
		renderPassBeginInfo.framebuffer = pFrameBuffer->GetDeviceHandle();
		renderPassBeginInfo.renderArea.extent.width = pFrameBuffer->GetFramebufferInfo().width;
		renderPassBeginInfo.renderArea.extent.height = pFrameBuffer->GetFramebufferInfo().height;
		renderPassBeginInfo.renderArea.offset.x = 0;
		renderPassBeginInfo.renderArea.offset.y = 0;

		VkSubpassContents contents = includeSecondary ? VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS : VK_SUBPASS_CONTENTS_INLINE;
		vkCmdBeginRenderPass(m_commandBuffer, &renderPassBeginInfo, contents);
	}

	void CommandBuffer::EndRenderPass()
	{
		vkCmdEndRenderPass(m_commandBuffer);
	}

	void CommandBuffer::Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
	{
		vkCmdDraw(m_commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
	}

	void CommandBuffer::Execute(const std::vector<std::shared_ptr<CommandBuffer>>& cmdBuffers)
	{
		std::vector<VkCommandBuffer> cmds;
		for (auto& cmd : cmdBuffers)
		{
			cmds.push_back(cmd->GetDeviceHandle());
		}

		vkCmdExecuteCommands(m_commandBuffer, (uint32_t)cmds.size(), cmds.data());
	}

	void CommandBuffer::PushConstant(const std::shared_ptr<PipelineLayout>& pPipelineLayout, 
		VkShaderStageFlags shaderFlag, uint32_t offset, uint32_t size, const void* pData)
	{
		vkCmdPushConstants(GetDeviceHandle(), pPipelineLayout->GetDeviceHandle(), shaderFlag, offset, size, pData);
	}

	void CommandBuffer::SetViewports(const std::vector<VkViewport>& viewports)
	{
		vkCmdSetViewport(m_commandBuffer, 0, (uint32_t)viewports.size(), viewports.data());
	}

	void CommandBuffer::SetScissors(const std::vector<VkRect2D>& scissors)
	{
		vkCmdSetScissor(m_commandBuffer, 0, (uint32_t)scissors.size(), scissors.data());
	}

	void CommandBuffer::NextSubpass()
	{
		vkCmdNextSubpass(m_commandBuffer, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
	}

	void CommandBuffer::Dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
	{
		vkCmdDispatch(m_commandBuffer, groupCountX, groupCountY, groupCountZ);
	}

	CommandBuffer::~CommandBuffer()
	{
		vkFreeCommandBuffers(m_pDevice->GetDeviceHandle(), m_pCommandPool->GetDeviceHandle(), 1, &m_commandBuffer);
	}
}