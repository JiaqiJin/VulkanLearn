#pragma once

#include <vulkan/vulkan.h>
#include "../Objects/Object.h"
#include <memory>
#include <vector>
#include <string>
#include "VertexLayout.h"

namespace tinygltf { class Model; }

namespace Rendering
{
	class Buffer;
	class DeviceMemory;

	class Mesh : Object
	{
	public:
		Mesh(const Application& app, std::string const& path);
		Mesh(const Application& app, const std::shared_ptr<tinygltf::Model>& model, std::size_t meshIndex, std::size_t primitiveIndex);

		VertexLayout getVertexLayout() const { return m_vertexLayout; }

		void bindBuffers(VkCommandBuffer commandBuffer) const;

		std::size_t getIndexCount() const { return m_indexCount; }

	public:
		static void resetBoundMesh() { ms_boundMesh = nullptr; }

	private:
		void loadMesh(std::string const& path);

		void createBuffers(const std::vector<unsigned char>& rawData);

	private:
		std::vector<unsigned char> m_combinedData;
		std::shared_ptr<tinygltf::Model> m_gltfModel;

		std::size_t m_indexCount = 0;

		std::unique_ptr<Buffer> m_buffer;
		std::unique_ptr<DeviceMemory> m_bufferMemory;
		VertexLayout m_vertexLayout;

	private:
		static const Mesh* ms_boundMesh;
	};
}