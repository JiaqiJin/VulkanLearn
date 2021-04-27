#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "UniqueHandle.h"
#include "../Objects/Object.h"

namespace Rendering
{
    class DeviceMemory : public Object
    {
    public:
        explicit DeviceMemory(const Application& app, VkMemoryRequirements memoryRequirements, VkMemoryPropertyFlags memoryProperties);
        ~DeviceMemory();

        DeviceMemory(const DeviceMemory&) = default;
        DeviceMemory(DeviceMemory&&) = default;
        DeviceMemory& operator=(const DeviceMemory&) = default;
        DeviceMemory& operator=(DeviceMemory&&) = default;

        void copyFrom(void const* sourcePointer, std::size_t sourceSize);

        template<typename T>
        void copyFrom(const std::vector<T>& source)
        {
            copyFrom(source.data(), sizeof(T) * source.size());
        }

        VkDeviceMemory getHandle() const { return m_handle; };

    private:
        UniqueHandle<VkDeviceMemory> m_handle;
    };
}
