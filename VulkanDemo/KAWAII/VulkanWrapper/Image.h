#pragma once

#include "Buffer.h"
#include "VKGPUSyncRes.h"
#include <gli/gli.hpp>

namespace RHI
{
	typedef struct _GliImageWrapper
	{
		std::vector<gli::texture> textures;
	} GliImageWrapper;

	class Image : public VKGPUSyncRes
	{
	public:
	

	private:
		VkImage m_image;
		VkImageCreateInfo m_info;

	};
}