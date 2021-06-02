#include "DeviceMemoryManager.h"
#include <algorithm>
#include "Buffer.h"
#include "Image.h"
#include "PhysicalDevice.h"

#include "../Common/Macro.h"
#include "../Common/Logger.h"

namespace RHI
{
	uint32_t MemoryKey::m_allocatedKeys = 0;

	MemoryKey::MemoryKey(const std::shared_ptr<DeviceMemoryManager>& pDeviceMemMgr, bool bufferOrImage)
		: m_pDeviceMemMgr(pDeviceMemMgr), m_bufferOrImage(bufferOrImage)
	{
		m_key = m_allocatedKeys++;
	}

	MemoryKey::~MemoryKey()
	{
		if (m_bufferOrImage)
			m_pDeviceMemMgr->FreeBufferMemChunk(m_key);
		else
			m_pDeviceMemMgr->FreeImageMemChunk(m_key);
	}

	// ---------------------------- DeviceMemoryManager ---------------------------------------
	DeviceMemoryManager::DeviceMemoryManager(const std::shared_ptr<Device> pDevice)
		: m_pDevice(pDevice)
	{
		m_bufferMemPool.resize(sizeof(uint32_t) * 8, {}); // Same size as bit count of uint32_t, i.e. type index count
	}

	bool DeviceMemoryManager::FindFreeBufferMemoryChunk(uint32_t key, uint32_t typeIndex, uint32_t numBytes, uint32_t& offset)
	{
		offset = 0;
		uint32_t endByte = 0;
		for (uint32_t i = 0; i < m_bufferMemPool[typeIndex].bindingList.size(); i++)
		{
			auto& bindingInfo = m_bufferBindingTable[m_bufferBindingLookupTable[m_bufferMemPool[typeIndex].bindingList[i]].first];
			// Skip if it's freed
			if (bindingInfo.second)
				continue;

			endByte = offset + numBytes - 1;
			if (endByte < bindingInfo.first.startByte)
			{
				m_bufferMemPool[typeIndex].bindingList.insert(m_bufferMemPool[typeIndex].bindingList.begin() + i, key);
				return true;
			}
			else
			{
				offset = bindingInfo.first.startByte + bindingInfo.first.numBytes;
			}
		}

		if (offset + numBytes > m_bufferMemPool[typeIndex].numBytes)
			return false;

		m_bufferMemPool[typeIndex].bindingList.push_back(key);
		return true;
	}

	void DeviceMemoryManager::AllocateBufferMemory(uint32_t key, uint32_t numBytes, uint32_t memoryTypeBits,
		uint32_t memoryPropertyBits, uint32_t& typeIndex, uint32_t& offset)
	{
		typeIndex = 0;
		uint32_t typeBits = memoryTypeBits;

		while (typeBits)
		{
			if (typeBits & 1)
			{
				int res = m_pDevice->GetPhysicalDevice()->GetPhysicalDeviceMemoryProperties().memoryTypes[typeIndex].propertyFlags & memoryPropertyBits;
				if (res == memoryPropertyBits)
				{
					break;
				}
			}
			typeBits >>= 1;
			typeIndex++;
		}

		if (m_bufferMemPool[typeIndex].memory == 0)
		{
			MemoryNode node;
			node.numBytes = (memoryPropertyBits & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) ? STAGING_MEMORY_ALLOCATE_INC : DEVICE_MEMORY_ALLOCATE_INC;
			node.memProperty = memoryPropertyBits;

			VkMemoryAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocInfo.memoryTypeIndex = typeIndex;
			allocInfo.allocationSize = node.numBytes;
			CHECK_VK_ERROR(vkAllocateMemory(m_pDevice->GetDeviceHandle(), &allocInfo, nullptr, &node.memory));

			if (memoryPropertyBits & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
				CHECK_VK_ERROR(vkMapMemory(m_pDevice->GetDeviceHandle(), node.memory, 0, numBytes, 0, &node.pData));

			m_bufferMemPool[typeIndex] = node;
		}

		if (!FindFreeBufferMemoryChunk(key, typeIndex, numBytes, offset))
		{
			// Should create a larger chunck of memory, do it later
			assert(false);
		}
	}

	void DeviceMemoryManager::AllocateImageMemory(uint32_t key, uint32_t numBytes, uint32_t memoryTypeBits, uint32_t memoryPropertyBits, uint32_t& typeIndex, uint32_t& offset)
	{
		typeIndex = 0;
		uint32_t typeBits = memoryTypeBits;
		while (typeBits)
		{
			if (typeBits & 1)
			{
				if (m_pDevice->GetPhysicalDevice()->GetPhysicalDeviceMemoryProperties().memoryTypes[typeIndex].propertyFlags & memoryPropertyBits)
				{
					break;
				}
			}
			typeBits >>= 1;
			typeIndex++;
		}

		MemoryNode node;
		node.numBytes = numBytes;
		node.memProperty = memoryPropertyBits;
		
		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = numBytes;
		allocInfo.memoryTypeIndex = typeIndex;
		CHECK_VK_ERROR(vkAllocateMemory(m_pDevice->GetDeviceHandle(), &allocInfo, nullptr, &node.memory));

		offset = 0;

		uint32_t imageMemPoolIndex = (uint32_t)m_imageMemPool.size();
		m_imageMemPool.push_back(node);

		if (key >= imageMemPoolIndex)
		{
			std::vector<std::pair<uint32_t, bool>> newLookupTable(m_imageMemPoolLookupTable.size() + LOOKUP_TABLE_SIZE_INC);
			std::swap_ranges(m_imageMemPoolLookupTable.begin(), m_imageMemPoolLookupTable.end(), newLookupTable.begin());
			m_imageMemPoolLookupTable = std::move(newLookupTable);
		}
		
		m_imageMemPoolLookupTable[key] = { imageMemPoolIndex, false };
	}

	void DeviceMemoryManager::FreeImageMemChunk(uint32_t key)
	{
		auto index = m_imageMemPoolLookupTable[key];

		vkFreeMemory(m_pDevice->GetDeviceHandle(), m_imageMemPool[index.first].memory, nullptr);

		m_imageMemPoolLookupTable[key].second = true;
	}

	void DeviceMemoryManager::FreeBufferMemChunk(uint32_t key)
	{
		if (m_bufferBindingLookupTable[key].second)
			return;

		auto& bindingInfo = m_bufferBindingTable[m_bufferBindingLookupTable[key].first];

		MemoryNode& node = m_bufferMemPool[bindingInfo.first.typeIndex];
		auto memIter = std::find_if(node.bindingList.begin(), node.bindingList.end(), [key](uint32_t poolKey)
		{
			return poolKey == key;
		});
		if (memIter == node.bindingList.end())
			return;

		m_bufferBindingLookupTable[key].second = true;
		m_bufferBindingTable[m_bufferBindingLookupTable[key].first].second = true;

		node.bindingList.erase(memIter);
	}
}