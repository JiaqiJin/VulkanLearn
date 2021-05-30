#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include <vector>
#include <mutex>

namespace RHI
{
	class Device;
	// Specify color and depth buffer, how many sampler to use and how their content shoul be handled throughout the rendering operation
	// In Vulkan, Render pass consists of one or more subpasses; for simple rendering operations.
	// Subpass : are subsequence rendering operation that depend on the framebuffer in previus passes. 
	class RenderPass
	{
	public:
		// A single render pass consist of multiple subpasses
		struct SubpassDef
		{
			std::vector<VkAttachmentReference> m_colorAttachmentRefs;
			VkAttachmentReference m_depthStencilAttachmentRef; // Attachment for depth and stencil data
			std::vector<VkAttachmentReference> m_inputAttachmentRefs; // Attachments that are read from a shader
			std::vector<uint32_t> m_preservAttachmentRefs; // Attachments that are not used by this subpass, but for which the data must be preserved
			std::vector<VkAttachmentReference> m_resolveAttachmentRefs; // Attachments used for multisampling color attachments
		};

	public:
		RenderPass(const std::shared_ptr<Device>& pDevice, const VkRenderPassCreateInfo& renderPassInfo);
		RenderPass(const RenderPass&) = delete;
		RenderPass& operator=(const RenderPass&) = delete;
		RenderPass& operator=(RenderPass&&) = delete;

		~RenderPass();

		// Getters
		VkRenderPass GetDeviceHandle() const { return m_renderPass; }
		std::vector<VkAttachmentDescription> GetAttachmentDesc() const { return m_attachmentDescList; }

	private:
		bool m_init = false;
		const std::shared_ptr<Device> m_pDevice;
		VkRenderPass m_renderPass;
		VkRenderPassCreateInfo m_renderPassInfo;
		std::vector<VkAttachmentDescription> m_attachmentDescList;
		std::vector<SubpassDef> m_subpasses;
		std::vector<VkSubpassDescription> m_subpassDescList; // Describe the subpass
		std::vector<VkSubpassDependency> m_subpassDependencyList; // Specify memory and execution dependencies between subpasses
	};

}