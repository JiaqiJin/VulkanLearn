#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <memory>

namespace RHI
{
	class Instance;
	class PhysicalDevice;

	class Device
	{
	public:
		Device(const std::shared_ptr<Instance> pInstance, const std::shared_ptr<PhysicalDevice> pPhyisicalDevice);

		~Device();

		bool Init(const std::shared_ptr<Instance>& pInst, const std::shared_ptr<PhysicalDevice>& pPhyisicalDevice);

		// Getters
		const VkDevice GetDeviceHandle() const { return m_device; }
		const std::shared_ptr<PhysicalDevice> GetPhysicalDevice() const { return m_pPhysicalDevice; }
		const std::shared_ptr<Instance> GetInstance() const { return m_pVulkanInst; }

		PFN_vkCmdDrawIndirectCountKHR CmdDrawIndexedIndirectCountKHR() const { return m_fpCmdDrawIndexedIndirectCountKHR; }

	private:
		VkDevice m_device;
		std::shared_ptr<PhysicalDevice> m_pPhysicalDevice;
		std::shared_ptr<Instance> m_pVulkanInst;

		PFN_vkCmdDrawIndirectCountKHR m_fpCmdDrawIndexedIndirectCountKHR;
	};
}