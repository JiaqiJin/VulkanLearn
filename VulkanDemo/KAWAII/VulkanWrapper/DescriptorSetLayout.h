#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include <vector>

namespace RHI
{
	class Device;

	class DescriptorSetLayout
	{
	public:
		DescriptorSetLayout(const std::shared_ptr<Device> pDevice, const std::vector<VkDescriptorSetLayoutBinding> dsLayoutBinding);
		~DescriptorSetLayout();

		DescriptorSetLayout(const DescriptorSetLayout&) = delete;
		DescriptorSetLayout& operator=(const DescriptorSetLayout&) = delete;
		DescriptorSetLayout& operator=(DescriptorSetLayout&&) = delete;

		// Getters
		const std::vector<VkDescriptorSetLayoutBinding>& GetDescriptorSetLayoutBinding() const { return m_descriptorSetLayoutBinding; }
		VkDescriptorSetLayout GetDeviceHandle() const { return m_descriptorSetLayout; }
	private:
		std::vector<VkDescriptorSetLayoutBinding> m_descriptorSetLayoutBinding;
		VkDescriptorSetLayout m_descriptorSetLayout;
		std::shared_ptr<Device> m_pDevice;
	};
}