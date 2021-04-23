#include "Sampler.h"
#include "Device.h"

namespace Rendering
{
	Sampler::Sampler(const Device& device)
		: m_device(device)
	{
        VkSamplerCreateInfo samplerCreateInfo{};
        samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
        samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
        samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerCreateInfo.anisotropyEnable = VK_TRUE;
        samplerCreateInfo.maxAnisotropy = 16.0f;
        samplerCreateInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;
        samplerCreateInfo.compareEnable = VK_FALSE;
        samplerCreateInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerCreateInfo.mipLodBias = 0.0f;
        samplerCreateInfo.minLod = 0.0f;
        samplerCreateInfo.maxLod = 0.0f;

        if (vkCreateSampler(m_device.getHandle(), &samplerCreateInfo, nullptr, &m_handle.get()) != VK_SUCCESS)
            throw std::runtime_error("failed to create texture sampler!");
	}

	Sampler::~Sampler()
	{
        vkDestroySampler(m_device.getHandle(), m_handle, nullptr);
	}
}