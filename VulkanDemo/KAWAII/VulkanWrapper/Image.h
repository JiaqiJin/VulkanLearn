#pragma once

#include "Buffer.h"
#include "VKGPUSyncRes.h"
#include <gli/gli.hpp>

namespace RHI
{
	class Device;

	typedef struct _GliImageWrapper
	{
		std::vector<gli::texture> textures;
	} GliImageWrapper;

	class Image : public VKGPUSyncRes
	{
	public:
		Image(const std::shared_ptr<Device>& pDevice);
		~Image();

		// Getters
		VkImage GetDeviceHandle() const { return m_image; }
		virtual VkMemoryRequirements GetMemoryReqirments() const;

		void BindMemory(VkDeviceMemory memory, uint32_t offset) const;

	private:
		VkImage m_image;
		VkImageCreateInfo m_info;
	
	};
}