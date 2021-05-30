#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include <vector>

namespace RHI
{
	class Device;
	class DescriptorSetLayout;

	typedef std::vector<std::shared_ptr<DescriptorSetLayout>> DescriptorSetLayoutList;

	class PipelineLayout
	{
	public:
		PipelineLayout(const std::shared_ptr<Device>& pDevice, const DescriptorSetLayoutList& descriptorSetLayoutList);
		PipelineLayout(const std::shared_ptr<Device>& pDevice, const DescriptorSetLayoutList& descriptorSetLayoutList,
			const std::vector<VkPushConstantRange>& pushConstsRanges);

		~PipelineLayout();

		// Getters
		VkPipelineLayout GetDeviceHandle() const { return m_pipelineLayout; }
		const DescriptorSetLayoutList GetDescriptorSetLayout() const { return m_descriptorSetLayoutList; }
		// Return the list of the Descriptor Set Layout
		const std::vector<VkDescriptorSetLayout> GetDescriptorSetLayoutDeviceHandleList();

	private:
		VkPipelineLayout m_pipelineLayout; // Pipeline layout
		DescriptorSetLayoutList m_descriptorSetLayoutList; // Descriptor Set Layout
		std::shared_ptr<Device> m_pDevice;

		friend class PipelineBase;
	};
}