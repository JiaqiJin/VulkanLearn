#pragma once

#include <vulkan/vulkan.h>
#include <map>
#include <unordered_map>
#include <memory>
#include <vector>
/*
* Memory Management for buffers : There will be 32 chunk of memory in "Buffer Mem Pool",
* exactly the same as Vulkan physical device provided "VK_MAX_MEMORY_TYPES", and each of them consiste size, data ptr,
* handle to Vulkan memory obj and the KEY. KEY act role to index the actual binding information and it'll be generated and kept by each buffer, to look up informations in binding table,
* and using "type index" to acquire Vulkan memory object from "Buffer Memory Pool"
* Memory Management for buffers Table : 
* 
 Buffer MEM POOL			Binding Lookup Table		Binding Info Table
--------------- 			--------------- 			---------------
|	Mem Node(0)		|		|		Key 0		|		|	Binding Info(0)	||
|	pData, NumByte  |	  ->|	|BindingInfo|	|		|	|Type Index|	||
|	vkMemory		|	-/	|	-Index			|		|	|Start Byte|	||
|	Binding List	|--	/	|	|Freed Flag|	|		|	| numByte  |	||
|	|key0, key1|	|----->	|		Key1		|	  ->|	|  pData   |	||
|	|key2, key3|	|		|	|BindingInfo|	|	-/	|					||
|		.			|		|	-Index			|-/		|		.			||
|		.			|		|	|Freed Flag|	|		|		.			||
|		.			|		|		.			|		|					||
|	Mem Node(32)	|->		|		.			|		|	Binding Info(n)	||

* Memory Management for images : 
* Each image must bind a different memory object. So image buffer pool doesn't update a binding list for multiple images.
* And a "binding info table" is not necessary too.
* The only thing left same as buffer memory management is lookup table, which is used for key->memory node indexing. 
*/
namespace RHI
{
	class Buffer;
	class Image;
	class Device;
	class DeviceMemoryManager;

	class MemoryKey
	{
	public:
		MemoryKey(const std::shared_ptr<DeviceMemoryManager>& pDeviceMemMgr, bool bufferOrImage);
		~MemoryKey();

	private:
		uint32_t m_key;
		bool m_bufferOrImage; //true: buffer, false: image
		static uint32_t m_allocatedKeys;
		std::shared_ptr<DeviceMemoryManager> m_pDeviceMemMgr;

		friend class DeviceMemoryManager;
	};

	class DeviceMemoryManager
	{
	private:
		typedef struct _MemoryNode
		{
			uint32_t numBytes = 0;
			uint32_t memProperty = 0;
			void* pData = nullptr;
			VkDeviceMemory memory;
			std::vector<uint32_t> bindingList;
		} MemoryNode;

		typedef struct _BindingInfo
		{
			uint32_t typeIndex;
			uint32_t startByte = 0;
			uint32_t numBytes = 0;
			void* pData = nullptr;
		} BindingInfo;

	public:
		static const uint32_t DEVICE_MEMORY_ALLOCATE_INC = 1024 * 1024 * 512;
		static const uint32_t STAGING_MEMORY_ALLOCATE_INC = 1024 * 1024 * 256;

	public:
		DeviceMemoryManager(const std::shared_ptr<Device> pDevice);
		~DeviceMemoryManager();

	protected:
		// Memory allocation
		void AllocateBufferMemory(uint32_t key, uint32_t numBytes, uint32_t memoryTypeBits, uint32_t memoryPropertyBits, 
			uint32_t& typeIndex, uint32_t& offset);
		bool FindFreeBufferMemoryChunk(uint32_t key, uint32_t typeIndex, uint32_t numBytes, uint32_t& offset);
		void AllocateImageMemory(uint32_t key, uint32_t numBytes, uint32_t memoryTypeBits, 
			uint32_t memoryPropertyBits, uint32_t& typeIndex, uint32_t& offset);

		void FreeBufferMemChunk(uint32_t key);
		void FreeImageMemChunk(uint32_t key);

	private:
		std::vector<MemoryNode> m_bufferMemPool;
		std::vector<MemoryNode> m_imageMemPool;
		std::vector<std::pair<BindingInfo, bool>> m_bufferBindingTable; // bool stands for whether it's freed

		// uint32_t stands for actual image mem pool index
		// bool stands for whether it's freed
		std::vector<std::pair<uint32_t, bool>> m_imageMemPoolLookupTable;

		// uint32_t stands for actual buffer binding table index
		// bool stands for whether it's freed
		std::vector<std::pair<uint32_t, bool>> m_bufferBindingLookupTable;

		static const uint32_t LOOKUP_TABLE_SIZE_INC = 256;

		std::shared_ptr<Device> m_pDevice;
		friend class MemoryKey;
	};
}