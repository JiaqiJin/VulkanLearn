#include "VertexLayout.h"

#include <algorithm>
#include <stdexcept>
#include <iterator>

namespace Rendering
{
    VkFormat findAttributeFormat(VertexLayout::AttributeType type, VertexLayout::ComponentType componentType)
    {
        if (type == VertexLayout::AttributeType::Vec2 && componentType == VertexLayout::ComponentType::Float)
            return VK_FORMAT_R32G32_SFLOAT;
        if (type == VertexLayout::AttributeType::Vec3 && componentType == VertexLayout::ComponentType::Float)
            return VK_FORMAT_R32G32B32_SFLOAT;

        throw std::runtime_error("unknown attribute");
    }

    VkIndexType findComponentType(VertexLayout::ComponentType indexType)
    {
        switch (indexType)
        {
        case VertexLayout::ComponentType::UnsignedByte:
            return VK_INDEX_TYPE_UINT8_EXT;
        case VertexLayout::ComponentType::UnsignedShort:
            return VK_INDEX_TYPE_UINT16;
        case VertexLayout::ComponentType::UnsignedInt:
            return VK_INDEX_TYPE_UINT32;

        case VertexLayout::ComponentType::Byte:
        case VertexLayout::ComponentType::Short:
        case VertexLayout::ComponentType::Int:
        case VertexLayout::ComponentType::Float:
        case VertexLayout::ComponentType::Double:
            throw std::invalid_argument("Given component type isn't valid for indices");
        }

        throw std::invalid_argument("indexType");
    }

	void VertexLayout::setBindings(const std::vector<Binding>& bindings)
	{
        std::transform(bindings.begin(), bindings.end(), std::back_inserter(m_bindingOffsets),
            [](VertexLayout::Binding const& binding) { return static_cast<VkDeviceSize>(binding.offset); });

        m_bindingDescriptions.reserve(bindings.size());

        for (std::size_t i = 0; i < bindings.size(); i++)
        {
            Binding const& binding = bindings[i];
            VkVertexInputBindingDescription& bindingDescription = m_bindingDescriptions.emplace_back();

            bindingDescription.binding = static_cast<uint32_t>(i);
            bindingDescription.stride = static_cast<uint32_t>(binding.stride);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            m_attributeDescriptions.reserve(m_attributeDescriptions.size() + binding.attributes.size());

            for (const Attribute& attribute : binding.attributes)
            {
                VkVertexInputAttributeDescription& attributeDescription = m_attributeDescriptions.emplace_back();

                attributeDescription.binding = static_cast<uint32_t>(i);
                attributeDescription.location = static_cast<uint32_t>(attribute.location);
                attributeDescription.format = findAttributeFormat(attribute.type, attribute.componentType);
                attributeDescription.offset = static_cast<uint32_t>(attribute.offset);
            }
        }
	}

    void VertexLayout::setIndexType(ComponentType indexType)
    {
        m_indexType = findComponentType(indexType);
    }
}