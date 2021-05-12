#pragma once

#include <memory>

#include "ImageView.h"
#include "Vulkan.h"

namespace Rendering
{
	class CommandPool;
	class ImageView;
	class Device;

	class DepthBuffer
	{
	public:
		NON_COPIABLE(DepthBuffer);

		DepthBuffer();
		~DepthBuffer() = default;

		const ImageView& GetImageView() const
		{
			return *imageView;
		}

	private:
		//const Device& device;
		std::unique_ptr<ImageView> imageView;
	};
}