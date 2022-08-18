#include "vulkan_api.h"

namespace plex::vkapi
{

// clang-format off

namespace
{
static VkDevice device_ = VK_NULL_HANDLE; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
} // namespace

void InitVulkanApi(VkDevice device)
{
    device_ = device;
}

VulkanResult vkCreateInstance(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance)
{
    return ::vkCreateInstance(pCreateInfo, pAllocator, pInstance);
}

void vkDestroyInstance(VkInstance instance, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyInstance(instance, pAllocator);
}

VulkanResultWithValue<std::vector<VkPhysicalDevice>> vkEnumeratePhysicalDevices(VkInstance instance)
{
    uint32_t count = 0;
    ::vkEnumeratePhysicalDevices(instance, &count, nullptr);
    VulkanResultWithValue<std::vector<VkPhysicalDevice>> result;
    result.value.resize(count);
    result.result = ::vkEnumeratePhysicalDevices(instance, &count, result.value.data());
    return result;
}

void vkGetPhysicalDeviceFeatures(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures)
{
    return ::vkGetPhysicalDeviceFeatures(physicalDevice, pFeatures);
}

void vkGetPhysicalDeviceFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties* pFormatProperties)
{
    return ::vkGetPhysicalDeviceFormatProperties(physicalDevice, format, pFormatProperties);
}

VulkanResult vkGetPhysicalDeviceImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties)
{
    return ::vkGetPhysicalDeviceImageFormatProperties(physicalDevice, format, type, tiling, usage, flags, pImageFormatProperties);
}

void vkGetPhysicalDeviceProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties)
{
    return ::vkGetPhysicalDeviceProperties(physicalDevice, pProperties);
}

VulkanResultWithValue<std::vector<VkQueueFamilyProperties>> vkGetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice physicalDevice)
{
    uint32_t count = 0;
    ::vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, nullptr);
    VulkanResultWithValue<std::vector<VkQueueFamilyProperties>> result;
    result.value.resize(count);
    ::vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, result.value.data());
    return result;
}

void vkGetPhysicalDeviceMemoryProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties)
{
    return ::vkGetPhysicalDeviceMemoryProperties(physicalDevice, pMemoryProperties);
}

VulkanFunctionPointer vkGetInstanceProcAddr(VkInstance instance, const char* pName)
{
    return ::vkGetInstanceProcAddr(instance, pName);
}

VulkanFunctionPointer vkGetDeviceProcAddr(const char* pName)
{
    return ::vkGetDeviceProcAddr(device_, pName);
}

VulkanResult vkCreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice)
{
    return ::vkCreateDevice(physicalDevice, pCreateInfo, pAllocator, pDevice);
}

void vkDestroyDevice(const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyDevice(device_, pAllocator);
}

VulkanResultWithValue<std::vector<VkExtensionProperties>> vkEnumerateInstanceExtensionProperties(const char* pLayerName)
{
    uint32_t count = 0;
    ::vkEnumerateInstanceExtensionProperties(pLayerName, &count, nullptr);
    VulkanResultWithValue<std::vector<VkExtensionProperties>> result;
    result.value.resize(count);
    result.result = ::vkEnumerateInstanceExtensionProperties(pLayerName, &count, result.value.data());
    return result;
}

VulkanResultWithValue<std::vector<VkExtensionProperties>> vkEnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice, const char* pLayerName)
{
    uint32_t count = 0;
    ::vkEnumerateDeviceExtensionProperties(physicalDevice, pLayerName, &count, nullptr);
    VulkanResultWithValue<std::vector<VkExtensionProperties>> result;
    result.value.resize(count);
    result.result = ::vkEnumerateDeviceExtensionProperties(physicalDevice, pLayerName, &count, result.value.data());
    return result;
}

VulkanResult vkEnumerateInstanceLayerProperties(uint32_t* pPropertyCount, VkLayerProperties* pProperties)
{
    return ::vkEnumerateInstanceLayerProperties(pPropertyCount, pProperties);
}

VulkanResultWithValue<std::vector<VkLayerProperties>> vkEnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice)
{
    uint32_t count = 0;
    ::vkEnumerateDeviceLayerProperties(physicalDevice, &count, nullptr);
    VulkanResultWithValue<std::vector<VkLayerProperties>> result;
    result.value.resize(count);
    result.result = ::vkEnumerateDeviceLayerProperties(physicalDevice, &count, result.value.data());
    return result;
}

void vkGetDeviceQueue(uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue)
{
    return ::vkGetDeviceQueue(device_, queueFamilyIndex, queueIndex, pQueue);
}

VulkanResult vkQueueSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence)
{
    return ::vkQueueSubmit(queue, submitCount, pSubmits, fence);
}

VulkanResult vkQueueWaitIdle(VkQueue queue)
{
    return ::vkQueueWaitIdle(queue);
}

VulkanResult vkDeviceWaitIdle()
{
    return ::vkDeviceWaitIdle(device_);
}

VulkanResult vkAllocateMemory(const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory)
{
    return ::vkAllocateMemory(device_, pAllocateInfo, pAllocator, pMemory);
}

void vkFreeMemory(VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator)
{
    return ::vkFreeMemory(device_, memory, pAllocator);
}

VulkanResult vkMapMemory(VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData)
{
    return ::vkMapMemory(device_, memory, offset, size, flags, ppData);
}

void vkUnmapMemory(VkDeviceMemory memory)
{
    return ::vkUnmapMemory(device_, memory);
}

VulkanResult vkFlushMappedMemoryRanges(uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges)
{
    return ::vkFlushMappedMemoryRanges(device_, memoryRangeCount, pMemoryRanges);
}

VulkanResult vkInvalidateMappedMemoryRanges(uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges)
{
    return ::vkInvalidateMappedMemoryRanges(device_, memoryRangeCount, pMemoryRanges);
}

void vkGetDeviceMemoryCommitment(VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes)
{
    return ::vkGetDeviceMemoryCommitment(device_, memory, pCommittedMemoryInBytes);
}

VulkanResult vkBindBufferMemory(VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset)
{
    return ::vkBindBufferMemory(device_, buffer, memory, memoryOffset);
}

VulkanResult vkBindImageMemory(VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset)
{
    return ::vkBindImageMemory(device_, image, memory, memoryOffset);
}

void vkGetBufferMemoryRequirements(VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements)
{
    return ::vkGetBufferMemoryRequirements(device_, buffer, pMemoryRequirements);
}

void vkGetImageMemoryRequirements(VkImage image, VkMemoryRequirements* pMemoryRequirements)
{
    return ::vkGetImageMemoryRequirements(device_, image, pMemoryRequirements);
}

void vkGetImageSparseMemoryRequirements(VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements)
{
    return ::vkGetImageSparseMemoryRequirements(device_, image, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}

VulkanResultWithValue<std::vector<VkSparseImageFormatProperties>> vkGetPhysicalDeviceSparseImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling)
{
    uint32_t count = 0;
    ::vkGetPhysicalDeviceSparseImageFormatProperties(physicalDevice, format, type, samples, usage, tiling, &count, nullptr);
    VulkanResultWithValue<std::vector<VkSparseImageFormatProperties>> result;
    result.value.resize(count);
    ::vkGetPhysicalDeviceSparseImageFormatProperties(physicalDevice, format, type, samples, usage, tiling, &count, result.value.data());
    return result;
}

VulkanResult vkQueueBindSparse(VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence)
{
    return ::vkQueueBindSparse(queue, bindInfoCount, pBindInfo, fence);
}

VulkanResult vkCreateFence(const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence)
{
    return ::vkCreateFence(device_, pCreateInfo, pAllocator, pFence);
}

void vkDestroyFence(VkFence fence, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyFence(device_, fence, pAllocator);
}

VulkanResult vkResetFences(uint32_t fenceCount, const VkFence* pFences)
{
    return ::vkResetFences(device_, fenceCount, pFences);
}

VulkanResult vkGetFenceStatus(VkFence fence)
{
    return ::vkGetFenceStatus(device_, fence);
}

VulkanResult vkWaitForFences(uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout)
{
    return ::vkWaitForFences(device_, fenceCount, pFences, waitAll, timeout);
}

VulkanResult vkCreateSemaphore(const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore)
{
    return ::vkCreateSemaphore(device_, pCreateInfo, pAllocator, pSemaphore);
}

void vkDestroySemaphore(VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroySemaphore(device_, semaphore, pAllocator);
}

VulkanResult vkCreateEvent(const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent)
{
    return ::vkCreateEvent(device_, pCreateInfo, pAllocator, pEvent);
}

void vkDestroyEvent(VkEvent event, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyEvent(device_, event, pAllocator);
}

VulkanResult vkGetEventStatus(VkEvent event)
{
    return ::vkGetEventStatus(device_, event);
}

VulkanResult vkSetEvent(VkEvent event)
{
    return ::vkSetEvent(device_, event);
}

VulkanResult vkResetEvent(VkEvent event)
{
    return ::vkResetEvent(device_, event);
}

VulkanResult vkCreateQueryPool(const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool)
{
    return ::vkCreateQueryPool(device_, pCreateInfo, pAllocator, pQueryPool);
}

void vkDestroyQueryPool(VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyQueryPool(device_, queryPool, pAllocator);
}

VulkanResult vkGetQueryPoolResults(VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags)
{
    return ::vkGetQueryPoolResults(device_, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags);
}

VulkanResult vkCreateBuffer(const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer)
{
    return ::vkCreateBuffer(device_, pCreateInfo, pAllocator, pBuffer);
}

void vkDestroyBuffer(VkBuffer buffer, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyBuffer(device_, buffer, pAllocator);
}

VulkanResult vkCreateBufferView(const VkBufferViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferView* pView)
{
    return ::vkCreateBufferView(device_, pCreateInfo, pAllocator, pView);
}

void vkDestroyBufferView(VkBufferView bufferView, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyBufferView(device_, bufferView, pAllocator);
}

VulkanResult vkCreateImage(const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage)
{
    return ::vkCreateImage(device_, pCreateInfo, pAllocator, pImage);
}

void vkDestroyImage(VkImage image, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyImage(device_, image, pAllocator);
}

void vkGetImageSubresourceLayout(VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout)
{
    return ::vkGetImageSubresourceLayout(device_, image, pSubresource, pLayout);
}

VulkanResult vkCreateImageView(const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView)
{
    return ::vkCreateImageView(device_, pCreateInfo, pAllocator, pView);
}

void vkDestroyImageView(VkImageView imageView, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyImageView(device_, imageView, pAllocator);
}

VulkanResult vkCreateShaderModule(const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule)
{
    return ::vkCreateShaderModule(device_, pCreateInfo, pAllocator, pShaderModule);
}

void vkDestroyShaderModule(VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyShaderModule(device_, shaderModule, pAllocator);
}

VulkanResult vkCreatePipelineCache(const VkPipelineCacheCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache)
{
    return ::vkCreatePipelineCache(device_, pCreateInfo, pAllocator, pPipelineCache);
}

void vkDestroyPipelineCache(VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyPipelineCache(device_, pipelineCache, pAllocator);
}

VulkanResult vkGetPipelineCacheData(VkPipelineCache pipelineCache, size_t* pDataSize, void* pData)
{
    return ::vkGetPipelineCacheData(device_, pipelineCache, pDataSize, pData);
}

VulkanResult vkMergePipelineCaches(VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches)
{
    return ::vkMergePipelineCaches(device_, dstCache, srcCacheCount, pSrcCaches);
}

VulkanResult vkCreateGraphicsPipelines(VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
{
    return ::vkCreateGraphicsPipelines(device_, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}

VulkanResult vkCreateComputePipelines(VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
{
    return ::vkCreateComputePipelines(device_, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}

void vkDestroyPipeline(VkPipeline pipeline, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyPipeline(device_, pipeline, pAllocator);
}

VulkanResult vkCreatePipelineLayout(const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout)
{
    return ::vkCreatePipelineLayout(device_, pCreateInfo, pAllocator, pPipelineLayout);
}

void vkDestroyPipelineLayout(VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyPipelineLayout(device_, pipelineLayout, pAllocator);
}

VulkanResult vkCreateSampler(const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler)
{
    return ::vkCreateSampler(device_, pCreateInfo, pAllocator, pSampler);
}

void vkDestroySampler(VkSampler sampler, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroySampler(device_, sampler, pAllocator);
}

VulkanResult vkCreateDescriptorSetLayout(const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout)
{
    return ::vkCreateDescriptorSetLayout(device_, pCreateInfo, pAllocator, pSetLayout);
}

void vkDestroyDescriptorSetLayout(VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyDescriptorSetLayout(device_, descriptorSetLayout, pAllocator);
}

VulkanResult vkCreateDescriptorPool(const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool)
{
    return ::vkCreateDescriptorPool(device_, pCreateInfo, pAllocator, pDescriptorPool);
}

void vkDestroyDescriptorPool(VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyDescriptorPool(device_, descriptorPool, pAllocator);
}

VulkanResult vkResetDescriptorPool(VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags)
{
    return ::vkResetDescriptorPool(device_, descriptorPool, flags);
}

VulkanResult vkAllocateDescriptorSets(const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets)
{
    return ::vkAllocateDescriptorSets(device_, pAllocateInfo, pDescriptorSets);
}

VulkanResult vkFreeDescriptorSets(VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets)
{
    return ::vkFreeDescriptorSets(device_, descriptorPool, descriptorSetCount, pDescriptorSets);
}

void vkUpdateDescriptorSets(uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies)
{
    return ::vkUpdateDescriptorSets(device_, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies);
}

VulkanResult vkCreateFramebuffer(const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer)
{
    return ::vkCreateFramebuffer(device_, pCreateInfo, pAllocator, pFramebuffer);
}

void vkDestroyFramebuffer(VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyFramebuffer(device_, framebuffer, pAllocator);
}

VulkanResult vkCreateRenderPass(const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass)
{
    return ::vkCreateRenderPass(device_, pCreateInfo, pAllocator, pRenderPass);
}

void vkDestroyRenderPass(VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyRenderPass(device_, renderPass, pAllocator);
}

void vkGetRenderAreaGranularity(VkRenderPass renderPass, VkExtent2D* pGranularity)
{
    return ::vkGetRenderAreaGranularity(device_, renderPass, pGranularity);
}

VulkanResult vkCreateCommandPool(const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool)
{
    return ::vkCreateCommandPool(device_, pCreateInfo, pAllocator, pCommandPool);
}

void vkDestroyCommandPool(VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyCommandPool(device_, commandPool, pAllocator);
}

VulkanResult vkResetCommandPool(VkCommandPool commandPool, VkCommandPoolResetFlags flags)
{
    return ::vkResetCommandPool(device_, commandPool, flags);
}

VulkanResult vkAllocateCommandBuffers(const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers)
{
    return ::vkAllocateCommandBuffers(device_, pAllocateInfo, pCommandBuffers);
}

void vkFreeCommandBuffers(VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers)
{
    return ::vkFreeCommandBuffers(device_, commandPool, commandBufferCount, pCommandBuffers);
}

VulkanResult vkEnumerateInstanceVersion(uint32_t* pApiVersion)
{
    return ::vkEnumerateInstanceVersion(pApiVersion);
}

VulkanResult vkBindBufferMemory2(uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos)
{
    return ::vkBindBufferMemory2(device_, bindInfoCount, pBindInfos);
}

VulkanResult vkBindImageMemory2(uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos)
{
    return ::vkBindImageMemory2(device_, bindInfoCount, pBindInfos);
}

void vkGetDeviceGroupPeerMemoryFeatures(uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures)
{
    return ::vkGetDeviceGroupPeerMemoryFeatures(device_, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
}

VulkanResultWithValue<std::vector<VkPhysicalDeviceGroupProperties>> vkEnumeratePhysicalDeviceGroups(VkInstance instance)
{
    uint32_t count = 0;
    ::vkEnumeratePhysicalDeviceGroups(instance, &count, nullptr);
    VulkanResultWithValue<std::vector<VkPhysicalDeviceGroupProperties>> result;
    result.value.resize(count);
    result.result = ::vkEnumeratePhysicalDeviceGroups(instance, &count, result.value.data());
    return result;
}

void vkGetImageMemoryRequirements2(const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
    return ::vkGetImageMemoryRequirements2(device_, pInfo, pMemoryRequirements);
}

void vkGetBufferMemoryRequirements2(const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
    return ::vkGetBufferMemoryRequirements2(device_, pInfo, pMemoryRequirements);
}

void vkGetImageSparseMemoryRequirements2(const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements)
{
    return ::vkGetImageSparseMemoryRequirements2(device_, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}

void vkGetPhysicalDeviceFeatures2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures)
{
    return ::vkGetPhysicalDeviceFeatures2(physicalDevice, pFeatures);
}

void vkGetPhysicalDeviceProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties)
{
    return ::vkGetPhysicalDeviceProperties2(physicalDevice, pProperties);
}

void vkGetPhysicalDeviceFormatProperties2(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties)
{
    return ::vkGetPhysicalDeviceFormatProperties2(physicalDevice, format, pFormatProperties);
}

VulkanResult vkGetPhysicalDeviceImageFormatProperties2(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties)
{
    return ::vkGetPhysicalDeviceImageFormatProperties2(physicalDevice, pImageFormatInfo, pImageFormatProperties);
}

VulkanResultWithValue<std::vector<VkQueueFamilyProperties2>> vkGetPhysicalDeviceQueueFamilyProperties2(VkPhysicalDevice physicalDevice)
{
    uint32_t count = 0;
    ::vkGetPhysicalDeviceQueueFamilyProperties2(physicalDevice, &count, nullptr);
    VulkanResultWithValue<std::vector<VkQueueFamilyProperties2>> result;
    result.value.resize(count);
    ::vkGetPhysicalDeviceQueueFamilyProperties2(physicalDevice, &count, result.value.data());
    return result;
}

void vkGetPhysicalDeviceMemoryProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties)
{
    return ::vkGetPhysicalDeviceMemoryProperties2(physicalDevice, pMemoryProperties);
}

VulkanResultWithValue<std::vector<VkSparseImageFormatProperties2>> vkGetPhysicalDeviceSparseImageFormatProperties2(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo)
{
    uint32_t count = 0;
    ::vkGetPhysicalDeviceSparseImageFormatProperties2(physicalDevice, pFormatInfo, &count, nullptr);
    VulkanResultWithValue<std::vector<VkSparseImageFormatProperties2>> result;
    result.value.resize(count);
    ::vkGetPhysicalDeviceSparseImageFormatProperties2(physicalDevice, pFormatInfo, &count, result.value.data());
    return result;
}

void vkTrimCommandPool(VkCommandPool commandPool, VkCommandPoolTrimFlags flags)
{
    return ::vkTrimCommandPool(device_, commandPool, flags);
}

void vkGetDeviceQueue2(const VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue)
{
    return ::vkGetDeviceQueue2(device_, pQueueInfo, pQueue);
}

VulkanResult vkCreateSamplerYcbcrConversion(const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion)
{
    return ::vkCreateSamplerYcbcrConversion(device_, pCreateInfo, pAllocator, pYcbcrConversion);
}

void vkDestroySamplerYcbcrConversion(VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroySamplerYcbcrConversion(device_, ycbcrConversion, pAllocator);
}

VulkanResult vkCreateDescriptorUpdateTemplate(const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate)
{
    return ::vkCreateDescriptorUpdateTemplate(device_, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
}

void vkDestroyDescriptorUpdateTemplate(VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyDescriptorUpdateTemplate(device_, descriptorUpdateTemplate, pAllocator);
}

void vkUpdateDescriptorSetWithTemplate(VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData)
{
    return ::vkUpdateDescriptorSetWithTemplate(device_, descriptorSet, descriptorUpdateTemplate, pData);
}

void vkGetPhysicalDeviceExternalBufferProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties)
{
    return ::vkGetPhysicalDeviceExternalBufferProperties(physicalDevice, pExternalBufferInfo, pExternalBufferProperties);
}

void vkGetPhysicalDeviceExternalFenceProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties)
{
    return ::vkGetPhysicalDeviceExternalFenceProperties(physicalDevice, pExternalFenceInfo, pExternalFenceProperties);
}

void vkGetPhysicalDeviceExternalSemaphoreProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties)
{
    return ::vkGetPhysicalDeviceExternalSemaphoreProperties(physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
}

void vkGetDescriptorSetLayoutSupport(const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport)
{
    return ::vkGetDescriptorSetLayoutSupport(device_, pCreateInfo, pSupport);
}

VulkanResult vkCreateRenderPass2(const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass)
{
    return ::vkCreateRenderPass2(device_, pCreateInfo, pAllocator, pRenderPass);
}

void vkResetQueryPool(VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount)
{
    return ::vkResetQueryPool(device_, queryPool, firstQuery, queryCount);
}

VulkanResult vkGetSemaphoreCounterValue(VkSemaphore semaphore, uint64_t* pValue)
{
    return ::vkGetSemaphoreCounterValue(device_, semaphore, pValue);
}

VulkanResult vkWaitSemaphores(const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout)
{
    return ::vkWaitSemaphores(device_, pWaitInfo, timeout);
}

VulkanResult vkSignalSemaphore(const VkSemaphoreSignalInfo* pSignalInfo)
{
    return ::vkSignalSemaphore(device_, pSignalInfo);
}

VulkanResultWithValue<VkDeviceAddress> vkGetBufferDeviceAddress(const VkBufferDeviceAddressInfo* pInfo)
{
    return ::vkGetBufferDeviceAddress(device_, pInfo);
}

VulkanResultWithValue<uint64_t> vkGetBufferOpaqueCaptureAddress(const VkBufferDeviceAddressInfo* pInfo)
{
    return ::vkGetBufferOpaqueCaptureAddress(device_, pInfo);
}

VulkanResultWithValue<uint64_t> vkGetDeviceMemoryOpaqueCaptureAddress(const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo)
{
    return ::vkGetDeviceMemoryOpaqueCaptureAddress(device_, pInfo);
}

VulkanResultWithValue<std::vector<VkPhysicalDeviceToolProperties>> vkGetPhysicalDeviceToolProperties(VkPhysicalDevice physicalDevice)
{
    uint32_t count = 0;
    ::vkGetPhysicalDeviceToolProperties(physicalDevice, &count, nullptr);
    VulkanResultWithValue<std::vector<VkPhysicalDeviceToolProperties>> result;
    result.value.resize(count);
    result.result = ::vkGetPhysicalDeviceToolProperties(physicalDevice, &count, result.value.data());
    return result;
}

VulkanResult vkCreatePrivateDataSlot(const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot)
{
    return ::vkCreatePrivateDataSlot(device_, pCreateInfo, pAllocator, pPrivateDataSlot);
}

void vkDestroyPrivateDataSlot(VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyPrivateDataSlot(device_, privateDataSlot, pAllocator);
}

VulkanResult vkSetPrivateData(VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data)
{
    return ::vkSetPrivateData(device_, objectType, objectHandle, privateDataSlot, data);
}

void vkGetPrivateData(VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData)
{
    return ::vkGetPrivateData(device_, objectType, objectHandle, privateDataSlot, pData);
}

VulkanResult vkQueueSubmit2(VkQueue queue, uint32_t submitCount, const VkSubmitInfo2* pSubmits, VkFence fence)
{
    return ::vkQueueSubmit2(queue, submitCount, pSubmits, fence);
}

void vkGetDeviceBufferMemoryRequirements(const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
    return ::vkGetDeviceBufferMemoryRequirements(device_, pInfo, pMemoryRequirements);
}

void vkGetDeviceImageMemoryRequirements(const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
    return ::vkGetDeviceImageMemoryRequirements(device_, pInfo, pMemoryRequirements);
}

void vkGetDeviceImageSparseMemoryRequirements(const VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements)
{
    return ::vkGetDeviceImageSparseMemoryRequirements(device_, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}

void vkDestroySurfaceKHR(VkInstance instance, VkSurfaceKHR surface, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroySurfaceKHR(instance, surface, pAllocator);
}

VulkanResult vkGetPhysicalDeviceSurfaceSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported)
{
    return ::vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, surface, pSupported);
}

VulkanResult vkGetPhysicalDeviceSurfaceCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities)
{
    return ::vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, pSurfaceCapabilities);
}

VulkanResultWithValue<std::vector<VkSurfaceFormatKHR>> vkGetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
    uint32_t count = 0;
    ::vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &count, nullptr);
    VulkanResultWithValue<std::vector<VkSurfaceFormatKHR>> result;
    result.value.resize(count);
    result.result = ::vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &count, result.value.data());
    return result;
}

VulkanResultWithValue<std::vector<VkPresentModeKHR>> vkGetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
    uint32_t count = 0;
    ::vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &count, nullptr);
    VulkanResultWithValue<std::vector<VkPresentModeKHR>> result;
    result.value.resize(count);
    result.result = ::vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &count, result.value.data());
    return result;
}

VulkanResult vkCreateSwapchainKHR(const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain)
{
    return ::vkCreateSwapchainKHR(device_, pCreateInfo, pAllocator, pSwapchain);
}

void vkDestroySwapchainKHR(VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroySwapchainKHR(device_, swapchain, pAllocator);
}

VulkanResult vkGetSwapchainImagesKHR(VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages)
{
    return ::vkGetSwapchainImagesKHR(device_, swapchain, pSwapchainImageCount, pSwapchainImages);
}

VulkanResult vkAcquireNextImageKHR(VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex)
{
    return ::vkAcquireNextImageKHR(device_, swapchain, timeout, semaphore, fence, pImageIndex);
}

VulkanResult vkQueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo)
{
    return ::vkQueuePresentKHR(queue, pPresentInfo);
}

VulkanResult vkGetDeviceGroupPresentCapabilitiesKHR(VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities)
{
    return ::vkGetDeviceGroupPresentCapabilitiesKHR(device_, pDeviceGroupPresentCapabilities);
}

VulkanResult vkGetDeviceGroupSurfacePresentModesKHR(VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes)
{
    return ::vkGetDeviceGroupSurfacePresentModesKHR(device_, surface, pModes);
}

VulkanResultWithValue<std::vector<VkRect2D>> vkGetPhysicalDevicePresentRectanglesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
    uint32_t count = 0;
    ::vkGetPhysicalDevicePresentRectanglesKHR(physicalDevice, surface, &count, nullptr);
    VulkanResultWithValue<std::vector<VkRect2D>> result;
    result.value.resize(count);
    result.result = ::vkGetPhysicalDevicePresentRectanglesKHR(physicalDevice, surface, &count, result.value.data());
    return result;
}

VulkanResult vkAcquireNextImage2KHR(const VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex)
{
    return ::vkAcquireNextImage2KHR(device_, pAcquireInfo, pImageIndex);
}

VulkanResultWithValue<std::vector<VkDisplayPropertiesKHR>> vkGetPhysicalDeviceDisplayPropertiesKHR(VkPhysicalDevice physicalDevice)
{
    uint32_t count = 0;
    ::vkGetPhysicalDeviceDisplayPropertiesKHR(physicalDevice, &count, nullptr);
    VulkanResultWithValue<std::vector<VkDisplayPropertiesKHR>> result;
    result.value.resize(count);
    result.result = ::vkGetPhysicalDeviceDisplayPropertiesKHR(physicalDevice, &count, result.value.data());
    return result;
}

VulkanResultWithValue<std::vector<VkDisplayPlanePropertiesKHR>> vkGetPhysicalDeviceDisplayPlanePropertiesKHR(VkPhysicalDevice physicalDevice)
{
    uint32_t count = 0;
    ::vkGetPhysicalDeviceDisplayPlanePropertiesKHR(physicalDevice, &count, nullptr);
    VulkanResultWithValue<std::vector<VkDisplayPlanePropertiesKHR>> result;
    result.value.resize(count);
    result.result = ::vkGetPhysicalDeviceDisplayPlanePropertiesKHR(physicalDevice, &count, result.value.data());
    return result;
}

VulkanResultWithValue<std::vector<VkDisplayKHR>> vkGetDisplayPlaneSupportedDisplaysKHR(VkPhysicalDevice physicalDevice, uint32_t planeIndex)
{
    uint32_t count = 0;
    ::vkGetDisplayPlaneSupportedDisplaysKHR(physicalDevice, planeIndex, &count, nullptr);
    VulkanResultWithValue<std::vector<VkDisplayKHR>> result;
    result.value.resize(count);
    result.result = ::vkGetDisplayPlaneSupportedDisplaysKHR(physicalDevice, planeIndex, &count, result.value.data());
    return result;
}

VulkanResultWithValue<std::vector<VkDisplayModePropertiesKHR>> vkGetDisplayModePropertiesKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display)
{
    uint32_t count = 0;
    ::vkGetDisplayModePropertiesKHR(physicalDevice, display, &count, nullptr);
    VulkanResultWithValue<std::vector<VkDisplayModePropertiesKHR>> result;
    result.value.resize(count);
    result.result = ::vkGetDisplayModePropertiesKHR(physicalDevice, display, &count, result.value.data());
    return result;
}

VulkanResult vkCreateDisplayModeKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, const VkDisplayModeCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDisplayModeKHR* pMode)
{
    return ::vkCreateDisplayModeKHR(physicalDevice, display, pCreateInfo, pAllocator, pMode);
}

VulkanResult vkGetDisplayPlaneCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkDisplayModeKHR mode, uint32_t planeIndex, VkDisplayPlaneCapabilitiesKHR* pCapabilities)
{
    return ::vkGetDisplayPlaneCapabilitiesKHR(physicalDevice, mode, planeIndex, pCapabilities);
}

VulkanResult vkCreateDisplayPlaneSurfaceKHR(VkInstance instance, const VkDisplaySurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    return ::vkCreateDisplayPlaneSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
}

VulkanResult vkCreateSharedSwapchainsKHR(uint32_t swapchainCount, const VkSwapchainCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchains)
{
    return ::vkCreateSharedSwapchainsKHR(device_, swapchainCount, pCreateInfos, pAllocator, pSwapchains);
}

void vkGetPhysicalDeviceFeatures2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures)
{
    return ::vkGetPhysicalDeviceFeatures2KHR(physicalDevice, pFeatures);
}

void vkGetPhysicalDeviceProperties2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties)
{
    return ::vkGetPhysicalDeviceProperties2KHR(physicalDevice, pProperties);
}

void vkGetPhysicalDeviceFormatProperties2KHR(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties)
{
    return ::vkGetPhysicalDeviceFormatProperties2KHR(physicalDevice, format, pFormatProperties);
}

VulkanResult vkGetPhysicalDeviceImageFormatProperties2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties)
{
    return ::vkGetPhysicalDeviceImageFormatProperties2KHR(physicalDevice, pImageFormatInfo, pImageFormatProperties);
}

VulkanResultWithValue<std::vector<VkQueueFamilyProperties2>> vkGetPhysicalDeviceQueueFamilyProperties2KHR(VkPhysicalDevice physicalDevice)
{
    uint32_t count = 0;
    ::vkGetPhysicalDeviceQueueFamilyProperties2KHR(physicalDevice, &count, nullptr);
    VulkanResultWithValue<std::vector<VkQueueFamilyProperties2>> result;
    result.value.resize(count);
    ::vkGetPhysicalDeviceQueueFamilyProperties2KHR(physicalDevice, &count, result.value.data());
    return result;
}

void vkGetPhysicalDeviceMemoryProperties2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties)
{
    return ::vkGetPhysicalDeviceMemoryProperties2KHR(physicalDevice, pMemoryProperties);
}

VulkanResultWithValue<std::vector<VkSparseImageFormatProperties2>> vkGetPhysicalDeviceSparseImageFormatProperties2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo)
{
    uint32_t count = 0;
    ::vkGetPhysicalDeviceSparseImageFormatProperties2KHR(physicalDevice, pFormatInfo, &count, nullptr);
    VulkanResultWithValue<std::vector<VkSparseImageFormatProperties2>> result;
    result.value.resize(count);
    ::vkGetPhysicalDeviceSparseImageFormatProperties2KHR(physicalDevice, pFormatInfo, &count, result.value.data());
    return result;
}

void vkGetDeviceGroupPeerMemoryFeaturesKHR(uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures)
{
    return ::vkGetDeviceGroupPeerMemoryFeaturesKHR(device_, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
}

void vkTrimCommandPoolKHR(VkCommandPool commandPool, VkCommandPoolTrimFlags flags)
{
    return ::vkTrimCommandPoolKHR(device_, commandPool, flags);
}

VulkanResultWithValue<std::vector<VkPhysicalDeviceGroupProperties>> vkEnumeratePhysicalDeviceGroupsKHR(VkInstance instance)
{
    uint32_t count = 0;
    ::vkEnumeratePhysicalDeviceGroupsKHR(instance, &count, nullptr);
    VulkanResultWithValue<std::vector<VkPhysicalDeviceGroupProperties>> result;
    result.value.resize(count);
    result.result = ::vkEnumeratePhysicalDeviceGroupsKHR(instance, &count, result.value.data());
    return result;
}

void vkGetPhysicalDeviceExternalBufferPropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties)
{
    return ::vkGetPhysicalDeviceExternalBufferPropertiesKHR(physicalDevice, pExternalBufferInfo, pExternalBufferProperties);
}

VulkanResult vkGetMemoryFdKHR(const VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd)
{
    return ::vkGetMemoryFdKHR(device_, pGetFdInfo, pFd);
}

VulkanResult vkGetMemoryFdPropertiesKHR(VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties)
{
    return ::vkGetMemoryFdPropertiesKHR(device_, handleType, fd, pMemoryFdProperties);
}

void vkGetPhysicalDeviceExternalSemaphorePropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties)
{
    return ::vkGetPhysicalDeviceExternalSemaphorePropertiesKHR(physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
}

VulkanResult vkImportSemaphoreFdKHR(const VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo)
{
    return ::vkImportSemaphoreFdKHR(device_, pImportSemaphoreFdInfo);
}

VulkanResult vkGetSemaphoreFdKHR(const VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd)
{
    return ::vkGetSemaphoreFdKHR(device_, pGetFdInfo, pFd);
}

VulkanResult vkCreateDescriptorUpdateTemplateKHR(const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate)
{
    return ::vkCreateDescriptorUpdateTemplateKHR(device_, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
}

void vkDestroyDescriptorUpdateTemplateKHR(VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyDescriptorUpdateTemplateKHR(device_, descriptorUpdateTemplate, pAllocator);
}

void vkUpdateDescriptorSetWithTemplateKHR(VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData)
{
    return ::vkUpdateDescriptorSetWithTemplateKHR(device_, descriptorSet, descriptorUpdateTemplate, pData);
}

VulkanResult vkCreateRenderPass2KHR(const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass)
{
    return ::vkCreateRenderPass2KHR(device_, pCreateInfo, pAllocator, pRenderPass);
}

VulkanResult vkGetSwapchainStatusKHR(VkSwapchainKHR swapchain)
{
    return ::vkGetSwapchainStatusKHR(device_, swapchain);
}

void vkGetPhysicalDeviceExternalFencePropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties)
{
    return ::vkGetPhysicalDeviceExternalFencePropertiesKHR(physicalDevice, pExternalFenceInfo, pExternalFenceProperties);
}

VulkanResult vkImportFenceFdKHR(const VkImportFenceFdInfoKHR* pImportFenceFdInfo)
{
    return ::vkImportFenceFdKHR(device_, pImportFenceFdInfo);
}

VulkanResult vkGetFenceFdKHR(const VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd)
{
    return ::vkGetFenceFdKHR(device_, pGetFdInfo, pFd);
}

VulkanResult vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, uint32_t* pCounterCount, VkPerformanceCounterKHR* pCounters, VkPerformanceCounterDescriptionKHR* pCounterDescriptions)
{
    return ::vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(physicalDevice, queueFamilyIndex, pCounterCount, pCounters, pCounterDescriptions);
}

void vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(VkPhysicalDevice physicalDevice, const VkQueryPoolPerformanceCreateInfoKHR* pPerformanceQueryCreateInfo, uint32_t* pNumPasses)
{
    return ::vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(physicalDevice, pPerformanceQueryCreateInfo, pNumPasses);
}

VulkanResult vkAcquireProfilingLockKHR(const VkAcquireProfilingLockInfoKHR* pInfo)
{
    return ::vkAcquireProfilingLockKHR(device_, pInfo);
}

void vkReleaseProfilingLockKHR()
{
    return ::vkReleaseProfilingLockKHR(device_);
}

VulkanResult vkGetPhysicalDeviceSurfaceCapabilities2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkSurfaceCapabilities2KHR* pSurfaceCapabilities)
{
    return ::vkGetPhysicalDeviceSurfaceCapabilities2KHR(physicalDevice, pSurfaceInfo, pSurfaceCapabilities);
}

VulkanResultWithValue<std::vector<VkSurfaceFormat2KHR>> vkGetPhysicalDeviceSurfaceFormats2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo)
{
    uint32_t count = 0;
    ::vkGetPhysicalDeviceSurfaceFormats2KHR(physicalDevice, pSurfaceInfo, &count, nullptr);
    VulkanResultWithValue<std::vector<VkSurfaceFormat2KHR>> result;
    result.value.resize(count);
    result.result = ::vkGetPhysicalDeviceSurfaceFormats2KHR(physicalDevice, pSurfaceInfo, &count, result.value.data());
    return result;
}

VulkanResultWithValue<std::vector<VkDisplayProperties2KHR>> vkGetPhysicalDeviceDisplayProperties2KHR(VkPhysicalDevice physicalDevice)
{
    uint32_t count = 0;
    ::vkGetPhysicalDeviceDisplayProperties2KHR(physicalDevice, &count, nullptr);
    VulkanResultWithValue<std::vector<VkDisplayProperties2KHR>> result;
    result.value.resize(count);
    result.result = ::vkGetPhysicalDeviceDisplayProperties2KHR(physicalDevice, &count, result.value.data());
    return result;
}

VulkanResultWithValue<std::vector<VkDisplayPlaneProperties2KHR>> vkGetPhysicalDeviceDisplayPlaneProperties2KHR(VkPhysicalDevice physicalDevice)
{
    uint32_t count = 0;
    ::vkGetPhysicalDeviceDisplayPlaneProperties2KHR(physicalDevice, &count, nullptr);
    VulkanResultWithValue<std::vector<VkDisplayPlaneProperties2KHR>> result;
    result.value.resize(count);
    result.result = ::vkGetPhysicalDeviceDisplayPlaneProperties2KHR(physicalDevice, &count, result.value.data());
    return result;
}

VulkanResultWithValue<std::vector<VkDisplayModeProperties2KHR>> vkGetDisplayModeProperties2KHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display)
{
    uint32_t count = 0;
    ::vkGetDisplayModeProperties2KHR(physicalDevice, display, &count, nullptr);
    VulkanResultWithValue<std::vector<VkDisplayModeProperties2KHR>> result;
    result.value.resize(count);
    result.result = ::vkGetDisplayModeProperties2KHR(physicalDevice, display, &count, result.value.data());
    return result;
}

VulkanResult vkGetDisplayPlaneCapabilities2KHR(VkPhysicalDevice physicalDevice, const VkDisplayPlaneInfo2KHR* pDisplayPlaneInfo, VkDisplayPlaneCapabilities2KHR* pCapabilities)
{
    return ::vkGetDisplayPlaneCapabilities2KHR(physicalDevice, pDisplayPlaneInfo, pCapabilities);
}

void vkGetImageMemoryRequirements2KHR(const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
    return ::vkGetImageMemoryRequirements2KHR(device_, pInfo, pMemoryRequirements);
}

void vkGetBufferMemoryRequirements2KHR(const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
    return ::vkGetBufferMemoryRequirements2KHR(device_, pInfo, pMemoryRequirements);
}

void vkGetImageSparseMemoryRequirements2KHR(const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements)
{
    return ::vkGetImageSparseMemoryRequirements2KHR(device_, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}

VulkanResult vkCreateSamplerYcbcrConversionKHR(const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion)
{
    return ::vkCreateSamplerYcbcrConversionKHR(device_, pCreateInfo, pAllocator, pYcbcrConversion);
}

void vkDestroySamplerYcbcrConversionKHR(VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroySamplerYcbcrConversionKHR(device_, ycbcrConversion, pAllocator);
}

VulkanResult vkBindBufferMemory2KHR(uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos)
{
    return ::vkBindBufferMemory2KHR(device_, bindInfoCount, pBindInfos);
}

VulkanResult vkBindImageMemory2KHR(uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos)
{
    return ::vkBindImageMemory2KHR(device_, bindInfoCount, pBindInfos);
}

void vkGetDescriptorSetLayoutSupportKHR(const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport)
{
    return ::vkGetDescriptorSetLayoutSupportKHR(device_, pCreateInfo, pSupport);
}

VulkanResult vkGetSemaphoreCounterValueKHR(VkSemaphore semaphore, uint64_t* pValue)
{
    return ::vkGetSemaphoreCounterValueKHR(device_, semaphore, pValue);
}

VulkanResult vkWaitSemaphoresKHR(const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout)
{
    return ::vkWaitSemaphoresKHR(device_, pWaitInfo, timeout);
}

VulkanResult vkSignalSemaphoreKHR(const VkSemaphoreSignalInfo* pSignalInfo)
{
    return ::vkSignalSemaphoreKHR(device_, pSignalInfo);
}

VulkanResultWithValue<std::vector<VkPhysicalDeviceFragmentShadingRateKHR>> vkGetPhysicalDeviceFragmentShadingRatesKHR(VkPhysicalDevice physicalDevice)
{
    uint32_t count = 0;
    ::vkGetPhysicalDeviceFragmentShadingRatesKHR(physicalDevice, &count, nullptr);
    VulkanResultWithValue<std::vector<VkPhysicalDeviceFragmentShadingRateKHR>> result;
    result.value.resize(count);
    result.result = ::vkGetPhysicalDeviceFragmentShadingRatesKHR(physicalDevice, &count, result.value.data());
    return result;
}

VulkanResult vkWaitForPresentKHR(VkSwapchainKHR swapchain, uint64_t presentId, uint64_t timeout)
{
    return ::vkWaitForPresentKHR(device_, swapchain, presentId, timeout);
}

VulkanResultWithValue<VkDeviceAddress> vkGetBufferDeviceAddressKHR(const VkBufferDeviceAddressInfo* pInfo)
{
    return ::vkGetBufferDeviceAddressKHR(device_, pInfo);
}

VulkanResultWithValue<uint64_t> vkGetBufferOpaqueCaptureAddressKHR(const VkBufferDeviceAddressInfo* pInfo)
{
    return ::vkGetBufferOpaqueCaptureAddressKHR(device_, pInfo);
}

VulkanResultWithValue<uint64_t> vkGetDeviceMemoryOpaqueCaptureAddressKHR(const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo)
{
    return ::vkGetDeviceMemoryOpaqueCaptureAddressKHR(device_, pInfo);
}

VulkanResult vkCreateDeferredOperationKHR(const VkAllocationCallbacks* pAllocator, VkDeferredOperationKHR* pDeferredOperation)
{
    return ::vkCreateDeferredOperationKHR(device_, pAllocator, pDeferredOperation);
}

void vkDestroyDeferredOperationKHR(VkDeferredOperationKHR operation, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyDeferredOperationKHR(device_, operation, pAllocator);
}

VulkanResultWithValue<uint32_t> vkGetDeferredOperationMaxConcurrencyKHR(VkDeferredOperationKHR operation)
{
    return ::vkGetDeferredOperationMaxConcurrencyKHR(device_, operation);
}

VulkanResult vkGetDeferredOperationResultKHR(VkDeferredOperationKHR operation)
{
    return ::vkGetDeferredOperationResultKHR(device_, operation);
}

VulkanResult vkDeferredOperationJoinKHR(VkDeferredOperationKHR operation)
{
    return ::vkDeferredOperationJoinKHR(device_, operation);
}

VulkanResult vkGetPipelineExecutablePropertiesKHR(const VkPipelineInfoKHR* pPipelineInfo, uint32_t* pExecutableCount, VkPipelineExecutablePropertiesKHR* pProperties)
{
    return ::vkGetPipelineExecutablePropertiesKHR(device_, pPipelineInfo, pExecutableCount, pProperties);
}

VulkanResult vkGetPipelineExecutableStatisticsKHR(const VkPipelineExecutableInfoKHR* pExecutableInfo, uint32_t* pStatisticCount, VkPipelineExecutableStatisticKHR* pStatistics)
{
    return ::vkGetPipelineExecutableStatisticsKHR(device_, pExecutableInfo, pStatisticCount, pStatistics);
}

VulkanResult vkGetPipelineExecutableInternalRepresentationsKHR(const VkPipelineExecutableInfoKHR* pExecutableInfo, uint32_t* pInternalRepresentationCount, VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations)
{
    return ::vkGetPipelineExecutableInternalRepresentationsKHR(device_, pExecutableInfo, pInternalRepresentationCount, pInternalRepresentations);
}

VulkanResult vkQueueSubmit2KHR(VkQueue queue, uint32_t submitCount, const VkSubmitInfo2* pSubmits, VkFence fence)
{
    return ::vkQueueSubmit2KHR(queue, submitCount, pSubmits, fence);
}

VulkanResultWithValue<std::vector<VkCheckpointData2NV>> vkGetQueueCheckpointData2NV(VkQueue queue)
{
    uint32_t count = 0;
    ::vkGetQueueCheckpointData2NV(queue, &count, nullptr);
    VulkanResultWithValue<std::vector<VkCheckpointData2NV>> result;
    result.value.resize(count);
    ::vkGetQueueCheckpointData2NV(queue, &count, result.value.data());
    return result;
}

void vkGetDeviceBufferMemoryRequirementsKHR(const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
    return ::vkGetDeviceBufferMemoryRequirementsKHR(device_, pInfo, pMemoryRequirements);
}

void vkGetDeviceImageMemoryRequirementsKHR(const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
    return ::vkGetDeviceImageMemoryRequirementsKHR(device_, pInfo, pMemoryRequirements);
}

void vkGetDeviceImageSparseMemoryRequirementsKHR(const VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements)
{
    return ::vkGetDeviceImageSparseMemoryRequirementsKHR(device_, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}

VulkanResult vkCreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback)
{
    return ::vkCreateDebugReportCallbackEXT(instance, pCreateInfo, pAllocator, pCallback);
}

void vkDestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyDebugReportCallbackEXT(instance, callback, pAllocator);
}

void vkDebugReportMessageEXT(VkInstance instance, VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage)
{
    return ::vkDebugReportMessageEXT(instance, flags, objectType, object, location, messageCode, pLayerPrefix, pMessage);
}

VulkanResult vkDebugMarkerSetObjectTagEXT(const VkDebugMarkerObjectTagInfoEXT* pTagInfo)
{
    return ::vkDebugMarkerSetObjectTagEXT(device_, pTagInfo);
}

VulkanResult vkDebugMarkerSetObjectNameEXT(const VkDebugMarkerObjectNameInfoEXT* pNameInfo)
{
    return ::vkDebugMarkerSetObjectNameEXT(device_, pNameInfo);
}

VulkanResult vkCreateCuModuleNVX(const VkCuModuleCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuModuleNVX* pModule)
{
    return ::vkCreateCuModuleNVX(device_, pCreateInfo, pAllocator, pModule);
}

VulkanResult vkCreateCuFunctionNVX(const VkCuFunctionCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuFunctionNVX* pFunction)
{
    return ::vkCreateCuFunctionNVX(device_, pCreateInfo, pAllocator, pFunction);
}

void vkDestroyCuModuleNVX(VkCuModuleNVX module, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyCuModuleNVX(device_, module, pAllocator);
}

void vkDestroyCuFunctionNVX(VkCuFunctionNVX function, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyCuFunctionNVX(device_, function, pAllocator);
}

VulkanResultWithValue<uint32_t> vkGetImageViewHandleNVX(const VkImageViewHandleInfoNVX* pInfo)
{
    return ::vkGetImageViewHandleNVX(device_, pInfo);
}

VulkanResult vkGetImageViewAddressNVX(VkImageView imageView, VkImageViewAddressPropertiesNVX* pProperties)
{
    return ::vkGetImageViewAddressNVX(device_, imageView, pProperties);
}

VulkanResult vkGetShaderInfoAMD(VkPipeline pipeline, VkShaderStageFlagBits shaderStage, VkShaderInfoTypeAMD infoType, size_t* pInfoSize, void* pInfo)
{
    return ::vkGetShaderInfoAMD(device_, pipeline, shaderStage, infoType, pInfoSize, pInfo);
}

VulkanResult vkGetPhysicalDeviceExternalImageFormatPropertiesNV(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkExternalMemoryHandleTypeFlagsNV externalHandleType, VkExternalImageFormatPropertiesNV* pExternalImageFormatProperties)
{
    return ::vkGetPhysicalDeviceExternalImageFormatPropertiesNV(physicalDevice, format, type, tiling, usage, flags, externalHandleType, pExternalImageFormatProperties);
}

VulkanResult vkReleaseDisplayEXT(VkPhysicalDevice physicalDevice, VkDisplayKHR display)
{
    return ::vkReleaseDisplayEXT(physicalDevice, display);
}

VulkanResult vkGetPhysicalDeviceSurfaceCapabilities2EXT(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilities2EXT* pSurfaceCapabilities)
{
    return ::vkGetPhysicalDeviceSurfaceCapabilities2EXT(physicalDevice, surface, pSurfaceCapabilities);
}

VulkanResult vkDisplayPowerControlEXT(VkDisplayKHR display, const VkDisplayPowerInfoEXT* pDisplayPowerInfo)
{
    return ::vkDisplayPowerControlEXT(device_, display, pDisplayPowerInfo);
}

VulkanResult vkRegisterDeviceEventEXT(const VkDeviceEventInfoEXT* pDeviceEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence)
{
    return ::vkRegisterDeviceEventEXT(device_, pDeviceEventInfo, pAllocator, pFence);
}

VulkanResult vkRegisterDisplayEventEXT(VkDisplayKHR display, const VkDisplayEventInfoEXT* pDisplayEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence)
{
    return ::vkRegisterDisplayEventEXT(device_, display, pDisplayEventInfo, pAllocator, pFence);
}

VulkanResult vkGetSwapchainCounterEXT(VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue)
{
    return ::vkGetSwapchainCounterEXT(device_, swapchain, counter, pCounterValue);
}

VulkanResult vkGetRefreshCycleDurationGOOGLE(VkSwapchainKHR swapchain, VkRefreshCycleDurationGOOGLE* pDisplayTimingProperties)
{
    return ::vkGetRefreshCycleDurationGOOGLE(device_, swapchain, pDisplayTimingProperties);
}

VulkanResult vkGetPastPresentationTimingGOOGLE(VkSwapchainKHR swapchain, uint32_t* pPresentationTimingCount, VkPastPresentationTimingGOOGLE* pPresentationTimings)
{
    return ::vkGetPastPresentationTimingGOOGLE(device_, swapchain, pPresentationTimingCount, pPresentationTimings);
}

void vkSetHdrMetadataEXT(uint32_t swapchainCount, const VkSwapchainKHR* pSwapchains, const VkHdrMetadataEXT* pMetadata)
{
    return ::vkSetHdrMetadataEXT(device_, swapchainCount, pSwapchains, pMetadata);
}

VulkanResult vkSetDebugUtilsObjectNameEXT(const VkDebugUtilsObjectNameInfoEXT* pNameInfo)
{
    return ::vkSetDebugUtilsObjectNameEXT(device_, pNameInfo);
}

VulkanResult vkSetDebugUtilsObjectTagEXT(const VkDebugUtilsObjectTagInfoEXT* pTagInfo)
{
    return ::vkSetDebugUtilsObjectTagEXT(device_, pTagInfo);
}

void vkQueueBeginDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo)
{
    return ::vkQueueBeginDebugUtilsLabelEXT(queue, pLabelInfo);
}

void vkQueueEndDebugUtilsLabelEXT(VkQueue queue)
{
    return ::vkQueueEndDebugUtilsLabelEXT(queue);
}

void vkQueueInsertDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo)
{
    return ::vkQueueInsertDebugUtilsLabelEXT(queue, pLabelInfo);
}

VulkanResult vkCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger)
{
    return ::vkCreateDebugUtilsMessengerEXT(instance, pCreateInfo, pAllocator, pMessenger);
}

void vkDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyDebugUtilsMessengerEXT(instance, messenger, pAllocator);
}

void vkSubmitDebugUtilsMessageEXT(VkInstance instance, VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData)
{
    return ::vkSubmitDebugUtilsMessageEXT(instance, messageSeverity, messageTypes, pCallbackData);
}

void vkGetPhysicalDeviceMultisamplePropertiesEXT(VkPhysicalDevice physicalDevice, VkSampleCountFlagBits samples, VkMultisamplePropertiesEXT* pMultisampleProperties)
{
    return ::vkGetPhysicalDeviceMultisamplePropertiesEXT(physicalDevice, samples, pMultisampleProperties);
}

VulkanResult vkGetImageDrmFormatModifierPropertiesEXT(VkImage image, VkImageDrmFormatModifierPropertiesEXT* pProperties)
{
    return ::vkGetImageDrmFormatModifierPropertiesEXT(device_, image, pProperties);
}

VulkanResult vkCreateValidationCacheEXT(const VkValidationCacheCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkValidationCacheEXT* pValidationCache)
{
    return ::vkCreateValidationCacheEXT(device_, pCreateInfo, pAllocator, pValidationCache);
}

void vkDestroyValidationCacheEXT(VkValidationCacheEXT validationCache, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyValidationCacheEXT(device_, validationCache, pAllocator);
}

VulkanResult vkMergeValidationCachesEXT(VkValidationCacheEXT dstCache, uint32_t srcCacheCount, const VkValidationCacheEXT* pSrcCaches)
{
    return ::vkMergeValidationCachesEXT(device_, dstCache, srcCacheCount, pSrcCaches);
}

VulkanResult vkGetValidationCacheDataEXT(VkValidationCacheEXT validationCache, size_t* pDataSize, void* pData)
{
    return ::vkGetValidationCacheDataEXT(device_, validationCache, pDataSize, pData);
}

VulkanResult vkCreateAccelerationStructureNV(const VkAccelerationStructureCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkAccelerationStructureNV* pAccelerationStructure)
{
    return ::vkCreateAccelerationStructureNV(device_, pCreateInfo, pAllocator, pAccelerationStructure);
}

void vkDestroyAccelerationStructureNV(VkAccelerationStructureNV accelerationStructure, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyAccelerationStructureNV(device_, accelerationStructure, pAllocator);
}

void vkGetAccelerationStructureMemoryRequirementsNV(const VkAccelerationStructureMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2KHR* pMemoryRequirements)
{
    return ::vkGetAccelerationStructureMemoryRequirementsNV(device_, pInfo, pMemoryRequirements);
}

VulkanResult vkBindAccelerationStructureMemoryNV(uint32_t bindInfoCount, const VkBindAccelerationStructureMemoryInfoNV* pBindInfos)
{
    return ::vkBindAccelerationStructureMemoryNV(device_, bindInfoCount, pBindInfos);
}

VulkanResult vkCreateRayTracingPipelinesNV(VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoNV* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
{
    return ::vkCreateRayTracingPipelinesNV(device_, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}

VulkanResult vkGetRayTracingShaderGroupHandlesKHR(VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData)
{
    return ::vkGetRayTracingShaderGroupHandlesKHR(device_, pipeline, firstGroup, groupCount, dataSize, pData);
}

VulkanResult vkGetRayTracingShaderGroupHandlesNV(VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData)
{
    return ::vkGetRayTracingShaderGroupHandlesNV(device_, pipeline, firstGroup, groupCount, dataSize, pData);
}

VulkanResult vkGetAccelerationStructureHandleNV(VkAccelerationStructureNV accelerationStructure, size_t dataSize, void* pData)
{
    return ::vkGetAccelerationStructureHandleNV(device_, accelerationStructure, dataSize, pData);
}

VulkanResult vkCompileDeferredNV(VkPipeline pipeline, uint32_t shader)
{
    return ::vkCompileDeferredNV(device_, pipeline, shader);
}

VulkanResult vkGetMemoryHostPointerPropertiesEXT(VkExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties)
{
    return ::vkGetMemoryHostPointerPropertiesEXT(device_, handleType, pHostPointer, pMemoryHostPointerProperties);
}

VulkanResultWithValue<std::vector<VkTimeDomainEXT>> vkGetPhysicalDeviceCalibrateableTimeDomainsEXT(VkPhysicalDevice physicalDevice)
{
    uint32_t count = 0;
    ::vkGetPhysicalDeviceCalibrateableTimeDomainsEXT(physicalDevice, &count, nullptr);
    VulkanResultWithValue<std::vector<VkTimeDomainEXT>> result;
    result.value.resize(count);
    result.result = ::vkGetPhysicalDeviceCalibrateableTimeDomainsEXT(physicalDevice, &count, result.value.data());
    return result;
}

VulkanResult vkGetCalibratedTimestampsEXT(uint32_t timestampCount, const VkCalibratedTimestampInfoEXT* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation)
{
    return ::vkGetCalibratedTimestampsEXT(device_, timestampCount, pTimestampInfos, pTimestamps, pMaxDeviation);
}

VulkanResultWithValue<std::vector<VkCheckpointDataNV>> vkGetQueueCheckpointDataNV(VkQueue queue)
{
    uint32_t count = 0;
    ::vkGetQueueCheckpointDataNV(queue, &count, nullptr);
    VulkanResultWithValue<std::vector<VkCheckpointDataNV>> result;
    result.value.resize(count);
    ::vkGetQueueCheckpointDataNV(queue, &count, result.value.data());
    return result;
}

VulkanResult vkInitializePerformanceApiINTEL(const VkInitializePerformanceApiInfoINTEL* pInitializeInfo)
{
    return ::vkInitializePerformanceApiINTEL(device_, pInitializeInfo);
}

void vkUninitializePerformanceApiINTEL()
{
    return ::vkUninitializePerformanceApiINTEL(device_);
}

VulkanResult vkAcquirePerformanceConfigurationINTEL(const VkPerformanceConfigurationAcquireInfoINTEL* pAcquireInfo, VkPerformanceConfigurationINTEL* pConfiguration)
{
    return ::vkAcquirePerformanceConfigurationINTEL(device_, pAcquireInfo, pConfiguration);
}

VulkanResult vkReleasePerformanceConfigurationINTEL(VkPerformanceConfigurationINTEL configuration)
{
    return ::vkReleasePerformanceConfigurationINTEL(device_, configuration);
}

VulkanResult vkQueueSetPerformanceConfigurationINTEL(VkQueue queue, VkPerformanceConfigurationINTEL configuration)
{
    return ::vkQueueSetPerformanceConfigurationINTEL(queue, configuration);
}

VulkanResult vkGetPerformanceParameterINTEL(VkPerformanceParameterTypeINTEL parameter, VkPerformanceValueINTEL* pValue)
{
    return ::vkGetPerformanceParameterINTEL(device_, parameter, pValue);
}

void vkSetLocalDimmingAMD(VkSwapchainKHR swapChain, VkBool32 localDimmingEnable)
{
    return ::vkSetLocalDimmingAMD(device_, swapChain, localDimmingEnable);
}

VulkanResultWithValue<VkDeviceAddress> vkGetBufferDeviceAddressEXT(const VkBufferDeviceAddressInfo* pInfo)
{
    return ::vkGetBufferDeviceAddressEXT(device_, pInfo);
}

VulkanResultWithValue<std::vector<VkPhysicalDeviceToolProperties>> vkGetPhysicalDeviceToolPropertiesEXT(VkPhysicalDevice physicalDevice)
{
    uint32_t count = 0;
    ::vkGetPhysicalDeviceToolPropertiesEXT(physicalDevice, &count, nullptr);
    VulkanResultWithValue<std::vector<VkPhysicalDeviceToolProperties>> result;
    result.value.resize(count);
    result.result = ::vkGetPhysicalDeviceToolPropertiesEXT(physicalDevice, &count, result.value.data());
    return result;
}

VulkanResultWithValue<std::vector<VkCooperativeMatrixPropertiesNV>> vkGetPhysicalDeviceCooperativeMatrixPropertiesNV(VkPhysicalDevice physicalDevice)
{
    uint32_t count = 0;
    ::vkGetPhysicalDeviceCooperativeMatrixPropertiesNV(physicalDevice, &count, nullptr);
    VulkanResultWithValue<std::vector<VkCooperativeMatrixPropertiesNV>> result;
    result.value.resize(count);
    result.result = ::vkGetPhysicalDeviceCooperativeMatrixPropertiesNV(physicalDevice, &count, result.value.data());
    return result;
}

VulkanResultWithValue<std::vector<VkFramebufferMixedSamplesCombinationNV>> vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(VkPhysicalDevice physicalDevice)
{
    uint32_t count = 0;
    ::vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(physicalDevice, &count, nullptr);
    VulkanResultWithValue<std::vector<VkFramebufferMixedSamplesCombinationNV>> result;
    result.value.resize(count);
    result.result = ::vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(physicalDevice, &count, result.value.data());
    return result;
}

VulkanResult vkCreateHeadlessSurfaceEXT(VkInstance instance, const VkHeadlessSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    return ::vkCreateHeadlessSurfaceEXT(instance, pCreateInfo, pAllocator, pSurface);
}

void vkResetQueryPoolEXT(VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount)
{
    return ::vkResetQueryPoolEXT(device_, queryPool, firstQuery, queryCount);
}

void vkGetGeneratedCommandsMemoryRequirementsNV(const VkGeneratedCommandsMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
    return ::vkGetGeneratedCommandsMemoryRequirementsNV(device_, pInfo, pMemoryRequirements);
}

VulkanResult vkCreateIndirectCommandsLayoutNV(const VkIndirectCommandsLayoutCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectCommandsLayoutNV* pIndirectCommandsLayout)
{
    return ::vkCreateIndirectCommandsLayoutNV(device_, pCreateInfo, pAllocator, pIndirectCommandsLayout);
}

void vkDestroyIndirectCommandsLayoutNV(VkIndirectCommandsLayoutNV indirectCommandsLayout, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyIndirectCommandsLayoutNV(device_, indirectCommandsLayout, pAllocator);
}

VulkanResult vkAcquireDrmDisplayEXT(VkPhysicalDevice physicalDevice, int32_t drmFd, VkDisplayKHR display)
{
    return ::vkAcquireDrmDisplayEXT(physicalDevice, drmFd, display);
}

VulkanResult vkGetDrmDisplayEXT(VkPhysicalDevice physicalDevice, int32_t drmFd, uint32_t connectorId, VkDisplayKHR* display)
{
    return ::vkGetDrmDisplayEXT(physicalDevice, drmFd, connectorId, display);
}

VulkanResult vkCreatePrivateDataSlotEXT(const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot)
{
    return ::vkCreatePrivateDataSlotEXT(device_, pCreateInfo, pAllocator, pPrivateDataSlot);
}

void vkDestroyPrivateDataSlotEXT(VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyPrivateDataSlotEXT(device_, privateDataSlot, pAllocator);
}

VulkanResult vkSetPrivateDataEXT(VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data)
{
    return ::vkSetPrivateDataEXT(device_, objectType, objectHandle, privateDataSlot, data);
}

void vkGetPrivateDataEXT(VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData)
{
    return ::vkGetPrivateDataEXT(device_, objectType, objectHandle, privateDataSlot, pData);
}

VulkanResult vkAcquireWinrtDisplayNV(VkPhysicalDevice physicalDevice, VkDisplayKHR display)
{
    return ::vkAcquireWinrtDisplayNV(physicalDevice, display);
}

VulkanResult vkGetWinrtDisplayNV(VkPhysicalDevice physicalDevice, uint32_t deviceRelativeId, VkDisplayKHR* pDisplay)
{
    return ::vkGetWinrtDisplayNV(physicalDevice, deviceRelativeId, pDisplay);
}

VulkanResult vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI(VkRenderPass renderpass, VkExtent2D* pMaxWorkgroupSize)
{
    return ::vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI(device_, renderpass, pMaxWorkgroupSize);
}

VulkanResult vkGetMemoryRemoteAddressNV(const VkMemoryGetRemoteAddressInfoNV* pMemoryGetRemoteAddressInfo, VkRemoteAddressNV* pAddress)
{
    return ::vkGetMemoryRemoteAddressNV(device_, pMemoryGetRemoteAddressInfo, pAddress);
}

void vkSetDeviceMemoryPriorityEXT(VkDeviceMemory memory, float priority)
{
    return ::vkSetDeviceMemoryPriorityEXT(device_, memory, priority);
}

void vkGetDescriptorSetLayoutHostMappingInfoVALVE(const VkDescriptorSetBindingReferenceVALVE* pBindingReference, VkDescriptorSetLayoutHostMappingInfoVALVE* pHostMapping)
{
    return ::vkGetDescriptorSetLayoutHostMappingInfoVALVE(device_, pBindingReference, pHostMapping);
}

void vkGetDescriptorSetHostMappingVALVE(VkDescriptorSet descriptorSet, void** ppData)
{
    return ::vkGetDescriptorSetHostMappingVALVE(device_, descriptorSet, ppData);
}

VulkanResult vkCreateAccelerationStructureKHR(const VkAccelerationStructureCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkAccelerationStructureKHR* pAccelerationStructure)
{
    return ::vkCreateAccelerationStructureKHR(device_, pCreateInfo, pAllocator, pAccelerationStructure);
}

void vkDestroyAccelerationStructureKHR(VkAccelerationStructureKHR accelerationStructure, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyAccelerationStructureKHR(device_, accelerationStructure, pAllocator);
}

VulkanResult vkBuildAccelerationStructuresKHR(VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos)
{
    return ::vkBuildAccelerationStructuresKHR(device_, deferredOperation, infoCount, pInfos, ppBuildRangeInfos);
}

VulkanResult vkCopyAccelerationStructureKHR(VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureInfoKHR* pInfo)
{
    return ::vkCopyAccelerationStructureKHR(device_, deferredOperation, pInfo);
}

VulkanResult vkCopyAccelerationStructureToMemoryKHR(VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo)
{
    return ::vkCopyAccelerationStructureToMemoryKHR(device_, deferredOperation, pInfo);
}

VulkanResult vkCopyMemoryToAccelerationStructureKHR(VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo)
{
    return ::vkCopyMemoryToAccelerationStructureKHR(device_, deferredOperation, pInfo);
}

VulkanResult vkWriteAccelerationStructuresPropertiesKHR(uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, size_t dataSize, void* pData, size_t stride)
{
    return ::vkWriteAccelerationStructuresPropertiesKHR(device_, accelerationStructureCount, pAccelerationStructures, queryType, dataSize, pData, stride);
}

VulkanResultWithValue<VkDeviceAddress> vkGetAccelerationStructureDeviceAddressKHR(const VkAccelerationStructureDeviceAddressInfoKHR* pInfo)
{
    return ::vkGetAccelerationStructureDeviceAddressKHR(device_, pInfo);
}

void vkGetDeviceAccelerationStructureCompatibilityKHR(const VkAccelerationStructureVersionInfoKHR* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility)
{
    return ::vkGetDeviceAccelerationStructureCompatibilityKHR(device_, pVersionInfo, pCompatibility);
}

void vkGetAccelerationStructureBuildSizesKHR(VkAccelerationStructureBuildTypeKHR buildType, const VkAccelerationStructureBuildGeometryInfoKHR* pBuildInfo, const uint32_t* pMaxPrimitiveCounts, VkAccelerationStructureBuildSizesInfoKHR* pSizeInfo)
{
    return ::vkGetAccelerationStructureBuildSizesKHR(device_, buildType, pBuildInfo, pMaxPrimitiveCounts, pSizeInfo);
}

VulkanResult vkCreateRayTracingPipelinesKHR(VkDeferredOperationKHR deferredOperation, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
{
    return ::vkCreateRayTracingPipelinesKHR(device_, deferredOperation, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}

VulkanResult vkGetRayTracingCaptureReplayShaderGroupHandlesKHR(VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData)
{
    return ::vkGetRayTracingCaptureReplayShaderGroupHandlesKHR(device_, pipeline, firstGroup, groupCount, dataSize, pData);
}

VulkanResultWithValue<VkDeviceSize> vkGetRayTracingShaderGroupStackSizeKHR(VkPipeline pipeline, uint32_t group, VkShaderGroupShaderKHR groupShader)
{
    return ::vkGetRayTracingShaderGroupStackSizeKHR(device_, pipeline, group, groupShader);
}

VulkanCommandRecorder::VulkanCommandRecorder(VkCommandBuffer command_buffer) : _command_buffer(command_buffer) {}

VulkanResult  VulkanCommandRecorder::vkBeginCommandBuffer(const VkCommandBufferBeginInfo* pBeginInfo) const noexcept
{
    return ::vkBeginCommandBuffer(_command_buffer, pBeginInfo);
}

VulkanResult  VulkanCommandRecorder::vkEndCommandBuffer() const noexcept
{
    return ::vkEndCommandBuffer(_command_buffer);
}

VulkanResult  VulkanCommandRecorder::vkResetCommandBuffer(VkCommandBufferResetFlags flags) const noexcept
{
    return ::vkResetCommandBuffer(_command_buffer, flags);
}

void  VulkanCommandRecorder::vkCmdBindPipeline(VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline) const noexcept
{
    ::vkCmdBindPipeline(_command_buffer, pipelineBindPoint, pipeline);
}

void  VulkanCommandRecorder::vkCmdSetViewport(uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports) const noexcept
{
    ::vkCmdSetViewport(_command_buffer, firstViewport, viewportCount, pViewports);
}

void  VulkanCommandRecorder::vkCmdSetScissor(uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors) const noexcept
{
    ::vkCmdSetScissor(_command_buffer, firstScissor, scissorCount, pScissors);
}

void  VulkanCommandRecorder::vkCmdSetLineWidth(float lineWidth) const noexcept
{
    ::vkCmdSetLineWidth(_command_buffer, lineWidth);
}

void  VulkanCommandRecorder::vkCmdSetDepthBias(float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) const noexcept
{
    ::vkCmdSetDepthBias(_command_buffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
}

void  VulkanCommandRecorder::vkCmdSetBlendConstants(const float blendConstants[4]) const noexcept
{
    ::vkCmdSetBlendConstants(_command_buffer, blendConstants);
}

void  VulkanCommandRecorder::vkCmdSetDepthBounds(float minDepthBounds, float maxDepthBounds) const noexcept
{
    ::vkCmdSetDepthBounds(_command_buffer, minDepthBounds, maxDepthBounds);
}

void  VulkanCommandRecorder::vkCmdSetStencilCompareMask(VkStencilFaceFlags faceMask, uint32_t compareMask) const noexcept
{
    ::vkCmdSetStencilCompareMask(_command_buffer, faceMask, compareMask);
}

void  VulkanCommandRecorder::vkCmdSetStencilWriteMask(VkStencilFaceFlags faceMask, uint32_t writeMask) const noexcept
{
    ::vkCmdSetStencilWriteMask(_command_buffer, faceMask, writeMask);
}

void  VulkanCommandRecorder::vkCmdSetStencilReference(VkStencilFaceFlags faceMask, uint32_t reference) const noexcept
{
    ::vkCmdSetStencilReference(_command_buffer, faceMask, reference);
}

void  VulkanCommandRecorder::vkCmdBindDescriptorSets(VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets) const noexcept
{
    ::vkCmdBindDescriptorSets(_command_buffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
}

void  VulkanCommandRecorder::vkCmdBindIndexBuffer(VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) const noexcept
{
    ::vkCmdBindIndexBuffer(_command_buffer, buffer, offset, indexType);
}

void  VulkanCommandRecorder::vkCmdBindVertexBuffers(uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets) const noexcept
{
    ::vkCmdBindVertexBuffers(_command_buffer, firstBinding, bindingCount, pBuffers, pOffsets);
}

void  VulkanCommandRecorder::vkCmdDraw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) const noexcept
{
    ::vkCmdDraw(_command_buffer, vertexCount, instanceCount, firstVertex, firstInstance);
}

void  VulkanCommandRecorder::vkCmdDrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) const noexcept
{
    ::vkCmdDrawIndexed(_command_buffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

void  VulkanCommandRecorder::vkCmdDrawIndirect(VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) const noexcept
{
    ::vkCmdDrawIndirect(_command_buffer, buffer, offset, drawCount, stride);
}

void  VulkanCommandRecorder::vkCmdDrawIndexedIndirect(VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) const noexcept
{
    ::vkCmdDrawIndexedIndirect(_command_buffer, buffer, offset, drawCount, stride);
}

void  VulkanCommandRecorder::vkCmdDispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const noexcept
{
    ::vkCmdDispatch(_command_buffer, groupCountX, groupCountY, groupCountZ);
}

void  VulkanCommandRecorder::vkCmdDispatchIndirect(VkBuffer buffer, VkDeviceSize offset) const noexcept
{
    ::vkCmdDispatchIndirect(_command_buffer, buffer, offset);
}

void  VulkanCommandRecorder::vkCmdCopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions) const noexcept
{
    ::vkCmdCopyBuffer(_command_buffer, srcBuffer, dstBuffer, regionCount, pRegions);
}

void  VulkanCommandRecorder::vkCmdCopyImage(VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions) const noexcept
{
    ::vkCmdCopyImage(_command_buffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}

void  VulkanCommandRecorder::vkCmdBlitImage(VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter) const noexcept
{
    ::vkCmdBlitImage(_command_buffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
}

void  VulkanCommandRecorder::vkCmdCopyBufferToImage(VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions) const noexcept
{
    ::vkCmdCopyBufferToImage(_command_buffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
}

void  VulkanCommandRecorder::vkCmdCopyImageToBuffer(VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions) const noexcept
{
    ::vkCmdCopyImageToBuffer(_command_buffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
}

void  VulkanCommandRecorder::vkCmdUpdateBuffer(VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData) const noexcept
{
    ::vkCmdUpdateBuffer(_command_buffer, dstBuffer, dstOffset, dataSize, pData);
}

void  VulkanCommandRecorder::vkCmdFillBuffer(VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data) const noexcept
{
    ::vkCmdFillBuffer(_command_buffer, dstBuffer, dstOffset, size, data);
}

void  VulkanCommandRecorder::vkCmdClearColorImage(VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) const noexcept
{
    ::vkCmdClearColorImage(_command_buffer, image, imageLayout, pColor, rangeCount, pRanges);
}

void  VulkanCommandRecorder::vkCmdClearDepthStencilImage(VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) const noexcept
{
    ::vkCmdClearDepthStencilImage(_command_buffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
}

void  VulkanCommandRecorder::vkCmdClearAttachments(uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects) const noexcept
{
    ::vkCmdClearAttachments(_command_buffer, attachmentCount, pAttachments, rectCount, pRects);
}

void  VulkanCommandRecorder::vkCmdResolveImage(VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions) const noexcept
{
    ::vkCmdResolveImage(_command_buffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}

void  VulkanCommandRecorder::vkCmdSetEvent(VkEvent event, VkPipelineStageFlags stageMask) const noexcept
{
    ::vkCmdSetEvent(_command_buffer, event, stageMask);
}

void  VulkanCommandRecorder::vkCmdResetEvent(VkEvent event, VkPipelineStageFlags stageMask) const noexcept
{
    ::vkCmdResetEvent(_command_buffer, event, stageMask);
}

void  VulkanCommandRecorder::vkCmdWaitEvents(uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) const noexcept
{
    ::vkCmdWaitEvents(_command_buffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}

void  VulkanCommandRecorder::vkCmdPipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) const noexcept
{
    ::vkCmdPipelineBarrier(_command_buffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}

void  VulkanCommandRecorder::vkCmdBeginQuery(VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags) const noexcept
{
    ::vkCmdBeginQuery(_command_buffer, queryPool, query, flags);
}

void  VulkanCommandRecorder::vkCmdEndQuery(VkQueryPool queryPool, uint32_t query) const noexcept
{
    ::vkCmdEndQuery(_command_buffer, queryPool, query);
}

void  VulkanCommandRecorder::vkCmdResetQueryPool(VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) const noexcept
{
    ::vkCmdResetQueryPool(_command_buffer, queryPool, firstQuery, queryCount);
}

void  VulkanCommandRecorder::vkCmdWriteTimestamp(VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query) const noexcept
{
    ::vkCmdWriteTimestamp(_command_buffer, pipelineStage, queryPool, query);
}

void  VulkanCommandRecorder::vkCmdCopyQueryPoolResults(VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags) const noexcept
{
    ::vkCmdCopyQueryPoolResults(_command_buffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
}

void  VulkanCommandRecorder::vkCmdPushConstants(VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues) const noexcept
{
    ::vkCmdPushConstants(_command_buffer, layout, stageFlags, offset, size, pValues);
}

void  VulkanCommandRecorder::vkCmdBeginRenderPass(const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents) const noexcept
{
    ::vkCmdBeginRenderPass(_command_buffer, pRenderPassBegin, contents);
}

void  VulkanCommandRecorder::vkCmdNextSubpass(VkSubpassContents contents) const noexcept
{
    ::vkCmdNextSubpass(_command_buffer, contents);
}

void  VulkanCommandRecorder::vkCmdEndRenderPass() const noexcept
{
    ::vkCmdEndRenderPass(_command_buffer);
}

void  VulkanCommandRecorder::vkCmdExecuteCommands(uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) const noexcept
{
    ::vkCmdExecuteCommands(_command_buffer, commandBufferCount, pCommandBuffers);
}

void  VulkanCommandRecorder::vkCmdSetDeviceMask(uint32_t deviceMask) const noexcept
{
    ::vkCmdSetDeviceMask(_command_buffer, deviceMask);
}

void  VulkanCommandRecorder::vkCmdDispatchBase(uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const noexcept
{
    ::vkCmdDispatchBase(_command_buffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
}

void  VulkanCommandRecorder::vkCmdDrawIndirectCount(VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const noexcept
{
    ::vkCmdDrawIndirectCount(_command_buffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void  VulkanCommandRecorder::vkCmdDrawIndexedIndirectCount(VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const noexcept
{
    ::vkCmdDrawIndexedIndirectCount(_command_buffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void  VulkanCommandRecorder::vkCmdBeginRenderPass2(const VkRenderPassBeginInfo* pRenderPassBegin, const VkSubpassBeginInfo* pSubpassBeginInfo) const noexcept
{
    ::vkCmdBeginRenderPass2(_command_buffer, pRenderPassBegin, pSubpassBeginInfo);
}

void  VulkanCommandRecorder::vkCmdNextSubpass2(const VkSubpassBeginInfo* pSubpassBeginInfo, const VkSubpassEndInfo* pSubpassEndInfo) const noexcept
{
    ::vkCmdNextSubpass2(_command_buffer, pSubpassBeginInfo, pSubpassEndInfo);
}

void  VulkanCommandRecorder::vkCmdEndRenderPass2(const VkSubpassEndInfo* pSubpassEndInfo) const noexcept
{
    ::vkCmdEndRenderPass2(_command_buffer, pSubpassEndInfo);
}

void  VulkanCommandRecorder::vkCmdSetEvent2(VkEvent event, const VkDependencyInfo* pDependencyInfo) const noexcept
{
    ::vkCmdSetEvent2(_command_buffer, event, pDependencyInfo);
}

void  VulkanCommandRecorder::vkCmdResetEvent2(VkEvent event, VkPipelineStageFlags2 stageMask) const noexcept
{
    ::vkCmdResetEvent2(_command_buffer, event, stageMask);
}

void  VulkanCommandRecorder::vkCmdWaitEvents2(uint32_t eventCount, const VkEvent* pEvents, const VkDependencyInfo* pDependencyInfos) const noexcept
{
    ::vkCmdWaitEvents2(_command_buffer, eventCount, pEvents, pDependencyInfos);
}

void  VulkanCommandRecorder::vkCmdPipelineBarrier2(const VkDependencyInfo* pDependencyInfo) const noexcept
{
    ::vkCmdPipelineBarrier2(_command_buffer, pDependencyInfo);
}

void  VulkanCommandRecorder::vkCmdWriteTimestamp2(VkPipelineStageFlags2 stage, VkQueryPool queryPool, uint32_t query) const noexcept
{
    ::vkCmdWriteTimestamp2(_command_buffer, stage, queryPool, query);
}

void  VulkanCommandRecorder::vkCmdCopyBuffer2(const VkCopyBufferInfo2* pCopyBufferInfo) const noexcept
{
    ::vkCmdCopyBuffer2(_command_buffer, pCopyBufferInfo);
}

void  VulkanCommandRecorder::vkCmdCopyImage2(const VkCopyImageInfo2* pCopyImageInfo) const noexcept
{
    ::vkCmdCopyImage2(_command_buffer, pCopyImageInfo);
}

void  VulkanCommandRecorder::vkCmdCopyBufferToImage2(const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo) const noexcept
{
    ::vkCmdCopyBufferToImage2(_command_buffer, pCopyBufferToImageInfo);
}

void  VulkanCommandRecorder::vkCmdCopyImageToBuffer2(const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo) const noexcept
{
    ::vkCmdCopyImageToBuffer2(_command_buffer, pCopyImageToBufferInfo);
}

void  VulkanCommandRecorder::vkCmdBlitImage2(const VkBlitImageInfo2* pBlitImageInfo) const noexcept
{
    ::vkCmdBlitImage2(_command_buffer, pBlitImageInfo);
}

void  VulkanCommandRecorder::vkCmdResolveImage2(const VkResolveImageInfo2* pResolveImageInfo) const noexcept
{
    ::vkCmdResolveImage2(_command_buffer, pResolveImageInfo);
}

void  VulkanCommandRecorder::vkCmdBeginRendering(const VkRenderingInfo* pRenderingInfo) const noexcept
{
    ::vkCmdBeginRendering(_command_buffer, pRenderingInfo);
}

void  VulkanCommandRecorder::vkCmdEndRendering() const noexcept
{
    ::vkCmdEndRendering(_command_buffer);
}

void  VulkanCommandRecorder::vkCmdSetCullMode(VkCullModeFlags cullMode) const noexcept
{
    ::vkCmdSetCullMode(_command_buffer, cullMode);
}

void  VulkanCommandRecorder::vkCmdSetFrontFace(VkFrontFace frontFace) const noexcept
{
    ::vkCmdSetFrontFace(_command_buffer, frontFace);
}

void  VulkanCommandRecorder::vkCmdSetPrimitiveTopology(VkPrimitiveTopology primitiveTopology) const noexcept
{
    ::vkCmdSetPrimitiveTopology(_command_buffer, primitiveTopology);
}

void  VulkanCommandRecorder::vkCmdSetViewportWithCount(uint32_t viewportCount, const VkViewport* pViewports) const noexcept
{
    ::vkCmdSetViewportWithCount(_command_buffer, viewportCount, pViewports);
}

void  VulkanCommandRecorder::vkCmdSetScissorWithCount(uint32_t scissorCount, const VkRect2D* pScissors) const noexcept
{
    ::vkCmdSetScissorWithCount(_command_buffer, scissorCount, pScissors);
}

void  VulkanCommandRecorder::vkCmdBindVertexBuffers2(uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides) const noexcept
{
    ::vkCmdBindVertexBuffers2(_command_buffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes, pStrides);
}

void  VulkanCommandRecorder::vkCmdSetDepthTestEnable(VkBool32 depthTestEnable) const noexcept
{
    ::vkCmdSetDepthTestEnable(_command_buffer, depthTestEnable);
}

void  VulkanCommandRecorder::vkCmdSetDepthWriteEnable(VkBool32 depthWriteEnable) const noexcept
{
    ::vkCmdSetDepthWriteEnable(_command_buffer, depthWriteEnable);
}

void  VulkanCommandRecorder::vkCmdSetDepthCompareOp(VkCompareOp depthCompareOp) const noexcept
{
    ::vkCmdSetDepthCompareOp(_command_buffer, depthCompareOp);
}

void  VulkanCommandRecorder::vkCmdSetDepthBoundsTestEnable(VkBool32 depthBoundsTestEnable) const noexcept
{
    ::vkCmdSetDepthBoundsTestEnable(_command_buffer, depthBoundsTestEnable);
}

void  VulkanCommandRecorder::vkCmdSetStencilTestEnable(VkBool32 stencilTestEnable) const noexcept
{
    ::vkCmdSetStencilTestEnable(_command_buffer, stencilTestEnable);
}

void  VulkanCommandRecorder::vkCmdSetStencilOp(VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp) const noexcept
{
    ::vkCmdSetStencilOp(_command_buffer, faceMask, failOp, passOp, depthFailOp, compareOp);
}

void  VulkanCommandRecorder::vkCmdSetRasterizerDiscardEnable(VkBool32 rasterizerDiscardEnable) const noexcept
{
    ::vkCmdSetRasterizerDiscardEnable(_command_buffer, rasterizerDiscardEnable);
}

void  VulkanCommandRecorder::vkCmdSetDepthBiasEnable(VkBool32 depthBiasEnable) const noexcept
{
    ::vkCmdSetDepthBiasEnable(_command_buffer, depthBiasEnable);
}

void  VulkanCommandRecorder::vkCmdSetPrimitiveRestartEnable(VkBool32 primitiveRestartEnable) const noexcept
{
    ::vkCmdSetPrimitiveRestartEnable(_command_buffer, primitiveRestartEnable);
}

void  VulkanCommandRecorder::vkCmdBeginRenderingKHR(const VkRenderingInfo* pRenderingInfo) const noexcept
{
    ::vkCmdBeginRenderingKHR(_command_buffer, pRenderingInfo);
}

void  VulkanCommandRecorder::vkCmdEndRenderingKHR() const noexcept
{
    ::vkCmdEndRenderingKHR(_command_buffer);
}

void  VulkanCommandRecorder::vkCmdSetDeviceMaskKHR(uint32_t deviceMask) const noexcept
{
    ::vkCmdSetDeviceMaskKHR(_command_buffer, deviceMask);
}

void  VulkanCommandRecorder::vkCmdDispatchBaseKHR(uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const noexcept
{
    ::vkCmdDispatchBaseKHR(_command_buffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
}

void  VulkanCommandRecorder::vkCmdPushDescriptorSetKHR(VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites) const noexcept
{
    ::vkCmdPushDescriptorSetKHR(_command_buffer, pipelineBindPoint, layout, set, descriptorWriteCount, pDescriptorWrites);
}

void  VulkanCommandRecorder::vkCmdPushDescriptorSetWithTemplateKHR(VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData) const noexcept
{
    ::vkCmdPushDescriptorSetWithTemplateKHR(_command_buffer, descriptorUpdateTemplate, layout, set, pData);
}

void  VulkanCommandRecorder::vkCmdBeginRenderPass2KHR(const VkRenderPassBeginInfo* pRenderPassBegin, const VkSubpassBeginInfo* pSubpassBeginInfo) const noexcept
{
    ::vkCmdBeginRenderPass2KHR(_command_buffer, pRenderPassBegin, pSubpassBeginInfo);
}

void  VulkanCommandRecorder::vkCmdNextSubpass2KHR(const VkSubpassBeginInfo* pSubpassBeginInfo, const VkSubpassEndInfo* pSubpassEndInfo) const noexcept
{
    ::vkCmdNextSubpass2KHR(_command_buffer, pSubpassBeginInfo, pSubpassEndInfo);
}

void  VulkanCommandRecorder::vkCmdEndRenderPass2KHR(const VkSubpassEndInfo* pSubpassEndInfo) const noexcept
{
    ::vkCmdEndRenderPass2KHR(_command_buffer, pSubpassEndInfo);
}

void  VulkanCommandRecorder::vkCmdDrawIndirectCountKHR(VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const noexcept
{
    ::vkCmdDrawIndirectCountKHR(_command_buffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void  VulkanCommandRecorder::vkCmdDrawIndexedIndirectCountKHR(VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const noexcept
{
    ::vkCmdDrawIndexedIndirectCountKHR(_command_buffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void  VulkanCommandRecorder::vkCmdSetFragmentShadingRateKHR(const VkExtent2D* pFragmentSize, const VkFragmentShadingRateCombinerOpKHR combinerOps[2]) const noexcept
{
    ::vkCmdSetFragmentShadingRateKHR(_command_buffer, pFragmentSize, combinerOps);
}

void  VulkanCommandRecorder::vkCmdSetEvent2KHR(VkEvent event, const VkDependencyInfo* pDependencyInfo) const noexcept
{
    ::vkCmdSetEvent2KHR(_command_buffer, event, pDependencyInfo);
}

void  VulkanCommandRecorder::vkCmdResetEvent2KHR(VkEvent event, VkPipelineStageFlags2 stageMask) const noexcept
{
    ::vkCmdResetEvent2KHR(_command_buffer, event, stageMask);
}

void  VulkanCommandRecorder::vkCmdWaitEvents2KHR(uint32_t eventCount, const VkEvent* pEvents, const VkDependencyInfo* pDependencyInfos) const noexcept
{
    ::vkCmdWaitEvents2KHR(_command_buffer, eventCount, pEvents, pDependencyInfos);
}

void  VulkanCommandRecorder::vkCmdPipelineBarrier2KHR(const VkDependencyInfo* pDependencyInfo) const noexcept
{
    ::vkCmdPipelineBarrier2KHR(_command_buffer, pDependencyInfo);
}

void  VulkanCommandRecorder::vkCmdWriteTimestamp2KHR(VkPipelineStageFlags2 stage, VkQueryPool queryPool, uint32_t query) const noexcept
{
    ::vkCmdWriteTimestamp2KHR(_command_buffer, stage, queryPool, query);
}

void  VulkanCommandRecorder::vkCmdWriteBufferMarker2AMD(VkPipelineStageFlags2 stage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker) const noexcept
{
    ::vkCmdWriteBufferMarker2AMD(_command_buffer, stage, dstBuffer, dstOffset, marker);
}

void  VulkanCommandRecorder::vkCmdCopyBuffer2KHR(const VkCopyBufferInfo2* pCopyBufferInfo) const noexcept
{
    ::vkCmdCopyBuffer2KHR(_command_buffer, pCopyBufferInfo);
}

void  VulkanCommandRecorder::vkCmdCopyImage2KHR(const VkCopyImageInfo2* pCopyImageInfo) const noexcept
{
    ::vkCmdCopyImage2KHR(_command_buffer, pCopyImageInfo);
}

void  VulkanCommandRecorder::vkCmdCopyBufferToImage2KHR(const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo) const noexcept
{
    ::vkCmdCopyBufferToImage2KHR(_command_buffer, pCopyBufferToImageInfo);
}

void  VulkanCommandRecorder::vkCmdCopyImageToBuffer2KHR(const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo) const noexcept
{
    ::vkCmdCopyImageToBuffer2KHR(_command_buffer, pCopyImageToBufferInfo);
}

void  VulkanCommandRecorder::vkCmdBlitImage2KHR(const VkBlitImageInfo2* pBlitImageInfo) const noexcept
{
    ::vkCmdBlitImage2KHR(_command_buffer, pBlitImageInfo);
}

void  VulkanCommandRecorder::vkCmdResolveImage2KHR(const VkResolveImageInfo2* pResolveImageInfo) const noexcept
{
    ::vkCmdResolveImage2KHR(_command_buffer, pResolveImageInfo);
}

void  VulkanCommandRecorder::vkCmdDebugMarkerBeginEXT(const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) const noexcept
{
    ::vkCmdDebugMarkerBeginEXT(_command_buffer, pMarkerInfo);
}

void  VulkanCommandRecorder::vkCmdDebugMarkerEndEXT() const noexcept
{
    ::vkCmdDebugMarkerEndEXT(_command_buffer);
}

void  VulkanCommandRecorder::vkCmdDebugMarkerInsertEXT(const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) const noexcept
{
    ::vkCmdDebugMarkerInsertEXT(_command_buffer, pMarkerInfo);
}

void  VulkanCommandRecorder::vkCmdBindTransformFeedbackBuffersEXT(uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes) const noexcept
{
    ::vkCmdBindTransformFeedbackBuffersEXT(_command_buffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes);
}

void  VulkanCommandRecorder::vkCmdBeginTransformFeedbackEXT(uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) const noexcept
{
    ::vkCmdBeginTransformFeedbackEXT(_command_buffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
}

void  VulkanCommandRecorder::vkCmdEndTransformFeedbackEXT(uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) const noexcept
{
    ::vkCmdEndTransformFeedbackEXT(_command_buffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
}

void  VulkanCommandRecorder::vkCmdBeginQueryIndexedEXT(VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index) const noexcept
{
    ::vkCmdBeginQueryIndexedEXT(_command_buffer, queryPool, query, flags, index);
}

void  VulkanCommandRecorder::vkCmdEndQueryIndexedEXT(VkQueryPool queryPool, uint32_t query, uint32_t index) const noexcept
{
    ::vkCmdEndQueryIndexedEXT(_command_buffer, queryPool, query, index);
}

void  VulkanCommandRecorder::vkCmdDrawIndirectByteCountEXT(uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride) const noexcept
{
    ::vkCmdDrawIndirectByteCountEXT(_command_buffer, instanceCount, firstInstance, counterBuffer, counterBufferOffset, counterOffset, vertexStride);
}

void  VulkanCommandRecorder::vkCmdCuLaunchKernelNVX(const VkCuLaunchInfoNVX* pLaunchInfo) const noexcept
{
    ::vkCmdCuLaunchKernelNVX(_command_buffer, pLaunchInfo);
}

void  VulkanCommandRecorder::vkCmdDrawIndirectCountAMD(VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const noexcept
{
    ::vkCmdDrawIndirectCountAMD(_command_buffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void  VulkanCommandRecorder::vkCmdDrawIndexedIndirectCountAMD(VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const noexcept
{
    ::vkCmdDrawIndexedIndirectCountAMD(_command_buffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void  VulkanCommandRecorder::vkCmdBeginConditionalRenderingEXT(const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin) const noexcept
{
    ::vkCmdBeginConditionalRenderingEXT(_command_buffer, pConditionalRenderingBegin);
}

void  VulkanCommandRecorder::vkCmdEndConditionalRenderingEXT() const noexcept
{
    ::vkCmdEndConditionalRenderingEXT(_command_buffer);
}

void  VulkanCommandRecorder::vkCmdSetViewportWScalingNV(uint32_t firstViewport, uint32_t viewportCount, const VkViewportWScalingNV* pViewportWScalings) const noexcept
{
    ::vkCmdSetViewportWScalingNV(_command_buffer, firstViewport, viewportCount, pViewportWScalings);
}

void  VulkanCommandRecorder::vkCmdSetDiscardRectangleEXT(uint32_t firstDiscardRectangle, uint32_t discardRectangleCount, const VkRect2D* pDiscardRectangles) const noexcept
{
    ::vkCmdSetDiscardRectangleEXT(_command_buffer, firstDiscardRectangle, discardRectangleCount, pDiscardRectangles);
}

void  VulkanCommandRecorder::vkCmdBeginDebugUtilsLabelEXT(const VkDebugUtilsLabelEXT* pLabelInfo) const noexcept
{
    ::vkCmdBeginDebugUtilsLabelEXT(_command_buffer, pLabelInfo);
}

void  VulkanCommandRecorder::vkCmdEndDebugUtilsLabelEXT() const noexcept
{
    ::vkCmdEndDebugUtilsLabelEXT(_command_buffer);
}

void  VulkanCommandRecorder::vkCmdInsertDebugUtilsLabelEXT(const VkDebugUtilsLabelEXT* pLabelInfo) const noexcept
{
    ::vkCmdInsertDebugUtilsLabelEXT(_command_buffer, pLabelInfo);
}

void  VulkanCommandRecorder::vkCmdSetSampleLocationsEXT(const VkSampleLocationsInfoEXT* pSampleLocationsInfo) const noexcept
{
    ::vkCmdSetSampleLocationsEXT(_command_buffer, pSampleLocationsInfo);
}

void  VulkanCommandRecorder::vkCmdBindShadingRateImageNV(VkImageView imageView, VkImageLayout imageLayout) const noexcept
{
    ::vkCmdBindShadingRateImageNV(_command_buffer, imageView, imageLayout);
}

void  VulkanCommandRecorder::vkCmdSetViewportShadingRatePaletteNV(uint32_t firstViewport, uint32_t viewportCount, const VkShadingRatePaletteNV* pShadingRatePalettes) const noexcept
{
    ::vkCmdSetViewportShadingRatePaletteNV(_command_buffer, firstViewport, viewportCount, pShadingRatePalettes);
}

void  VulkanCommandRecorder::vkCmdSetCoarseSampleOrderNV(VkCoarseSampleOrderTypeNV sampleOrderType, uint32_t customSampleOrderCount, const VkCoarseSampleOrderCustomNV* pCustomSampleOrders) const noexcept
{
    ::vkCmdSetCoarseSampleOrderNV(_command_buffer, sampleOrderType, customSampleOrderCount, pCustomSampleOrders);
}

void  VulkanCommandRecorder::vkCmdBuildAccelerationStructureNV(const VkAccelerationStructureInfoNV* pInfo, VkBuffer instanceData, VkDeviceSize instanceOffset, VkBool32 update, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkBuffer scratch, VkDeviceSize scratchOffset) const noexcept
{
    ::vkCmdBuildAccelerationStructureNV(_command_buffer, pInfo, instanceData, instanceOffset, update, dst, src, scratch, scratchOffset);
}

void  VulkanCommandRecorder::vkCmdCopyAccelerationStructureNV(VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkCopyAccelerationStructureModeKHR mode) const noexcept
{
    ::vkCmdCopyAccelerationStructureNV(_command_buffer, dst, src, mode);
}

void  VulkanCommandRecorder::vkCmdTraceRaysNV(VkBuffer raygenShaderBindingTableBuffer, VkDeviceSize raygenShaderBindingOffset, VkBuffer missShaderBindingTableBuffer, VkDeviceSize missShaderBindingOffset, VkDeviceSize missShaderBindingStride, VkBuffer hitShaderBindingTableBuffer, VkDeviceSize hitShaderBindingOffset, VkDeviceSize hitShaderBindingStride, VkBuffer callableShaderBindingTableBuffer, VkDeviceSize callableShaderBindingOffset, VkDeviceSize callableShaderBindingStride, uint32_t width, uint32_t height, uint32_t depth) const noexcept
{
    ::vkCmdTraceRaysNV(_command_buffer, raygenShaderBindingTableBuffer, raygenShaderBindingOffset, missShaderBindingTableBuffer, missShaderBindingOffset, missShaderBindingStride, hitShaderBindingTableBuffer, hitShaderBindingOffset, hitShaderBindingStride, callableShaderBindingTableBuffer, callableShaderBindingOffset, callableShaderBindingStride, width, height, depth);
}

void  VulkanCommandRecorder::vkCmdWriteAccelerationStructuresPropertiesNV(uint32_t accelerationStructureCount, const VkAccelerationStructureNV* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) const noexcept
{
    ::vkCmdWriteAccelerationStructuresPropertiesNV(_command_buffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
}

void  VulkanCommandRecorder::vkCmdWriteBufferMarkerAMD(VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker) const noexcept
{
    ::vkCmdWriteBufferMarkerAMD(_command_buffer, pipelineStage, dstBuffer, dstOffset, marker);
}

void  VulkanCommandRecorder::vkCmdDrawMeshTasksNV(uint32_t taskCount, uint32_t firstTask) const noexcept
{
    ::vkCmdDrawMeshTasksNV(_command_buffer, taskCount, firstTask);
}

void  VulkanCommandRecorder::vkCmdDrawMeshTasksIndirectNV(VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) const noexcept
{
    ::vkCmdDrawMeshTasksIndirectNV(_command_buffer, buffer, offset, drawCount, stride);
}

void  VulkanCommandRecorder::vkCmdDrawMeshTasksIndirectCountNV(VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const noexcept
{
    ::vkCmdDrawMeshTasksIndirectCountNV(_command_buffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void  VulkanCommandRecorder::vkCmdSetExclusiveScissorNV(uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkRect2D* pExclusiveScissors) const noexcept
{
    ::vkCmdSetExclusiveScissorNV(_command_buffer, firstExclusiveScissor, exclusiveScissorCount, pExclusiveScissors);
}

void  VulkanCommandRecorder::vkCmdSetCheckpointNV(const void* pCheckpointMarker) const noexcept
{
    ::vkCmdSetCheckpointNV(_command_buffer, pCheckpointMarker);
}

VulkanResult  VulkanCommandRecorder::vkCmdSetPerformanceMarkerINTEL(const VkPerformanceMarkerInfoINTEL* pMarkerInfo) const noexcept
{
    return ::vkCmdSetPerformanceMarkerINTEL(_command_buffer, pMarkerInfo);
}

VulkanResult  VulkanCommandRecorder::vkCmdSetPerformanceStreamMarkerINTEL(const VkPerformanceStreamMarkerInfoINTEL* pMarkerInfo) const noexcept
{
    return ::vkCmdSetPerformanceStreamMarkerINTEL(_command_buffer, pMarkerInfo);
}

VulkanResult  VulkanCommandRecorder::vkCmdSetPerformanceOverrideINTEL(const VkPerformanceOverrideInfoINTEL* pOverrideInfo) const noexcept
{
    return ::vkCmdSetPerformanceOverrideINTEL(_command_buffer, pOverrideInfo);
}

void  VulkanCommandRecorder::vkCmdSetLineStippleEXT(uint32_t lineStippleFactor, uint16_t lineStipplePattern) const noexcept
{
    ::vkCmdSetLineStippleEXT(_command_buffer, lineStippleFactor, lineStipplePattern);
}

void  VulkanCommandRecorder::vkCmdSetCullModeEXT(VkCullModeFlags cullMode) const noexcept
{
    ::vkCmdSetCullModeEXT(_command_buffer, cullMode);
}

void  VulkanCommandRecorder::vkCmdSetFrontFaceEXT(VkFrontFace frontFace) const noexcept
{
    ::vkCmdSetFrontFaceEXT(_command_buffer, frontFace);
}

void  VulkanCommandRecorder::vkCmdSetPrimitiveTopologyEXT(VkPrimitiveTopology primitiveTopology) const noexcept
{
    ::vkCmdSetPrimitiveTopologyEXT(_command_buffer, primitiveTopology);
}

void  VulkanCommandRecorder::vkCmdSetViewportWithCountEXT(uint32_t viewportCount, const VkViewport* pViewports) const noexcept
{
    ::vkCmdSetViewportWithCountEXT(_command_buffer, viewportCount, pViewports);
}

void  VulkanCommandRecorder::vkCmdSetScissorWithCountEXT(uint32_t scissorCount, const VkRect2D* pScissors) const noexcept
{
    ::vkCmdSetScissorWithCountEXT(_command_buffer, scissorCount, pScissors);
}

void  VulkanCommandRecorder::vkCmdBindVertexBuffers2EXT(uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides) const noexcept
{
    ::vkCmdBindVertexBuffers2EXT(_command_buffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes, pStrides);
}

void  VulkanCommandRecorder::vkCmdSetDepthTestEnableEXT(VkBool32 depthTestEnable) const noexcept
{
    ::vkCmdSetDepthTestEnableEXT(_command_buffer, depthTestEnable);
}

void  VulkanCommandRecorder::vkCmdSetDepthWriteEnableEXT(VkBool32 depthWriteEnable) const noexcept
{
    ::vkCmdSetDepthWriteEnableEXT(_command_buffer, depthWriteEnable);
}

void  VulkanCommandRecorder::vkCmdSetDepthCompareOpEXT(VkCompareOp depthCompareOp) const noexcept
{
    ::vkCmdSetDepthCompareOpEXT(_command_buffer, depthCompareOp);
}

void  VulkanCommandRecorder::vkCmdSetDepthBoundsTestEnableEXT(VkBool32 depthBoundsTestEnable) const noexcept
{
    ::vkCmdSetDepthBoundsTestEnableEXT(_command_buffer, depthBoundsTestEnable);
}

void  VulkanCommandRecorder::vkCmdSetStencilTestEnableEXT(VkBool32 stencilTestEnable) const noexcept
{
    ::vkCmdSetStencilTestEnableEXT(_command_buffer, stencilTestEnable);
}

void  VulkanCommandRecorder::vkCmdSetStencilOpEXT(VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp) const noexcept
{
    ::vkCmdSetStencilOpEXT(_command_buffer, faceMask, failOp, passOp, depthFailOp, compareOp);
}

void  VulkanCommandRecorder::vkCmdPreprocessGeneratedCommandsNV(const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo) const noexcept
{
    ::vkCmdPreprocessGeneratedCommandsNV(_command_buffer, pGeneratedCommandsInfo);
}

void  VulkanCommandRecorder::vkCmdExecuteGeneratedCommandsNV(VkBool32 isPreprocessed, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo) const noexcept
{
    ::vkCmdExecuteGeneratedCommandsNV(_command_buffer, isPreprocessed, pGeneratedCommandsInfo);
}

void  VulkanCommandRecorder::vkCmdBindPipelineShaderGroupNV(VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline, uint32_t groupIndex) const noexcept
{
    ::vkCmdBindPipelineShaderGroupNV(_command_buffer, pipelineBindPoint, pipeline, groupIndex);
}

void  VulkanCommandRecorder::vkCmdSetFragmentShadingRateEnumNV(VkFragmentShadingRateNV shadingRate, const VkFragmentShadingRateCombinerOpKHR combinerOps[2]) const noexcept
{
    ::vkCmdSetFragmentShadingRateEnumNV(_command_buffer, shadingRate, combinerOps);
}

void  VulkanCommandRecorder::vkCmdSetVertexInputEXT(uint32_t vertexBindingDescriptionCount, const VkVertexInputBindingDescription2EXT* pVertexBindingDescriptions, uint32_t vertexAttributeDescriptionCount, const VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions) const noexcept
{
    ::vkCmdSetVertexInputEXT(_command_buffer, vertexBindingDescriptionCount, pVertexBindingDescriptions, vertexAttributeDescriptionCount, pVertexAttributeDescriptions);
}

void  VulkanCommandRecorder::vkCmdSubpassShadingHUAWEI() const noexcept
{
    ::vkCmdSubpassShadingHUAWEI(_command_buffer);
}

void  VulkanCommandRecorder::vkCmdBindInvocationMaskHUAWEI(VkImageView imageView, VkImageLayout imageLayout) const noexcept
{
    ::vkCmdBindInvocationMaskHUAWEI(_command_buffer, imageView, imageLayout);
}

void  VulkanCommandRecorder::vkCmdSetPatchControlPointsEXT(uint32_t patchControlPoints) const noexcept
{
    ::vkCmdSetPatchControlPointsEXT(_command_buffer, patchControlPoints);
}

void  VulkanCommandRecorder::vkCmdSetRasterizerDiscardEnableEXT(VkBool32 rasterizerDiscardEnable) const noexcept
{
    ::vkCmdSetRasterizerDiscardEnableEXT(_command_buffer, rasterizerDiscardEnable);
}

void  VulkanCommandRecorder::vkCmdSetDepthBiasEnableEXT(VkBool32 depthBiasEnable) const noexcept
{
    ::vkCmdSetDepthBiasEnableEXT(_command_buffer, depthBiasEnable);
}

void  VulkanCommandRecorder::vkCmdSetLogicOpEXT(VkLogicOp logicOp) const noexcept
{
    ::vkCmdSetLogicOpEXT(_command_buffer, logicOp);
}

void  VulkanCommandRecorder::vkCmdSetPrimitiveRestartEnableEXT(VkBool32 primitiveRestartEnable) const noexcept
{
    ::vkCmdSetPrimitiveRestartEnableEXT(_command_buffer, primitiveRestartEnable);
}

void  VulkanCommandRecorder::vkCmdSetColorWriteEnableEXT(uint32_t attachmentCount, const VkBool32* pColorWriteEnables) const noexcept
{
    ::vkCmdSetColorWriteEnableEXT(_command_buffer, attachmentCount, pColorWriteEnables);
}

void  VulkanCommandRecorder::vkCmdDrawMultiEXT(uint32_t drawCount, const VkMultiDrawInfoEXT* pVertexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride) const noexcept
{
    ::vkCmdDrawMultiEXT(_command_buffer, drawCount, pVertexInfo, instanceCount, firstInstance, stride);
}

void  VulkanCommandRecorder::vkCmdDrawMultiIndexedEXT(uint32_t drawCount, const VkMultiDrawIndexedInfoEXT* pIndexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride, const int32_t* pVertexOffset) const noexcept
{
    ::vkCmdDrawMultiIndexedEXT(_command_buffer, drawCount, pIndexInfo, instanceCount, firstInstance, stride, pVertexOffset);
}

void  VulkanCommandRecorder::vkCmdBuildAccelerationStructuresKHR(uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos) const noexcept
{
    ::vkCmdBuildAccelerationStructuresKHR(_command_buffer, infoCount, pInfos, ppBuildRangeInfos);
}

void  VulkanCommandRecorder::vkCmdBuildAccelerationStructuresIndirectKHR(uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkDeviceAddress* pIndirectDeviceAddresses, const uint32_t* pIndirectStrides, const uint32_t* const* ppMaxPrimitiveCounts) const noexcept
{
    ::vkCmdBuildAccelerationStructuresIndirectKHR(_command_buffer, infoCount, pInfos, pIndirectDeviceAddresses, pIndirectStrides, ppMaxPrimitiveCounts);
}

void  VulkanCommandRecorder::vkCmdCopyAccelerationStructureKHR(const VkCopyAccelerationStructureInfoKHR* pInfo) const noexcept
{
    ::vkCmdCopyAccelerationStructureKHR(_command_buffer, pInfo);
}

void  VulkanCommandRecorder::vkCmdCopyAccelerationStructureToMemoryKHR(const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo) const noexcept
{
    ::vkCmdCopyAccelerationStructureToMemoryKHR(_command_buffer, pInfo);
}

void  VulkanCommandRecorder::vkCmdCopyMemoryToAccelerationStructureKHR(const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo) const noexcept
{
    ::vkCmdCopyMemoryToAccelerationStructureKHR(_command_buffer, pInfo);
}

void  VulkanCommandRecorder::vkCmdWriteAccelerationStructuresPropertiesKHR(uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) const noexcept
{
    ::vkCmdWriteAccelerationStructuresPropertiesKHR(_command_buffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
}

void  VulkanCommandRecorder::vkCmdTraceRaysKHR(const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, uint32_t width, uint32_t height, uint32_t depth) const noexcept
{
    ::vkCmdTraceRaysKHR(_command_buffer, pRaygenShaderBindingTable, pMissShaderBindingTable, pHitShaderBindingTable, pCallableShaderBindingTable, width, height, depth);
}

void  VulkanCommandRecorder::vkCmdTraceRaysIndirectKHR(const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, VkDeviceAddress indirectDeviceAddress) const noexcept
{
    ::vkCmdTraceRaysIndirectKHR(_command_buffer, pRaygenShaderBindingTable, pMissShaderBindingTable, pHitShaderBindingTable, pCallableShaderBindingTable, indirectDeviceAddress);
}

void  VulkanCommandRecorder::vkCmdSetRayTracingPipelineStackSizeKHR(uint32_t pipelineStackSize) const noexcept
{
    ::vkCmdSetRayTracingPipelineStackSizeKHR(_command_buffer, pipelineStackSize);
}

}
// clang-format on

} // namespace plex::vkapi
