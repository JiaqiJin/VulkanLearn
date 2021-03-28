#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan.hpp>

#include <array>
#include <string>

#include "raii.h"

class VContext;

class VUtility
{
public:
	VUtility(const VContext& context);
	~VUtility() = default;

	VUtility(VUtility&&) = delete;
	VUtility& operator= (VUtility&&) = delete;
	VUtility(const VUtility&) = delete;
	VUtility& operator= (const VUtility&) = delete;

private:
	const VContext* context;
};
