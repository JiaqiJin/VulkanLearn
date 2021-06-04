#pragma once

#include "Buffer.h"
#include "VKGPUSyncRes.h"
#include "Buffer.h"
#include "../Math/Math.h"
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

	class Image : public VKGPUSyncRes//, public std::enable_shared_from_this<Image>
	{
	public:
		Image(const std::shared_ptr<Device>& pDevice);
		~Image();

		// Getters
		VkImage GetDeviceHandle() const { return m_image; }
		const VkImageCreateInfo& GetImageInfo() const { return m_info; }
		virtual uint32_t GetMemoryProperty() const { return m_memProperty; }
		virtual VkMemoryRequirements GetMemoryReqirments() const;

		void BindMemory(VkDeviceMemory memory, uint32_t offset) const;

	public:
		void CreateEmptyTexture(const Vector3ui& size,
			uint32_t mipLevels,
			uint32_t layers,
			VkFormat format,
			VkImageLayout defaultLayout,
			VkImageUsageFlags usage,
			VkPipelineStageFlags stageFlag,
			VkAccessFlags accessFlag,
			VkImageViewCreateFlags createFlag = 0);

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