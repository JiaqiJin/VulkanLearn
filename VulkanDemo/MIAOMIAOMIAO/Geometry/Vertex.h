#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "../Core/Vulkan.h"
#include <array>

namespace Rendering
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;
		int32_t materialId;

	};
}


namespace std
{
	template <>
	struct hash<Rendering::Vertex>
	{
		size_t operator()(Rendering::Vertex const& vertex) const
		{
			return
				((hash<glm::vec3>()(vertex.position) ^
					(hash<glm::vec3>()(vertex.normal) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.texCoords) << 1);
		}
	};
}