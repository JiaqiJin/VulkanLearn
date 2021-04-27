#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <memory>
#include <optional>
#include <sstream>
#include <array>

#include "Core/ImageView.h"
#include "Core/DeviceMemory.h"
#include "Core/Image.h"
#include "Core/Buffer.h"
#include "Core/Sampler.h"
#include "Core/ShaderModule.h"
#include "Core/Swapchain.h"
#include "Core/RenderPass.h"
#include "Core/Framebuffer.h"
#include "Core/DescriptorSetLayout.h"
#include "Core/PipelineLayout.h"
#include "Core/Pipeline.h"
#include "Core/DescriptorSet.h"
#include "Core/Mesh.h"
#include "Core/Semaphore.h"
#include "Core/Fence.h"
#include "Core/CommandBuffers.h"
#include "Core/ScopedOneTimeCommandBuffer.h"
#include "Core/PhysicalDevice.h"
#include "Core/Utils.h"
#include "Core/Texture.h"
#include "Core/DescriptorPool.h"
#include "Core/Instance.h"
#include "Window.h"
#include "Application.h"
#include "Core/Device.h"
#include "Core/Queue.h"
#include <array>
#include <iostream>
#include "Core/Renderer.h"
#include "Core/Material.h"
#include "Core/SceneObject.h"
#include "Core/Shader.h"
#include "Window.h"
#include "Application.h"

#include "vendor/magic_enum.hpp"
#include "vendor/tiny_gltf.h"

#include "Core/Timer.h"
#include "Core/Light.h"

#pragma warning(push, 0)
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_vulkan.h"
#pragma warning(pop)

#ifdef _DEBUG
bool const VALIDATION_ENABLED = true;
#else
bool const VALIDATION_ENABLED = false;
#endif
bool const API_DUMP_ENABLED = false;
const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

const std::string GLTF_MODEL_PATH = "data/models/Duck/glTF/Duck.gltf";
//const float GLTF_MODEL_SCALE = 1.0f;
const float GLTF_MODEL_SCALE = 0.01f;

const glm::vec3 LIGHT_POS = glm::vec3(-10.0, 0.0f, 0.0f);
const glm::vec3 CAMERA_POS = glm::vec3(0.0f, -3.0f, 3.0f);
const glm::vec3 CAMERA_ANGLES = glm::vec3(-45.0f, 0.0f, 0.0f);
const float CAMERA_SPEED = 1.0f;

std::shared_ptr<tinygltf::Model> loadModel(std::string const& path)
{
	std::shared_ptr<tinygltf::Model> model = std::make_shared<tinygltf::Model>();
	tinygltf::TinyGLTF loader;
	std::string err;
	std::string warn;
	bool success = loader.LoadASCIIFromFile(model.get(), &err, &warn, path);

	if (!warn.empty())
		std::cout << warn << std::endl;
	if (!err.empty())
		std::cout << err << std::endl;

	if (!success)
		return nullptr;

	return model;
}

class WindowApplication
{
public:
	WindowApplication()
	{
        m_keyState.resize(1 << 8 * sizeof(char), false);

		m_window = std::make_unique<Rendering::Window>(WIDTH, HEIGHT, "Vulkan Demo");
		m_window->addResizeCallback([this](int, int) { onFramebufferResized(); });
		m_window->addKeyCallback([this](Rendering::Window::Action action, Rendering::Window::Key key, char c, Rendering::Window::Modifiers modifiers) { onKey(action, key, c, modifiers); });
		m_window->addMouseMoveCallback([this](glm::vec2 const& delta) { onMouseMove(delta); });
		m_application = std::make_unique<Rendering::Application>("Vulkan demo", VALIDATION_ENABLED, API_DUMP_ENABLED,*m_window);

        loadResources();
        createRenderer();
        initImGui();

        createSceneObjects();
	}

    ~WindowApplication()
    {
        getApp().getDevice().waitIdle();
    }

	void run()
	{
        m_frameTimer.start();
		m_window->startEventLoop([this]() { drawFrame(); });
	}

	const Rendering::Application& getApp() { return *m_application; }

private:
	
    void initImGui()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        ImGui::StyleColorsDark();

        io.Fonts->AddFontDefault();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForVulkan(m_window->getHandle(), true);

        m_descriptorPool = std::make_unique<Rendering::DescriptorPool>(getApp(), 1);
        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = getApp().getInstance().getHandle();
        init_info.PhysicalDevice = getApp().getPhysicalDevice().getHandle();
        init_info.Device = getApp().getDevice().getHandle();
        init_info.QueueFamily = getApp().getDevice().getGraphicsQueue().getFamily().getIndex();
        init_info.Queue = getApp().getDevice().getGraphicsQueue().getHandle();
        init_info.PipelineCache = VK_NULL_HANDLE;
        init_info.DescriptorPool = m_descriptorPool->getHandle();
        init_info.Allocator = nullptr;
        init_info.MinImageCount = 2; 
        init_info.ImageCount = static_cast<uint32_t>(m_renderer->getSwapchain().getImageCount());
        init_info.CheckVkResultFn = nullptr;
        ImGui_ImplVulkan_Init(&init_info, m_renderer->getRenderPass().getHandle());

        {
            Rendering::ScopedOneTimeCommandBuffer buffer{ getApp() };
            ImGui_ImplVulkan_CreateFontsTexture(buffer.getHandle());
            buffer.submit();

            ImGui_ImplVulkan_DestroyFontUploadObjects();
        }
    }

	void onFramebufferResized()
	{
		m_application->onSurfaceChanged();
	}

    void onKey(Rendering::Window::Action action, Rendering::Window::Key key, char c, Rendering::Window::Modifiers mods)
    {
        std::stringstream ss;
        auto separator = "";

        for (Rendering::Window::Modifiers value : magic_enum::enum_values<Rendering::Window::Modifiers>())
        {
            if (mods & value)
            {
                ss << separator << magic_enum::enum_name(value);
                separator = " | ";
            }
        }

        std::cout << magic_enum::enum_name(action) << ' ' << magic_enum::enum_name(key) << ' ' << ss.str() << ": " << "'" << c << "'" << std::endl;

        std::size_t index = static_cast<std::size_t>(c);
        m_keyState[index] = action == Rendering::Window::Action::Press;
        m_modifiers = mods;
    }

    void onMouseMove(glm::vec2 const& delta)
    {
        glm::vec3 angleDelta = m_mouseSensitivity * glm::vec3{ -delta.y, 0.0f, -delta.x };
        glm::quat rotationDelta = createRotation(angleDelta);

        Rendering::Transform& cameraTransform = m_renderer->getCamera().getTransform();
        cameraTransform.setRotation(cameraTransform.getRotation() * rotationDelta);
    }

    void createRenderer()
    {
        m_renderer = std::make_unique<Rendering::Renderer>(getApp());
        m_renderer->setWaitUntilWindowInForegroundCallback([this]() { m_window->waitUntilInForeground(); });

        Rendering::Transform& cameraTransform = m_renderer->getCamera().getTransform();
        cameraTransform.setPos(CAMERA_POS);
        m_cameraRotation = CAMERA_ANGLES;
        cameraTransform.setRotation(createRotation(m_cameraRotation));
    }

    glm::quat createRotation(glm::vec3 const& eulerDegrees)
    {
        glm::vec3 radians = glm::radians(eulerDegrees);
        return glm::toQuat(glm::eulerAngleYXZ(radians.y, radians.x, radians.z));
    }

    void loadResources()
    {
        m_defaultShader = Rendering::ShaderBuilder()
            .addStage(Rendering::ShaderModule::Type::Vertex, "data/shaders/default.vert.spv")
            .addStage(Rendering::ShaderModule::Type::Fragment, "data/shaders/default.frag.spv")
            .build(getApp());
        m_noColorShader = Rendering::ShaderBuilder()
            .addStage(Rendering::ShaderModule::Type::Vertex, "data/shaders/default.vert.spv")
            .addStage(Rendering::ShaderModule::Type::Fragment, "data/shaders/no-color.frag.spv")
            .build(getApp());
        m_testGeometryShader = Rendering::ShaderBuilder()
            .addStage(Rendering::ShaderModule::Type::Vertex, "data/shaders/normaldebug.vert.spv")
            .addStage(Rendering::ShaderModule::Type::Geometry, "data/shaders/normaldebug.geom.spv")
            .addStage(Rendering::ShaderModule::Type::Fragment, "data/shaders/color.frag.spv")
            .build(getApp());

        m_defaultSampler = std::make_shared<Rendering::Sampler>(getApp());
        m_gltfModel = loadModel(GLTF_MODEL_PATH);
    }

    std::unique_ptr<Rendering::SceneObject> createSceneObject(std::shared_ptr<tinygltf::Model> const& model)
    {
        std::unique_ptr<Rendering::SceneObject> object = std::make_unique<Rendering::SceneObject>();

        std::size_t const meshIndex = 0;
        std::size_t const primitiveIndex = 0;

        object->setMesh(std::make_shared<Rendering::Mesh>(getApp(), model, meshIndex, primitiveIndex));

        auto material = std::make_shared<Rendering::Material>();
        material->setShader(m_noColorShader);

        if (!model->images.empty())
        {
            tinygltf::Image const& image = model->images[0];
            auto texture = std::make_shared<Rendering::Texture>(getApp(), image);
            material->setTexture(texture);
            material->setSampler(m_defaultSampler);
            material->setShader(m_defaultShader);
        }

        object->setMaterial(material);

        return object;
    }

    void createSceneObjects()
    {
        m_model = createSceneObject(m_gltfModel);
        m_renderer->addObject(m_model);

        m_light = std::make_shared<Rendering::Light>();
        m_light->getTransform().setPos(LIGHT_POS);
        m_renderer->setLight(m_light);
    }

    void updateUI(float frameTime, float fenceTime)
    {
        ImGuiIO& io = ImGui::GetIO();

        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        float cpuUtilizationRatio = 1.0f - fenceTime / frameTime;

        ImGui::Begin("Time", nullptr);
        ImGui::Text("Frame time %.3f ms", frameTime * 1000.0f);
        ImGui::Text("Fence time %.3f ms", fenceTime * 1000.0f);
        ImGui::Text("CPU Utilization %.2f%%", cpuUtilizationRatio * 100.0f);
        ImGui::End();

        ImGui::Begin("Camera");
        Rendering::Transform& cameraTransform = m_renderer->getCamera().getTransform();
        glm::vec3 cameraPos = cameraTransform.getPos();
        if (ImGui::SliderFloat3("Position", &cameraPos[0], -10.0f, 10.0f, "%.2f", 1.0f))
        {
            cameraTransform.setPos(cameraPos);
        }
        if (ImGui::SliderFloat3("Rotation", &m_cameraRotation[0], -180.0f, 180.0f, "%.1f", 1.0f))
        {
            cameraTransform.setRotation(createRotation(m_cameraRotation));
        }
        ImGui::End();

        ImGui::Begin("Light");
        glm::vec3 lightPos = m_light->getTransform().getPos();
        if (ImGui::SliderFloat3("Position", &lightPos[0], -10.0f, 10.0f, "%.2f", 1.0f))
        {
            m_light->getTransform().setPos(lightPos);
        }
        ImGui::End();

        ImGui::Begin("Input");
        ImGui::SliderFloat("Mouse sensitivity", &m_mouseSensitivity, 0.01f, 1.0f, "%.2f", 1.0f);
        ImGui::SliderFloat("Camera speed", &m_cameraSpeed, 0.1f, 10.0f, "%.2f", 1.0f);
        ImGui::End();

        ImGui::Render();

        m_window->setCanCaptureCursor(!io.WantCaptureMouse);
    }


    void drawFrame()
    {
        update();
        m_renderer->draw();
        m_fpsDrawnFrames++;
    }

    void update()
    {
        if (m_frameTimer.getTime() > 0.2f)
        {
            float multiplier = 1.0f / static_cast<float>(m_fpsDrawnFrames);
            m_lastFrameTime = m_frameTimer.loop() * multiplier;
            m_lastFenceTime = m_renderer->getCumulativeFenceTime() * multiplier;
            m_renderer->resetCumulativeFenceTime();
            m_fpsDrawnFrames = 0;
        }
        updateUI(m_lastFrameTime, m_lastFenceTime);
        updateScene(m_lastFrameTime);
    }

    void updateScene(float dt)
    {
        auto updateObject = [](Rendering::Transform& transform, float time, bool fixRotation)
        {
            glm::quat initialRotation = glm::identity<glm::quat>();
            if (fixRotation)
                initialRotation = glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

            transform.setRotation(glm::angleAxis(time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * initialRotation);
        };

        float time = m_appTime.getTime();

        (void*)&time;

        m_model->getTransform().setRotation(glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
        //updateObject(m_model->getTransform(), time * 0.5f, true);

        m_model->getTransform().setScale(glm::vec3(GLTF_MODEL_SCALE));

        updateCamera(dt);
    }

    void updateCamera(float dt)
    {
        glm::vec3 localDirection = glm::zero<glm::vec3>();

        if (m_keyState['A'])
            localDirection.x -= 1.0f;
        if (m_keyState['D'])
            localDirection.x += 1.0f;
        if (m_keyState['S'])
            localDirection.y -= 1.0f;
        if (m_keyState['W'])
            localDirection.y += 1.0f;
        if (m_keyState['Q'])
            localDirection.z -= 1.0f;
        if (m_keyState['E'])
            localDirection.z += 1.0f;

        Rendering::Transform& cameraTransform = m_renderer->getCamera().getTransform();

        glm::vec3 worldDirection = cameraTransform.transformVectorLocalToWorld(localDirection);
        cameraTransform.setPos(cameraTransform.getPos() + m_cameraSpeed * dt * worldDirection);
    }

private:
    std::unique_ptr<Rendering::Window> m_window;

    std::unique_ptr<Rendering::Application> m_application;
    std::unique_ptr<Rendering::Renderer> m_renderer;

    // Resources
    std::shared_ptr<Rendering::Sampler> m_defaultSampler;
    std::shared_ptr<Rendering::Shader> m_defaultShader;
    std::shared_ptr<Rendering::Shader> m_noColorShader;
    std::shared_ptr<Rendering::Shader> m_testGeometryShader;

    std::shared_ptr<tinygltf::Model> m_gltfModel;

    // Objects
    std::shared_ptr<Rendering::SceneObject> m_model;
    std::shared_ptr<Rendering::Light> m_light;

    std::unique_ptr<Rendering::DescriptorPool> m_descriptorPool;

    Rendering::Timer m_frameTimer;
    Rendering::Timer m_appTime;
    std::uint32_t m_fpsDrawnFrames = 0;
    float m_lastFrameTime = 0.0f;
    float m_lastFenceTime = 0.0f;

    glm::vec3 m_cameraRotation = glm::vec3(0.0f, 0.0f, 0.0f);
    float m_mouseSensitivity = 0.3f;
    float m_cameraSpeed = 5.0f;

    std::vector<bool> m_keyState;
    Rendering::Window::Modifiers m_modifiers = Rendering::Window::Modifiers::None;
};