#include "DeviceMemoryManager.h"
#include <algorithm>
#include "Buffer.h"
#include "Image.h"

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

	}

	// ---------------------------- DeviceMemoryManager ---------------------------------------
	DeviceMemoryManager::DeviceMemoryManager(const std::shared_ptr<Device> pDevice)
		: m_pDevice(pDevice)
	{
		m_bufferMemPool.resize(sizeof(uint32_t) * 8, {}); // Same size as bit count of uint32_t, i.e. type index count
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