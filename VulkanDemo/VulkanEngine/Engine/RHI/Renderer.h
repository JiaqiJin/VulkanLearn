#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <optional>
#include <mutex>

#include "Vertex.h"
#include "ShaderPath.h"

namespace Renderer
{
	class Renderer
	{
	public:
		Renderer() = default;

		~Renderer() = default;

		Renderer(Renderer const&) = delete;

		Renderer& operator = (Renderer const&) = delete;

		Renderer(Renderer&&) = delete;

		Renderer& operator = (Renderer&&) = delete;

	private:

	};
}