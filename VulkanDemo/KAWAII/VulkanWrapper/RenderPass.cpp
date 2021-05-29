#include "RenderPass.h"
#include "Device.h"

#include "../Common/Macro.h"
#include "../Common/Logger.h"

namespace RHI
{
	RenderPass::RenderPass(const std::shared_ptr<Device> pDevice, const VkRenderPassCreateInfo renderPassInfo)
		: m_pDevice(pDevice), m_renderPassInfo(renderPassInfo)
	{
		// Attachment
		for(uint32_t i = 0; i < m_renderPassInfo.attachmentCount; i++)
			m_attachmentDescList.push_back(m_renderPassInfo.pAttachments[i]);
		m_renderPassInfo.pAttachments = m_attachmentDescList.data();

		// Dependencies
		for(uint32_t i = 0; i < m_renderPassInfo.dependencyCount; i++)
			m_subpassDependencyList.push_back(m_renderPassInfo.pDependencies[i]);
		m_renderPassInfo.pDependencies = m_subpassDependencyList.data();

		// Subpass
		for (uint32_t i = 0; i < m_renderPassInfo.subpassCount; i++)
		{
			VkSubpassDescription subpassDesc = m_renderPassInfo.pSubpasses[i];
			SubpassDef subpass = {};

			for (uint32_t i = 0; i < subpassDesc.colorAttachmentCount; i++)
				subpass.m_colorAttachmentRefs.push_back(subpassDesc.pColorAttachments[i]);

			if (subpassDesc.pDepthStencilAttachment)
				subpass.m_depthStencilAttachmentRef = *subpassDesc.pDepthStencilAttachment;
			else
				subpass.m_depthStencilAttachmentRef = {};

			for (uint32_t i = 0; i < subpassDesc.inputAttachmentCount; i++)
				subpass.m_inputAttachmentRefs.push_back(subpassDesc.pInputAttachments[i]);

			for (uint32_t i = 0; i < subpassDesc.preserveAttachmentCount; i++)
				subpass.m_inputAttachmentRefs.push_back(subpassDesc.pInputAttachments[i]);

			if (subpassDesc.pResolveAttachments != nullptr)
			{
				for (uint32_t i = 0; i < subpassDesc.colorAttachmentCount; i++)
					subpass.m_resolveAttachmentRefs.push_back(subpassDesc.pResolveAttachments[i]);
			}
			m_subpasses.push_back(subpass);
		}

		for (uint32_t i = 0; i < m_renderPassInfo.subpassCount; i++)
		{
			VkSubpassDescription subpassDesc = m_renderPassInfo.pSubpasses[i];

			subpassDesc.pColorAttachments = m_subpasses[i].m_colorAttachmentRefs.data();
			subpassDesc.pInputAttachments = m_subpasses[i].m_inputAttachmentRefs.data();
			subpassDesc.pPreserveAttachments = m_subpasses[i].m_preservAttachmentRefs.data();
			subpassDesc.pResolveAttachments = m_subpasses[i].m_resolveAttachmentRefs.data();
			if (subpassDesc.pDepthStencilAttachment)
				subpassDesc.pDepthStencilAttachment = &m_subpasses[i].m_depthStencilAttachmentRef;
			m_subpassDescList.push_back(subpassDesc);
		}

		m_renderPassInfo.pSubpasses = m_subpassDescList.data();

		CHECK_VK_ERROR(vkCreateRenderPass(pDevice->GetDeviceHandle(), &m_renderPassInfo, nullptr, &m_renderPass));
	}

	RenderPass::~RenderPass()
	{
		vkDestroyRenderPass(m_pDevice->GetDeviceHandle(), m_renderPass, nullptr);
	}
}