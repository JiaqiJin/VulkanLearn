#include "Renderer.h"
#include "VulkanRender.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan.hpp>

#include <functional>
#include <vector>
#include <array>
#include <string>
#include <algorithm>
#include <fstream>
#include <chrono>

Renderer::Renderer(GLFWwindow* window)
	:p_render(std::make_unique<VulkanRender>(window)) {}

void Renderer::Draw(float deltatime)
{
	p_render->Draw(deltatime);
}

void Renderer::cleanUp()
{
	p_render->cleanUp();
}