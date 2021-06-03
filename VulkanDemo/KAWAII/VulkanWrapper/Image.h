#pragma once

#include "Buffer.h"
#include "VKGPUSyncRes.h"
#include "Buffer.h"

#include <gli/gli.hpp>

#include <memory>
#include <vector>

namespace RHI
{
	class Device;
	class SwapChain;
	class MemoryKey;
	class ImageView;
	class CommandBuffer;

	typedef struct _GliImageWrapper
	{
		std::vector<gli::texture> textures;
	} GliImageWrapper;

	class Image : public VKGPUSyncRes, public std::enable_shared_from_this<Image>
	{
	public:
		Image(const std::shared_ptr<Device>& pDevice);
		~Image();

		// Getters
		VkImage GetDeviceHandle() const { return m_image; }
		virtual VkMemoryRequirements GetMemoryReqirments() const;

		void BindMemory(VkDeviceMemory memory, uint32_t offset) const;

	protected:
		bool Init( VkImage img);
		bool Init(const VkImageCreateInfo& info, uint32_t memoryPropertyFlag);
		bool Init(const GliImageWrapper& gliTex, const VkImageCreateInfo& info, uint32_t memoryPropertyFlag);

	private:
		VkImage m_image;
		VkImageCreateInfo m_info;
		uint32_t m_memProperty; // Memory properties
		std::shared_ptr<MemoryKey> m_pMemKey;
	};
}