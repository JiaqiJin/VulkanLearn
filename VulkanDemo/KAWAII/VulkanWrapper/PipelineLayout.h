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
		PipelineLayout(const std::shared_ptr<Device> pDevice, const DescriptorSetLayoutList descriptorSetLayoutList);
		PipelineLayout(const std::shared_ptr<Device> pDevice, const DescriptorSetLayoutList descriptorSetLayoutList,
			const std::vector<VkPushConstantRange> pushConstsRanges);

		~PipelineLayout();

		VkPipelineLayout GetDeviceHandle() const { return m_pipelineLayout; }
		const DescriptorSetLayoutList GetDescriptorSetLayout() const { return m_descriptorSetLayoutList; }
		const std::vector<VkDescriptorSetLayout> GetDescriptorSetLayoutDeviceHandleList();

	private:
		VkPipelineLayout m_pipelineLayout;
		DescriptorSetLayoutList m_descriptorSetLayoutList;
		std::shared_ptr<Device> m_pDevice;

		friend class PipelineBase;
	};
}