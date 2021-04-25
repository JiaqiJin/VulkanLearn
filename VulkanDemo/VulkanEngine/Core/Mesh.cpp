#include "Mesh.h"
#include "Buffer.h"
#include "DeviceMemory.h"
#include "Utils.h"
#include <tiny_obj_loader.h>
#include "../vendor/tiny_gltf.h"
#include "VertexLayout.h"

#include "../glm.h"
#include <stdexcept>

namespace 
{
    struct Vertex
    {
        glm::vec3 pos;
        glm::vec3 color;
        glm::vec2 texCoord;

        bool operator==(const Vertex& other) const
        {
            return pos == other.pos && color == other.color && texCoord == other.texCoord;
        }
    };

    Rendering::VertexLayout::ComponentType findComponentType(int gltfComponentType)
    {
        switch (gltfComponentType)
        {
        case TINYGLTF_COMPONENT_TYPE_BYTE:
            return Rendering::VertexLayout::ComponentType::Byte;
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
            return Rendering::VertexLayout::ComponentType::UnsignedByte;
        case TINYGLTF_COMPONENT_TYPE_SHORT:
            return Rendering::VertexLayout::ComponentType::Short;
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
            return Rendering::VertexLayout::ComponentType::UnsignedShort;
        case TINYGLTF_COMPONENT_TYPE_INT:
            return Rendering::VertexLayout::ComponentType::Int;
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
            return Rendering::VertexLayout::ComponentType::UnsignedInt;
        case TINYGLTF_COMPONENT_TYPE_FLOAT:
            return Rendering::VertexLayout::ComponentType::Float;
        case TINYGLTF_COMPONENT_TYPE_DOUBLE:
            return Rendering::VertexLayout::ComponentType::Double;
        }

        throw std::invalid_argument("gltfComponentType");
    }

    Rendering::VertexLayout::AttributeType findAttributeType(int gltfAttributeType)
    {
        switch (gltfAttributeType)
        {
        case TINYGLTF_TYPE_VEC2:
            return Rendering::VertexLayout::AttributeType::Vec2;
        case TINYGLTF_TYPE_VEC3:
            return Rendering::VertexLayout::AttributeType::Vec3;
        case TINYGLTF_TYPE_VEC4:
            return Rendering::VertexLayout::AttributeType::Vec4;
        case TINYGLTF_TYPE_MAT2:
            return Rendering::VertexLayout::AttributeType::Mat2;
        case TINYGLTF_TYPE_MAT3:
            return Rendering::VertexLayout::AttributeType::Mat3;
        case TINYGLTF_TYPE_MAT4:
            return Rendering::VertexLayout::AttributeType::Mat4;
        }

        throw std::invalid_argument("gltfAttributeType");
    }

    std::size_t getNumberOfComponents(Rendering::VertexLayout::AttributeType type)
    {
        switch (type)
        {
        case Rendering::VertexLayout::AttributeType::Vec2:
            return 2;
        case Rendering::VertexLayout::AttributeType::Vec3:
            return 3;
        case Rendering::VertexLayout::AttributeType::Vec4:
            return 4;
        case Rendering::VertexLayout::AttributeType::Mat2:
            return 4;
        case Rendering::VertexLayout::AttributeType::Mat3:
            return 9;
        case Rendering::VertexLayout::AttributeType::Mat4:
            return 16;
        }

        throw std::invalid_argument("type");
    }

    std::size_t getComponentByteSize(Rendering::VertexLayout::ComponentType type)
    {
        switch (type)
        {
        case Rendering::VertexLayout::ComponentType::Byte:
            return sizeof(int8_t);
        case Rendering::VertexLayout::ComponentType::UnsignedByte:
            return sizeof(uint8_t);
        case Rendering::VertexLayout::ComponentType::Short:
            return sizeof(int16_t);
        case Rendering::VertexLayout::ComponentType::UnsignedShort:
            return sizeof(uint16_t);
        case Rendering::VertexLayout::ComponentType::Int:
            return sizeof(int32_t);
        case Rendering::VertexLayout::ComponentType::UnsignedInt:
            return sizeof(uint32_t);
        case Rendering::VertexLayout::ComponentType::Float:
            return sizeof(float);
        case Rendering::VertexLayout::ComponentType::Double:
            return sizeof(double);
        }

        throw std::invalid_argument("type");
    }

    std::size_t getAttributeStride(Rendering::VertexLayout::AttributeType attributeType, Rendering::VertexLayout::ComponentType componentType)
    {
        return getNumberOfComponents(attributeType) * getComponentByteSize(componentType);
    }

    std::size_t findAttributeLocation(std::string const& name)
    {
        static std::vector<std::string> const attributeNames = { "POSITION", "COLOR_0", "TEXCOORD_0", "NORMAL" };

        auto it = std::find(attributeNames.cbegin(), attributeNames.cend(), name);

        if (it != attributeNames.end())
            return static_cast<std::size_t>(std::distance(attributeNames.begin(), it));

        throw std::runtime_error("Unkown attribute name: " + name);
    }
}

namespace std
{
    template<> struct hash<Vertex>
    {
        size_t operator()(Vertex const& vertex) const
        {
            auto pos = hash<glm::vec3>()(vertex.pos);
            auto color = hash<glm::vec3>()(vertex.color);
            auto texCoord = hash<glm::vec2>()(vertex.texCoord);

            return ((pos ^ (color << 1)) >> 1) ^ (texCoord << 1);
        }
    };
}

Rendering::Mesh const* Rendering::Mesh::ms_boundMesh = nullptr;

namespace Rendering
{
    Mesh::Mesh(Application const& app, std::string const& path) : Object(app)
    {
        loadMesh(path);
        createBuffers(m_combinedData);
    }

    Mesh::Mesh(const Application& app, const std::shared_ptr<tinygltf::Model>& model, std::size_t meshIndex, std::size_t primitiveIndex) : Object(app)
    {
        m_gltfModel = model;

        const tinygltf::Primitive& primitive = model->meshes[meshIndex].primitives[primitiveIndex];

        {
            const tinygltf::Accessor& indexAccessor = model->accessors[static_cast<std::size_t>(primitive.indices)];
            m_vertexLayout.setIndexType(findComponentType(indexAccessor.componentType));
            m_indexCount = indexAccessor.count;
        }

        std::vector<VertexLayout::Binding> bindings;
      
        bindings.reserve(model->bufferViews.size());

        for (std::size_t bufferViewIndex = 0; bufferViewIndex < model->bufferViews.size(); bufferViewIndex++)
        {
            tinygltf::BufferView const& bufferView = model->bufferViews[bufferViewIndex];

            if (bufferView.target == TINYGLTF_TARGET_ARRAY_BUFFER)
            {
                //bindingsMap.emplace(bufferViewIndex, bindings.size());
                //bindings.emplace_back(bufferView.byteOffset, bufferView.byteLength, bufferView.byteStride);
            }
            else if (bufferView.target == TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER)
            {
                m_vertexLayout.setIndexDataOffset(bufferView.byteOffset);
            }
        }

        for (auto const& entry : primitive.attributes)
        {
            std::string const& name = entry.first;
            int accessorIndex = entry.second;

            tinygltf::Accessor const& accessor = model->accessors[static_cast<std::size_t>(accessorIndex)];
            tinygltf::BufferView const& bufferView = model->bufferViews[static_cast<std::size_t>(accessor.bufferView)];

            std::size_t location = findAttributeLocation(name);
            VertexLayout::AttributeType attributeType = findAttributeType(accessor.type);
            VertexLayout::ComponentType componentType = findComponentType(accessor.componentType);
            std::size_t offset = accessor.byteOffset;

            std::size_t stride = bufferView.byteStride;
            if (stride == 0)
                stride = getAttributeStride(attributeType, componentType);

            bindings.emplace_back(bufferView.byteOffset + offset, bufferView.byteLength, stride)
                .addAttribute(location, attributeType, componentType, 0);
        }

        m_vertexLayout.setBindings(bindings);

        m_vertexLayout.setIndexType(VertexLayout::ComponentType::UnsignedShort);

        std::size_t bufferIndex = 0; // TODO use all buffers
        std::vector<unsigned char> const& data = m_gltfModel->buffers[bufferIndex].data;

        createBuffers(data);
    }

    void Mesh::bindBuffers(VkCommandBuffer commandBuffer) const
    {
        if (ms_boundMesh == this)
            return;

        auto const& bindingOffsets = m_vertexLayout.getBindingOffsets();

        std::vector<VkBuffer> vertexBuffers;
        vertexBuffers.resize(bindingOffsets.size(), m_buffer->getHandle());

        vkCmdBindVertexBuffers(commandBuffer, 0, static_cast<uint32_t>(bindingOffsets.size()), vertexBuffers.data(), m_vertexLayout.getBindingOffsets().data());

        vkCmdBindIndexBuffer(commandBuffer, m_buffer->getHandle(), m_vertexLayout.getIndexDataOffset(), m_vertexLayout.getIndexType());

        ms_boundMesh = this;
    }

    void Mesh::loadMesh(std::string const& path)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warnings, errors;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warnings, &errors, path.c_str()))
            throw std::runtime_error(warnings + errors);

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        std::unordered_map<Vertex, uint32_t> uniqueVertices{};

        for (const auto& shape : shapes)
        {
            for (const auto& index : shape.mesh.indices)
            {
                std::size_t vertexIndex = static_cast<std::size_t>(index.vertex_index);
                std::size_t texcoordIndex = static_cast<std::size_t>(index.texcoord_index);

                Vertex vertex{};

                vertex.pos = {
                    attrib.vertices[3 * vertexIndex + 0],
                    attrib.vertices[3 * vertexIndex + 1],
                    attrib.vertices[3 * vertexIndex + 2],
                };

                vertex.texCoord = {
                    attrib.texcoords[2 * texcoordIndex + 0],
                    1.0f - attrib.texcoords[2 * texcoordIndex + 1],
                };

                vertex.color = { 1.0f, 1.0f, 1.0f };

                if (uniqueVertices.count(vertex) == 0)
                {
                    uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                }

                indices.push_back(uniqueVertices[vertex]);
            }
        }

        // TODO remove this bullshit
        struct PositionColor
        {
            glm::vec3 pos;
            glm::vec3 color;
        };

        std::vector<PositionColor> positionColors;
        std::vector<glm::vec2> texCoords;

        for (Vertex const& vertex : vertices)
        {
            PositionColor& positionColor = positionColors.emplace_back();
            positionColor.pos = vertex.pos;
            positionColor.color = vertex.color;

            texCoords.push_back(vertex.texCoord);
        }

        auto positionColorBufferSize = sizeof(positionColors[0]) * positionColors.size();
        auto texcoordBufferSize = sizeof(texCoords[0]) * texCoords.size();
        std::size_t indexBufferSize = sizeof(indices[0]) * indices.size();

        auto positionColorDataOffset = 0;
        auto texcoordDataOffset = positionColorBufferSize;
        auto indexDataOffset = positionColorBufferSize + texcoordBufferSize;

        m_indexCount = indices.size();
        m_vertexLayout.setIndexType(VertexLayout::ComponentType::UnsignedInt);
        m_vertexLayout.setIndexDataOffset(indexDataOffset);

        m_combinedData.resize(positionColorBufferSize + texcoordBufferSize + indexBufferSize);
        std::memcpy(m_combinedData.data() + positionColorDataOffset, positionColors.data(), positionColorBufferSize);
        std::memcpy(m_combinedData.data() + texcoordDataOffset, texCoords.data(), texcoordBufferSize);
        std::memcpy(m_combinedData.data() + indexDataOffset, indices.data(), indexBufferSize);

        std::vector<VertexLayout::Binding> bindings;
        bindings.emplace_back(positionColorDataOffset, positionColorBufferSize, sizeof(glm::vec3) + sizeof(glm::vec3))
            .addAttribute(0, VertexLayout::AttributeType::Vec3, VertexLayout::ComponentType::Float, 0)
            .addAttribute(1, VertexLayout::AttributeType::Vec3, VertexLayout::ComponentType::Float, sizeof(glm::vec3));

        bindings.emplace_back(texcoordDataOffset, texcoordBufferSize, sizeof(glm::vec2))
            .addAttribute(2, VertexLayout::AttributeType::Vec2, VertexLayout::ComponentType::Float, 0);

        m_vertexLayout.setBindings(bindings);
    }

    void Mesh::createBuffers(std::vector<unsigned char> const& rawData)
    {
        VkDeviceSize bufferSize = sizeof(rawData[0]) * rawData.size();
        void const* bufferData = rawData.data();

        std::unique_ptr<Buffer> stagingBuffer;
        std::unique_ptr<DeviceMemory> stagingBufferMemory;
        utils::createBuffer(getApp(), bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        stagingBufferMemory->copyFrom(bufferData, bufferSize);

        utils::createBuffer(getApp(), bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_buffer, m_bufferMemory);

        Buffer::copy(*stagingBuffer, *m_buffer);
    }
}

