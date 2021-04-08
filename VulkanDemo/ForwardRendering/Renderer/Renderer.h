#pragma once

#include <glm/glm.hpp>

#include <memory>

struct GLFWwindow;
struct VulkanRender;

class Renderer
{
public:
	Renderer(GLFWwindow* window);
	~Renderer();

	void Draw(float deltatime);
	void cleanUp();

	Renderer(const Renderer&) = delete;
	Renderer& operator= (const Renderer&) = delete;
	Renderer(Renderer&&) = delete;
	Renderer& operator= (Renderer&&) = delete;

private:
	std::unique_ptr<VulkanRender> p_render;
};