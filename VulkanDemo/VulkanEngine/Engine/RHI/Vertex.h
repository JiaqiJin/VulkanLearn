#pragma once

#include "../Math/Math.h"

namespace Renderer 
{
	struct Vertex 
	{
		Vec4f position = Vec4f(0.0f);
		Vec4f color = Vec4f(0.0f);
		Vec3f texCoord = Vec3f(0.0f);
		Vec3f normal = { 0.0f,1.0f,0.0f };
	};
}