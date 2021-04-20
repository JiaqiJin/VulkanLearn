#pragma once

#include <vulkan/vulkan.h>
#include "UniqueHandle.h"

/*
* Wrapper class for VkDescriptorSetLayout 
* Tell vulkan this descriptor for vertex shader
*/
namespace Rendering
{
	class Device;

	class DescriptorSetLayout
	{
	public:
		DescriptorSetLayout(const Device& device);
		~DescriptorSetLayout();

		DescriptorSetLayout(const DescriptorSetLayout&) = default;
		DescriptorSetLayout(DescriptorSetLayout&&) = default;
		DescriptorSetLayout& operator=(const DescriptorSetLayout&) = default;
		DescriptorSetLayout& operator=(DescriptorSetLayout&&) = default;

		VkDescriptorSetLayout getHandle() const { return m_handle; }
	private:
		UniqueHandle<VkDescriptorSetLayout> m_handle;
		const Device& m_device;
	};
}