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
			Attribute(size_t location, AttributeType type, ComponentType componentType, size_t offset)
				: location(location)
				, type(type)
				, componentType(componentType)
				, offset(offset)
			{

			}

			AttributeType type = AttributeType::Vec2;
			ComponentType componentType = ComponentType::Float;
			size_t location = 0;
			size_t offset = 0;
		};

		struct Binding
		{
			Binding(size_t offset, size_t stride) : offset(offset), stride(stride) {}

			template<class... Args>
			Binding& addAttribute(Args&&... args)
			{
				attributes.emplace_back(std::forward<Args>(args)...);
				return *this;
			}

			size_t stride = 0;
			size_t offset = 0;
			std::vector<Attribute> attributes;
		};

		void setBindings(const std::vector<Binding>& bindings);

	private:
		std::vector<VkDeviceSize> m_bindingOffsets;
		std::vector<VkVertexInputBindingDescription> m_bindingDescriptions;
		std::vector<VkVertexInputAttributeDescription> m_attributeDescriptions;

		VkIndexType m_indexType = VK_INDEX_TYPE_UINT16;
	};
}