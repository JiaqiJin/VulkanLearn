#pragma once

#include "Vulkan.h"
#include <memory>
#include <vector>

namespace Rendering
{
	class Device;
	class SwapChain;
	class DescriptorSetLayout;

	struct DescriptorBinding
	{
		uint32_t Binding;
		uint32_t DescriptorCount;
		VkDescriptorType Type;
		VkShaderStageFlags Stage;
	};

	class DescriptorsManager final
	{
	public:
		NON_COPIABLE(DescriptorsManager);

		DescriptorsManager(const Device& device, const SwapChain& swapChain,
			const std::vector<DescriptorBinding>& descriptorBindings);

		~DescriptorsManager();

		VkDescriptorPool GetDescriptorPool() const
		{
			return descriptorPool;
		}

		const DescriptorSetLayout& GetDescriptorSetLayout() const
		{
			return *descriptorSetLayout;
		}

	private:
		void CreateDescriptorPool(const std::vector<DescriptorBinding>& descriptorBindings);

		const Device& device;
		const SwapChain& swapChain;
		VkDescriptorPool descriptorPool{};
		std::unique_ptr<DescriptorSetLayout> descriptorSetLayout{};
	};
}