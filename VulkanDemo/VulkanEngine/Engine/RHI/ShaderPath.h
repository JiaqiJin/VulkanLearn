#pragma once
#include <string>

namespace Renderer 
{
	struct ShaderPath
	{
		std::string vertexShaderPath = "Resource/shaders/spv/vertex.spv";
		std::string fragmentShaderPath = "Resource/shaders/spv/fragment.spv";
	};
}