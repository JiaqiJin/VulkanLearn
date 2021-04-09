#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include <vector>

namespace Rendering
{
	namespace Utils
	{
		bool checkSupportOption(std::vector<char const*> const& availableOptions, std::vector<char const*> const& requestedOptions);
	}
}