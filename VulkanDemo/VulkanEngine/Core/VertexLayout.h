#pragma once

#include <vulkan/vulkan.h>
#include <vector>

/*
* Helper class for Vertex shader element creation
* Binding descriptions(VkVertexInputBindingDescription ), how passing data format to the vertex shaders
* Attribute descriptions (VkVertexInputAttributeDescription), how handle vertex input
*/
namespace Rendering
{
    class VertexLayout
    {
    public:
        enum class AttributeType
        {
            Vec2,
            Vec3,
            Vec4,
            Mat2,
            Mat3,
            Mat4,
        };

        enum class ComponentType
        {
            Byte,
            UnsignedByte,
            Short,
            UnsignedShort,
            Int,
            UnsignedInt,
            Float,
            Double,
        };

        struct Attribute
        {
            Attribute(std::size_t location, AttributeType type, ComponentType componentType, std::size_t offset)
                : location(location)
                , type(type)
                , componentType(componentType)
                , offset(offset)
            {

            }

            std::size_t location = 0;
            AttributeType type = AttributeType::Vec2;
            ComponentType componentType = ComponentType::Float;
            std::size_t offset = 0;
        };

        struct Binding
        {
            Binding(std::size_t offset, std::size_t length, std::size_t stride) : offset(offset), length(length), stride(stride) {}

            template<class... Args>
            Binding& addAttribute(Args&&... args)
            {
                attributes.emplace_back(std::forward<Args>(args)...);
                return *this;
            }

            std::size_t offset = 0;
            std::size_t length = 0;
            std::size_t stride = 0;
            std::vector<Attribute> attributes;
        };

        void setBindings(std::vector<Binding> const& bindings);
        const std::vector<VkDeviceSize>& getBindingOffsets() const { return m_bindingOffsets; }
        const std::vector<VkVertexInputBindingDescription>& getBindingDescriptions() const { return m_bindingDescriptions; }
        const std::vector<VkVertexInputAttributeDescription>& getAttributeDescriptions() const { return m_attributeDescriptions; };

        void setIndexType(ComponentType indexType);
        VkIndexType getIndexType() const { return m_indexType; }

        void setIndexDataOffset(std::size_t offset) { m_indexDataOffset = offset; }
        VkDeviceSize getIndexDataOffset() const { return m_indexDataOffset; }

    private:
        std::vector<VkDeviceSize> m_bindingOffsets;
        std::vector<VkVertexInputBindingDescription> m_bindingDescriptions;
        std::vector<VkVertexInputAttributeDescription> m_attributeDescriptions;

        VkIndexType m_indexType = VK_INDEX_TYPE_UINT16;
        VkDeviceSize m_indexDataOffset = 0;
    };
}