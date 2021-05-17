#pragma once

#include "Vulkan.h"
#include "DescriptorsManager.h"

namespace Rendering
{
	class Device;
	/*
	* The descriptor layout specifies the types of resources that are going to be accessed by the pipeline,
	* just like a render pass specifies the types of attachments that will be accessed. 
	*/
	class DescriptorSetLayout final
	{
	public:
		NON_COPIABLE(DescriptorSetLayout)

		DescriptorSetLayout(const Device& device, const std::vector<DescriptorBinding>& descriptorBindings);
		~DescriptorSetLayout();

		VkDescriptorSetLayout Get() const
		{
			return descriptorSetLayout;
		}

		 const Device& GetDevice() const
		{
			return m_device;
		}
	private:
		const Device& m_device;
		VkDescriptorSetLayout descriptorSetLayout{};
	};
}