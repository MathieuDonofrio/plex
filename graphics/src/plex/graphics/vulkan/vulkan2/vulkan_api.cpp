#include "vulkan_api.h"

#include "vulkan_function_table.h"
#include "vulkan_loader.h"

namespace plex::graphics::vkapi
{

using namespace loader;

// clang-format off

VulkanResult CreateInstance(const VkApplicationInfo& app_info,
const std::vector<const char*>& extensions,
const std::vector<const char*>& layers,
const void* create_info_extension) {
  return loader::CreateInstance(app_info, extensions, layers, create_info_extension);
}

VulkanResult UseDevice(VkDevice device)
{
    return loader::UseDevice(device);
};

VkInstance GetInstance() noexcept
{
    return loader::GetInstance();
}

VulkanResult vkEnumeratePhysicalDevices(uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices)
{
  return GetFunctionTable().vkEnumeratePhysicalDevices(GetInstance(), pPhysicalDeviceCount, pPhysicalDevices);
}

void vkGetPhysicalDeviceFeatures(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures)
{
  return GetFunctionTable().vkGetPhysicalDeviceFeatures(physicalDevice, pFeatures);
}

void vkGetPhysicalDeviceFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties* pFormatProperties)
{
  return GetFunctionTable().vkGetPhysicalDeviceFormatProperties(physicalDevice, format, pFormatProperties);
}

VulkanResult vkGetPhysicalDeviceImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties)
{
  return GetFunctionTable().vkGetPhysicalDeviceImageFormatProperties(physicalDevice, format, type, tiling, usage, flags, pImageFormatProperties);
}

void vkGetPhysicalDeviceProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties)
{
  return GetFunctionTable().vkGetPhysicalDeviceProperties(physicalDevice, pProperties);
}

VulkanResultWithValue<std::vector<VkQueueFamilyProperties>> vkGetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice physicalDevice)
{
  const auto& fp = GetFunctionTable().vkGetPhysicalDeviceQueueFamilyProperties;
  uint32_t count = 0;
  fp(physicalDevice, &count, nullptr);
  VulkanResultWithValue<std::vector<VkQueueFamilyProperties>> result;
  result.value.resize(count);
  fp(physicalDevice, &count, result.value.data());
  return result;
}

void vkGetPhysicalDeviceMemoryProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties)
{
  return GetFunctionTable().vkGetPhysicalDeviceMemoryProperties(physicalDevice, pMemoryProperties);
}

VulkanFunctionPointer vkGetInstanceProcAddr(const char* pName)
{
  return GetFunctionTable().vkGetInstanceProcAddr(GetInstance(), pName);
}

VulkanFunctionPointer vkGetDeviceProcAddr(const char* pName)
{
  return GetFunctionTable().vkGetDeviceProcAddr(GetDevice(), pName);
}

VulkanResult vkCreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice)
{
  return GetFunctionTable().vkCreateDevice(physicalDevice, pCreateInfo, pAllocator, pDevice);
}

VulkanResultWithValue<std::vector<VkExtensionProperties>> vkEnumerateInstanceExtensionProperties(const char* pLayerName)
{
  const auto& fp = GetFunctionTable().vkEnumerateInstanceExtensionProperties;
  uint32_t count = 0;
  fp(pLayerName, &count, nullptr);
  VulkanResultWithValue<std::vector<VkExtensionProperties>> result;
  result.value.resize(count);
  result.result = fp(pLayerName, &count, result.value.data());
  return result;
}

VulkanResultWithValue<std::vector<VkExtensionProperties>> vkEnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice, const char* pLayerName)
{
  const auto& fp = GetFunctionTable().vkEnumerateDeviceExtensionProperties;
  uint32_t count = 0;
  fp(physicalDevice, pLayerName, &count, nullptr);
  VulkanResultWithValue<std::vector<VkExtensionProperties>> result;
  result.value.resize(count);
  result.result = fp(physicalDevice, pLayerName, &count, result.value.data());
  return result;
}

VulkanResult vkEnumerateInstanceLayerProperties(uint32_t* pPropertyCount, VkLayerProperties* pProperties)
{
  return GetFunctionTable().vkEnumerateInstanceLayerProperties(pPropertyCount, pProperties);
}

VulkanResultWithValue<std::vector<VkLayerProperties>> vkEnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice)
{
  const auto& fp = GetFunctionTable().vkEnumerateDeviceLayerProperties;
  uint32_t count = 0;
  fp(physicalDevice, &count, nullptr);
  VulkanResultWithValue<std::vector<VkLayerProperties>> result;
  result.value.resize(count);
  result.result = fp(physicalDevice, &count, result.value.data());
  return result;
}

void vkGetDeviceQueue(uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue)
{
  return GetFunctionTable().vkGetDeviceQueue(GetDevice(), queueFamilyIndex, queueIndex, pQueue);
}

VulkanResult vkQueueSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence)
{
  return GetFunctionTable().vkQueueSubmit(queue, submitCount, pSubmits, fence);
}

VulkanResult vkQueueWaitIdle(VkQueue queue)
{
  return GetFunctionTable().vkQueueWaitIdle(queue);
}

VulkanResult vkDeviceWaitIdle()
{
  return GetFunctionTable().vkDeviceWaitIdle(GetDevice());
}

VulkanResult vkAllocateMemory(const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory)
{
  return GetFunctionTable().vkAllocateMemory(GetDevice(), pAllocateInfo, pAllocator, pMemory);
}

void vkFreeMemory(VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator)
{
  return GetFunctionTable().vkFreeMemory(GetDevice(), memory, pAllocator);
}

VulkanResult vkMapMemory(VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData)
{
  return GetFunctionTable().vkMapMemory(GetDevice(), memory, offset, size, flags, ppData);
}

void vkUnmapMemory(VkDeviceMemory memory)
{
  return GetFunctionTable().vkUnmapMemory(GetDevice(), memory);
}

VulkanResult vkFlushMappedMemoryRanges(uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges)
{
  return GetFunctionTable().vkFlushMappedMemoryRanges(GetDevice(), memoryRangeCount, pMemoryRanges);
}

VulkanResult vkInvalidateMappedMemoryRanges(uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges)
{
  return GetFunctionTable().vkInvalidateMappedMemoryRanges(GetDevice(), memoryRangeCount, pMemoryRanges);
}

void vkGetDeviceMemoryCommitment(VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes)
{
  return GetFunctionTable().vkGetDeviceMemoryCommitment(GetDevice(), memory, pCommittedMemoryInBytes);
}

VulkanResult vkBindBufferMemory(VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset)
{
  return GetFunctionTable().vkBindBufferMemory(GetDevice(), buffer, memory, memoryOffset);
}

VulkanResult vkBindImageMemory(VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset)
{
  return GetFunctionTable().vkBindImageMemory(GetDevice(), image, memory, memoryOffset);
}

void vkGetBufferMemoryRequirements(VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements)
{
  return GetFunctionTable().vkGetBufferMemoryRequirements(GetDevice(), buffer, pMemoryRequirements);
}

void vkGetImageMemoryRequirements(VkImage image, VkMemoryRequirements* pMemoryRequirements)
{
  return GetFunctionTable().vkGetImageMemoryRequirements(GetDevice(), image, pMemoryRequirements);
}

void vkGetImageSparseMemoryRequirements(VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements)
{
  return GetFunctionTable().vkGetImageSparseMemoryRequirements(GetDevice(), image, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}

VulkanResultWithValue<std::vector<VkSparseImageFormatProperties>> vkGetPhysicalDeviceSparseImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling)
{
  const auto& fp = GetFunctionTable().vkGetPhysicalDeviceSparseImageFormatProperties;
  uint32_t count = 0;
  fp(physicalDevice, format, type, samples, usage, tiling, &count, nullptr);
  VulkanResultWithValue<std::vector<VkSparseImageFormatProperties>> result;
  result.value.resize(count);
  fp(physicalDevice, format, type, samples, usage, tiling, &count, result.value.data());
  return result;
}

VulkanResult vkQueueBindSparse(VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence)
{
  return GetFunctionTable().vkQueueBindSparse(queue, bindInfoCount, pBindInfo, fence);
}

VulkanResult vkCreateFence(const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence)
{
  return GetFunctionTable().vkCreateFence(GetDevice(), pCreateInfo, pAllocator, pFence);
}

void vkDestroyFence(VkFence fence, const VkAllocationCallbacks* pAllocator)
{
  return GetFunctionTable().vkDestroyFence(GetDevice(), fence, pAllocator);
}

VulkanResult vkResetFences(uint32_t fenceCount, const VkFence* pFences)
{
  return GetFunctionTable().vkResetFences(GetDevice(), fenceCount, pFences);
}

VulkanResult vkGetFenceStatus(VkFence fence)
{
  return GetFunctionTable().vkGetFenceStatus(GetDevice(), fence);
}

VulkanResult vkWaitForFences(uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout)
{
  return GetFunctionTable().vkWaitForFences(GetDevice(), fenceCount, pFences, waitAll, timeout);
}

VulkanResult vkCreateSemaphore(const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore)
{
  return GetFunctionTable().vkCreateSemaphore(GetDevice(), pCreateInfo, pAllocator, pSemaphore);
}

void vkDestroySemaphore(VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator)
{
  return GetFunctionTable().vkDestroySemaphore(GetDevice(), semaphore, pAllocator);
}

VulkanResult vkCreateEvent(const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent)
{
  return GetFunctionTable().vkCreateEvent(GetDevice(), pCreateInfo, pAllocator, pEvent);
}

void vkDestroyEvent(VkEvent event, const VkAllocationCallbacks* pAllocator)
{
  return GetFunctionTable().vkDestroyEvent(GetDevice(), event, pAllocator);
}

VulkanResult vkGetEventStatus(VkEvent event)
{
  return GetFunctionTable().vkGetEventStatus(GetDevice(), event);
}

VulkanResult vkSetEvent(VkEvent event)
{
  return GetFunctionTable().vkSetEvent(GetDevice(), event);
}

VulkanResult vkResetEvent(VkEvent event)
{
  return GetFunctionTable().vkResetEvent(GetDevice(), event);
}

VulkanResult vkCreateQueryPool(const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool)
{
  return GetFunctionTable().vkCreateQueryPool(GetDevice(), pCreateInfo, pAllocator, pQueryPool);
}

void vkDestroyQueryPool(VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator)
{
  return GetFunctionTable().vkDestroyQueryPool(GetDevice(), queryPool, pAllocator);
}

VulkanResult vkGetQueryPoolResults(VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags)
{
  return GetFunctionTable().vkGetQueryPoolResults(GetDevice(), queryPool, firstQuery, queryCount, dataSize, pData, stride, flags);
}

VulkanResult vkCreateBuffer(const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer)
{
  return GetFunctionTable().vkCreateBuffer(GetDevice(), pCreateInfo, pAllocator, pBuffer);
}

void vkDestroyBuffer(VkBuffer buffer, const VkAllocationCallbacks* pAllocator)
{
  return GetFunctionTable().vkDestroyBuffer(GetDevice(), buffer, pAllocator);
}

VulkanResult vkCreateBufferView(const VkBufferViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferView* pView)
{
  return GetFunctionTable().vkCreateBufferView(GetDevice(), pCreateInfo, pAllocator, pView);
}

void vkDestroyBufferView(VkBufferView bufferView, const VkAllocationCallbacks* pAllocator)
{
  return GetFunctionTable().vkDestroyBufferView(GetDevice(), bufferView, pAllocator);
}

VulkanResult vkCreateImage(const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage)
{
  return GetFunctionTable().vkCreateImage(GetDevice(), pCreateInfo, pAllocator, pImage);
}

void vkDestroyImage(VkImage image, const VkAllocationCallbacks* pAllocator)
{
  return GetFunctionTable().vkDestroyImage(GetDevice(), image, pAllocator);
}

void vkGetImageSubresourceLayout(VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout)
{
  return GetFunctionTable().vkGetImageSubresourceLayout(GetDevice(), image, pSubresource, pLayout);
}

VulkanResult vkCreateImageView(const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView)
{
  return GetFunctionTable().vkCreateImageView(GetDevice(), pCreateInfo, pAllocator, pView);
}

void vkDestroyImageView(VkImageView imageView, const VkAllocationCallbacks* pAllocator)
{
  return GetFunctionTable().vkDestroyImageView(GetDevice(), imageView, pAllocator);
}

VulkanResult vkCreateShaderModule(const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule)
{
  return GetFunctionTable().vkCreateShaderModule(GetDevice(), pCreateInfo, pAllocator, pShaderModule);
}

void vkDestroyShaderModule(VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator)
{
  return GetFunctionTable().vkDestroyShaderModule(GetDevice(), shaderModule, pAllocator);
}

VulkanResult vkCreatePipelineCache(const VkPipelineCacheCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache)
{
  return GetFunctionTable().vkCreatePipelineCache(GetDevice(), pCreateInfo, pAllocator, pPipelineCache);
}

void vkDestroyPipelineCache(VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator)
{
  return GetFunctionTable().vkDestroyPipelineCache(GetDevice(), pipelineCache, pAllocator);
}

VulkanResult vkGetPipelineCacheData(VkPipelineCache pipelineCache, size_t* pDataSize, void* pData)
{
  return GetFunctionTable().vkGetPipelineCacheData(GetDevice(), pipelineCache, pDataSize, pData);
}

VulkanResult vkMergePipelineCaches(VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches)
{
  return GetFunctionTable().vkMergePipelineCaches(GetDevice(), dstCache, srcCacheCount, pSrcCaches);
}

VulkanResult vkCreateGraphicsPipelines(VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
{
  return GetFunctionTable().vkCreateGraphicsPipelines(GetDevice(), pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}

VulkanResult vkCreateComputePipelines(VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
{
  return GetFunctionTable().vkCreateComputePipelines(GetDevice(), pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}

void vkDestroyPipeline(VkPipeline pipeline, const VkAllocationCallbacks* pAllocator)
{
  return GetFunctionTable().vkDestroyPipeline(GetDevice(), pipeline, pAllocator);
}

VulkanResult vkCreatePipelineLayout(const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout)
{
  return GetFunctionTable().vkCreatePipelineLayout(GetDevice(), pCreateInfo, pAllocator, pPipelineLayout);
}

void vkDestroyPipelineLayout(VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator)
{
  return GetFunctionTable().vkDestroyPipelineLayout(GetDevice(), pipelineLayout, pAllocator);
}

VulkanResult vkCreateSampler(const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler)
{
  return GetFunctionTable().vkCreateSampler(GetDevice(), pCreateInfo, pAllocator, pSampler);
}

void vkDestroySampler(VkSampler sampler, const VkAllocationCallbacks* pAllocator)
{
  return GetFunctionTable().vkDestroySampler(GetDevice(), sampler, pAllocator);
}

VulkanResult vkCreateDescriptorSetLayout(const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout)
{
  return GetFunctionTable().vkCreateDescriptorSetLayout(GetDevice(), pCreateInfo, pAllocator, pSetLayout);
}

void vkDestroyDescriptorSetLayout(VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator)
{
  return GetFunctionTable().vkDestroyDescriptorSetLayout(GetDevice(), descriptorSetLayout, pAllocator);
}

VulkanResult vkCreateDescriptorPool(const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool)
{
  return GetFunctionTable().vkCreateDescriptorPool(GetDevice(), pCreateInfo, pAllocator, pDescriptorPool);
}

void vkDestroyDescriptorPool(VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator)
{
  return GetFunctionTable().vkDestroyDescriptorPool(GetDevice(), descriptorPool, pAllocator);
}

VulkanResult vkResetDescriptorPool(VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags)
{
  return GetFunctionTable().vkResetDescriptorPool(GetDevice(), descriptorPool, flags);
}

VulkanResult vkAllocateDescriptorSets(const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets)
{
  return GetFunctionTable().vkAllocateDescriptorSets(GetDevice(), pAllocateInfo, pDescriptorSets);
}

VulkanResult vkFreeDescriptorSets(VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets)
{
  return GetFunctionTable().vkFreeDescriptorSets(GetDevice(), descriptorPool, descriptorSetCount, pDescriptorSets);
}

void vkUpdateDescriptorSets(uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies)
{
  return GetFunctionTable().vkUpdateDescriptorSets(GetDevice(), descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies);
}

VulkanResult vkCreateFramebuffer(const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer)
{
  return GetFunctionTable().vkCreateFramebuffer(GetDevice(), pCreateInfo, pAllocator, pFramebuffer);
}

void vkDestroyFramebuffer(VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator)
{
  return GetFunctionTable().vkDestroyFramebuffer(GetDevice(), framebuffer, pAllocator);
}

VulkanResult vkCreateRenderPass(const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass)
{
  return GetFunctionTable().vkCreateRenderPass(GetDevice(), pCreateInfo, pAllocator, pRenderPass);
}

void vkDestroyRenderPass(VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator)
{
  return GetFunctionTable().vkDestroyRenderPass(GetDevice(), renderPass, pAllocator);
}

void vkGetRenderAreaGranularity(VkRenderPass renderPass, VkExtent2D* pGranularity)
{
  return GetFunctionTable().vkGetRenderAreaGranularity(GetDevice(), renderPass, pGranularity);
}

VulkanResult vkCreateCommandPool(const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool)
{
  return GetFunctionTable().vkCreateCommandPool(GetDevice(), pCreateInfo, pAllocator, pCommandPool);
}

void vkDestroyCommandPool(VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator)
{
  return GetFunctionTable().vkDestroyCommandPool(GetDevice(), commandPool, pAllocator);
}

VulkanResult vkResetCommandPool(VkCommandPool commandPool, VkCommandPoolResetFlags flags)
{
  return GetFunctionTable().vkResetCommandPool(GetDevice(), commandPool, flags);
}

VulkanResult vkAllocateCommandBuffers(const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers)
{
  return GetFunctionTable().vkAllocateCommandBuffers(GetDevice(), pAllocateInfo, pCommandBuffers);
}

void vkFreeCommandBuffers(VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers)
{
  return GetFunctionTable().vkFreeCommandBuffers(GetDevice(), commandPool, commandBufferCount, pCommandBuffers);
}

VulkanResult vkEnumerateInstanceVersion(uint32_t* pApiVersion)
{
  return GetFunctionTable().vkEnumerateInstanceVersion(pApiVersion);
}

VulkanResult vkBindBufferMemory2(uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos)
{
  return GetFunctionTable().vkBindBufferMemory2(GetDevice(), bindInfoCount, pBindInfos);
}

VulkanResult vkBindImageMemory2(uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos)
{
  return GetFunctionTable().vkBindImageMemory2(GetDevice(), bindInfoCount, pBindInfos);
}

void vkGetDeviceGroupPeerMemoryFeatures(uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures)
{
  return GetFunctionTable().vkGetDeviceGroupPeerMemoryFeatures(GetDevice(), heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
}

VulkanResult vkEnumeratePhysicalDeviceGroups(uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties)
{
  return GetFunctionTable().vkEnumeratePhysicalDeviceGroups(GetInstance(), pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
}

void vkGetImageMemoryRequirements2(const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
  return GetFunctionTable().vkGetImageMemoryRequirements2(GetDevice(), pInfo, pMemoryRequirements);
}

void vkGetBufferMemoryRequirements2(const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
  return GetFunctionTable().vkGetBufferMemoryRequirements2(GetDevice(), pInfo, pMemoryRequirements);
}

void vkGetImageSparseMemoryRequirements2(const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements)
{
  return GetFunctionTable().vkGetImageSparseMemoryRequirements2(GetDevice(), pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}

void vkGetPhysicalDeviceFeatures2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures)
{
  return GetFunctionTable().vkGetPhysicalDeviceFeatures2(physicalDevice, pFeatures);
}

void vkGetPhysicalDeviceProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties)
{
  return GetFunctionTable().vkGetPhysicalDeviceProperties2(physicalDevice, pProperties);
}

void vkGetPhysicalDeviceFormatProperties2(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties)
{
  return GetFunctionTable().vkGetPhysicalDeviceFormatProperties2(physicalDevice, format, pFormatProperties);
}

VulkanResult vkGetPhysicalDeviceImageFormatProperties2(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties)
{
  return GetFunctionTable().vkGetPhysicalDeviceImageFormatProperties2(physicalDevice, pImageFormatInfo, pImageFormatProperties);
}

VulkanResultWithValue<std::vector<VkQueueFamilyProperties2>> vkGetPhysicalDeviceQueueFamilyProperties2(VkPhysicalDevice physicalDevice)
{
  const auto& fp = GetFunctionTable().vkGetPhysicalDeviceQueueFamilyProperties2;
  uint32_t count = 0;
  fp(physicalDevice, &count, nullptr);
  VulkanResultWithValue<std::vector<VkQueueFamilyProperties2>> result;
  result.value.resize(count);
  fp(physicalDevice, &count, result.value.data());
  return result;
}

void vkGetPhysicalDeviceMemoryProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties)
{
  return GetFunctionTable().vkGetPhysicalDeviceMemoryProperties2(physicalDevice, pMemoryProperties);
}

VulkanResultWithValue<std::vector<VkSparseImageFormatProperties2>> vkGetPhysicalDeviceSparseImageFormatProperties2(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo)
{
  const auto& fp = GetFunctionTable().vkGetPhysicalDeviceSparseImageFormatProperties2;
  uint32_t count = 0;
  fp(physicalDevice, pFormatInfo, &count, nullptr);
  VulkanResultWithValue<std::vector<VkSparseImageFormatProperties2>> result;
  result.value.resize(count);
  fp(physicalDevice, pFormatInfo, &count, result.value.data());
  return result;
}

void vkTrimCommandPool(VkCommandPool commandPool, VkCommandPoolTrimFlags flags)
{
  return GetFunctionTable().vkTrimCommandPool(GetDevice(), commandPool, flags);
}

void vkGetDeviceQueue2(const VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue)
{
  return GetFunctionTable().vkGetDeviceQueue2(GetDevice(), pQueueInfo, pQueue);
}

VulkanResult vkCreateSamplerYcbcrConversion(const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion)
{
  return GetFunctionTable().vkCreateSamplerYcbcrConversion(GetDevice(), pCreateInfo, pAllocator, pYcbcrConversion);
}

void vkDestroySamplerYcbcrConversion(VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator)
{
  return GetFunctionTable().vkDestroySamplerYcbcrConversion(GetDevice(), ycbcrConversion, pAllocator);
}

VulkanResult vkCreateDescriptorUpdateTemplate(const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate)
{
  return GetFunctionTable().vkCreateDescriptorUpdateTemplate(GetDevice(), pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
}

void vkDestroyDescriptorUpdateTemplate(VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator)
{
  return GetFunctionTable().vkDestroyDescriptorUpdateTemplate(GetDevice(), descriptorUpdateTemplate, pAllocator);
}

void vkUpdateDescriptorSetWithTemplate(VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData)
{
  return GetFunctionTable().vkUpdateDescriptorSetWithTemplate(GetDevice(), descriptorSet, descriptorUpdateTemplate, pData);
}

void vkGetPhysicalDeviceExternalBufferProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties)
{
  return GetFunctionTable().vkGetPhysicalDeviceExternalBufferProperties(physicalDevice, pExternalBufferInfo, pExternalBufferProperties);
}

void vkGetPhysicalDeviceExternalFenceProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties)
{
  return GetFunctionTable().vkGetPhysicalDeviceExternalFenceProperties(physicalDevice, pExternalFenceInfo, pExternalFenceProperties);
}

void vkGetPhysicalDeviceExternalSemaphoreProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties)
{
  return GetFunctionTable().vkGetPhysicalDeviceExternalSemaphoreProperties(physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
}

void vkGetDescriptorSetLayoutSupport(const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport)
{
  return GetFunctionTable().vkGetDescriptorSetLayoutSupport(GetDevice(), pCreateInfo, pSupport);
}

VulkanResult vkCreateRenderPass2(const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass)
{
  return GetFunctionTable().vkCreateRenderPass2(GetDevice(), pCreateInfo, pAllocator, pRenderPass);
}

void vkResetQueryPool(VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount)
{
  return GetFunctionTable().vkResetQueryPool(GetDevice(), queryPool, firstQuery, queryCount);
}

VulkanResult vkGetSemaphoreCounterValue(VkSemaphore semaphore, uint64_t* pValue)
{
  return GetFunctionTable().vkGetSemaphoreCounterValue(GetDevice(), semaphore, pValue);
}

VulkanResult vkWaitSemaphores(const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout)
{
  return GetFunctionTable().vkWaitSemaphores(GetDevice(), pWaitInfo, timeout);
}

VulkanResult vkSignalSemaphore(const VkSemaphoreSignalInfo* pSignalInfo)
{
  return GetFunctionTable().vkSignalSemaphore(GetDevice(), pSignalInfo);
}

VulkanResultWithValue<VkDeviceAddress> vkGetBufferDeviceAddress(const VkBufferDeviceAddressInfo* pInfo)
{
  return GetFunctionTable().vkGetBufferDeviceAddress(GetDevice(), pInfo);
}

VulkanResultWithValue<uint64_t> vkGetBufferOpaqueCaptureAddress(const VkBufferDeviceAddressInfo* pInfo)
{
  return GetFunctionTable().vkGetBufferOpaqueCaptureAddress(GetDevice(), pInfo);
}

VulkanResultWithValue<uint64_t> vkGetDeviceMemoryOpaqueCaptureAddress(const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo)
{
  return GetFunctionTable().vkGetDeviceMemoryOpaqueCaptureAddress(GetDevice(), pInfo);
}

VulkanResultWithValue<std::vector<VkPhysicalDeviceToolProperties>> vkGetPhysicalDeviceToolProperties(VkPhysicalDevice physicalDevice)
{
  const auto& fp = GetFunctionTable().vkGetPhysicalDeviceToolProperties;
  uint32_t count = 0;
  fp(physicalDevice, &count, nullptr);
  VulkanResultWithValue<std::vector<VkPhysicalDeviceToolProperties>> result;
  result.value.resize(count);
  result.result = fp(physicalDevice, &count, result.value.data());
  return result;
}

VulkanResult vkCreatePrivateDataSlot(const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot)
{
  return GetFunctionTable().vkCreatePrivateDataSlot(GetDevice(), pCreateInfo, pAllocator, pPrivateDataSlot);
}

void vkDestroyPrivateDataSlot(VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator)
{
  return GetFunctionTable().vkDestroyPrivateDataSlot(GetDevice(), privateDataSlot, pAllocator);
}

VulkanResult vkSetPrivateData(VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data)
{
  return GetFunctionTable().vkSetPrivateData(GetDevice(), objectType, objectHandle, privateDataSlot, data);
}

void vkGetPrivateData(VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData)
{
  return GetFunctionTable().vkGetPrivateData(GetDevice(), objectType, objectHandle, privateDataSlot, pData);
}

VulkanResult vkQueueSubmit2(VkQueue queue, uint32_t submitCount, const VkSubmitInfo2* pSubmits, VkFence fence)
{
  return GetFunctionTable().vkQueueSubmit2(queue, submitCount, pSubmits, fence);
}

void vkGetDeviceBufferMemoryRequirements(const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
  return GetFunctionTable().vkGetDeviceBufferMemoryRequirements(GetDevice(), pInfo, pMemoryRequirements);
}

void vkGetDeviceImageMemoryRequirements(const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
  return GetFunctionTable().vkGetDeviceImageMemoryRequirements(GetDevice(), pInfo, pMemoryRequirements);
}

void vkGetDeviceImageSparseMemoryRequirements(const VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements)
{
  return GetFunctionTable().vkGetDeviceImageSparseMemoryRequirements(GetDevice(), pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}

void vkDestroySurfaceKHR(VkSurfaceKHR surface, const VkAllocationCallbacks* pAllocator)
{
  return GetFunctionTable().vkDestroySurfaceKHR(GetInstance(), surface, pAllocator);
}

VulkanResult vkGetPhysicalDeviceSurfaceSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported)
{
  return GetFunctionTable().vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, surface, pSupported);
}

VulkanResult vkGetPhysicalDeviceSurfaceCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities)
{
  return GetFunctionTable().vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, pSurfaceCapabilities);
}

VulkanResultWithValue<std::vector<VkSurfaceFormatKHR>> vkGetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
  const auto& fp = GetFunctionTable().vkGetPhysicalDeviceSurfaceFormatsKHR;
  uint32_t count = 0;
  fp(physicalDevice, surface, &count, nullptr);
  VulkanResultWithValue<std::vector<VkSurfaceFormatKHR>> result;
  result.value.resize(count);
  result.result = fp(physicalDevice, surface, &count, result.value.data());
  return result;
}

VulkanResultWithValue<std::vector<VkPresentModeKHR>> vkGetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
  const auto& fp = GetFunctionTable().vkGetPhysicalDeviceSurfacePresentModesKHR;
  uint32_t count = 0;
  fp(physicalDevice, surface, &count, nullptr);
  VulkanResultWithValue<std::vector<VkPresentModeKHR>> result;
  result.value.resize(count);
  result.result = fp(physicalDevice, surface, &count, result.value.data());
  return result;
}

VulkanResult vkCreateSwapchainKHR(const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain)
{
  return GetFunctionTable().vkCreateSwapchainKHR(GetDevice(), pCreateInfo, pAllocator, pSwapchain);
}

void vkDestroySwapchainKHR(VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator)
{
  return GetFunctionTable().vkDestroySwapchainKHR(GetDevice(), swapchain, pAllocator);
}

VulkanResult vkGetSwapchainImagesKHR(VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages)
{
  return GetFunctionTable().vkGetSwapchainImagesKHR(GetDevice(), swapchain, pSwapchainImageCount, pSwapchainImages);
}

VulkanResult vkAcquireNextImageKHR(VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex)
{
  return GetFunctionTable().vkAcquireNextImageKHR(GetDevice(), swapchain, timeout, semaphore, fence, pImageIndex);
}

VulkanResult vkQueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo)
{
  return GetFunctionTable().vkQueuePresentKHR(queue, pPresentInfo);
}

VulkanResult vkGetDeviceGroupPresentCapabilitiesKHR(VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities)
{
  return GetFunctionTable().vkGetDeviceGroupPresentCapabilitiesKHR(GetDevice(), pDeviceGroupPresentCapabilities);
}

VulkanResult vkGetDeviceGroupSurfacePresentModesKHR(VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes)
{
  return GetFunctionTable().vkGetDeviceGroupSurfacePresentModesKHR(GetDevice(), surface, pModes);
}

VulkanResultWithValue<std::vector<VkRect2D>> vkGetPhysicalDevicePresentRectanglesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
  const auto& fp = GetFunctionTable().vkGetPhysicalDevicePresentRectanglesKHR;
  uint32_t count = 0;
  fp(physicalDevice, surface, &count, nullptr);
  VulkanResultWithValue<std::vector<VkRect2D>> result;
  result.value.resize(count);
  result.result = fp(physicalDevice, surface, &count, result.value.data());
  return result;
}

VulkanResult vkAcquireNextImage2KHR(const VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex)
{
  return GetFunctionTable().vkAcquireNextImage2KHR(GetDevice(), pAcquireInfo, pImageIndex);
}

VulkanResultWithValue<std::vector<VkDisplayPropertiesKHR>> vkGetPhysicalDeviceDisplayPropertiesKHR(VkPhysicalDevice physicalDevice)
{
  const auto& fp = GetFunctionTable().vkGetPhysicalDeviceDisplayPropertiesKHR;
  uint32_t count = 0;
  fp(physicalDevice, &count, nullptr);
  VulkanResultWithValue<std::vector<VkDisplayPropertiesKHR>> result;
  result.value.resize(count);
  result.result = fp(physicalDevice, &count, result.value.data());
  return result;
}

VulkanResultWithValue<std::vector<VkDisplayPlanePropertiesKHR>> vkGetPhysicalDeviceDisplayPlanePropertiesKHR(VkPhysicalDevice physicalDevice)
{
  const auto& fp = GetFunctionTable().vkGetPhysicalDeviceDisplayPlanePropertiesKHR;
  uint32_t count = 0;
  fp(physicalDevice, &count, nullptr);
  VulkanResultWithValue<std::vector<VkDisplayPlanePropertiesKHR>> result;
  result.value.resize(count);
  result.result = fp(physicalDevice, &count, result.value.data());
  return result;
}

VulkanResultWithValue<std::vector<VkDisplayKHR>> vkGetDisplayPlaneSupportedDisplaysKHR(VkPhysicalDevice physicalDevice, uint32_t planeIndex)
{
  const auto& fp = GetFunctionTable().vkGetDisplayPlaneSupportedDisplaysKHR;
  uint32_t count = 0;
  fp(physicalDevice, planeIndex, &count, nullptr);
  VulkanResultWithValue<std::vector<VkDisplayKHR>> result;
  result.value.resize(count);
  result.result = fp(physicalDevice, planeIndex, &count, result.value.data());
  return result;
}

VulkanResultWithValue<std::vector<VkDisplayModePropertiesKHR>> vkGetDisplayModePropertiesKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display)
{
  const auto& fp = GetFunctionTable().vkGetDisplayModePropertiesKHR;
  uint32_t count = 0;
  fp(physicalDevice, display, &count, nullptr);
  VulkanResultWithValue<std::vector<VkDisplayModePropertiesKHR>> result;
  result.value.resize(count);
  result.result = fp(physicalDevice, display, &count, result.value.data());
  return result;
}

VulkanResult vkCreateDisplayModeKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, const VkDisplayModeCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDisplayModeKHR* pMode)
{
  return GetFunctionTable().vkCreateDisplayModeKHR(physicalDevice, display, pCreateInfo, pAllocator, pMode);
}

VulkanResult vkGetDisplayPlaneCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkDisplayModeKHR mode, uint32_t planeIndex, VkDisplayPlaneCapabilitiesKHR* pCapabilities)
{
  return GetFunctionTable().vkGetDisplayPlaneCapabilitiesKHR(physicalDevice, mode, planeIndex, pCapabilities);
}

VulkanResult vkCreateDisplayPlaneSurfaceKHR(const VkDisplaySurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
  return GetFunctionTable().vkCreateDisplayPlaneSurfaceKHR(GetInstance(), pCreateInfo, pAllocator, pSurface);
}

VulkanResult vkCreateSharedSwapchainsKHR(uint32_t swapchainCount, const VkSwapchainCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchains)
{
  return GetFunctionTable().vkCreateSharedSwapchainsKHR(GetDevice(), swapchainCount, pCreateInfos, pAllocator, pSwapchains);
}

void vkGetPhysicalDeviceFeatures2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures)
{
  return GetFunctionTable().vkGetPhysicalDeviceFeatures2KHR(physicalDevice, pFeatures);
}

void vkGetPhysicalDeviceProperties2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties)
{
  return GetFunctionTable().vkGetPhysicalDeviceProperties2KHR(physicalDevice, pProperties);
}

void vkGetPhysicalDeviceFormatProperties2KHR(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties)
{
  return GetFunctionTable().vkGetPhysicalDeviceFormatProperties2KHR(physicalDevice, format, pFormatProperties);
}

VulkanResult vkGetPhysicalDeviceImageFormatProperties2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties)
{
  return GetFunctionTable().vkGetPhysicalDeviceImageFormatProperties2KHR(physicalDevice, pImageFormatInfo, pImageFormatProperties);
}

VulkanResultWithValue<std::vector<VkQueueFamilyProperties2>> vkGetPhysicalDeviceQueueFamilyProperties2KHR(VkPhysicalDevice physicalDevice)
{
  const auto& fp = GetFunctionTable().vkGetPhysicalDeviceQueueFamilyProperties2KHR;
  uint32_t count = 0;
  fp(physicalDevice, &count, nullptr);
  VulkanResultWithValue<std::vector<VkQueueFamilyProperties2>> result;
  result.value.resize(count);
  fp(physicalDevice, &count, result.value.data());
  return result;
}

void vkGetPhysicalDeviceMemoryProperties2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties)
{
  return GetFunctionTable().vkGetPhysicalDeviceMemoryProperties2KHR(physicalDevice, pMemoryProperties);
}

VulkanResultWithValue<std::vector<VkSparseImageFormatProperties2>> vkGetPhysicalDeviceSparseImageFormatProperties2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo)
{
  const auto& fp = GetFunctionTable().vkGetPhysicalDeviceSparseImageFormatProperties2KHR;
  uint32_t count = 0;
  fp(physicalDevice, pFormatInfo, &count, nullptr);
  VulkanResultWithValue<std::vector<VkSparseImageFormatProperties2>> result;
  result.value.resize(count);
  fp(physicalDevice, pFormatInfo, &count, result.value.data());
  return result;
}

void vkGetDeviceGroupPeerMemoryFeaturesKHR(uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures)
{
  return GetFunctionTable().vkGetDeviceGroupPeerMemoryFeaturesKHR(GetDevice(), heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
}

void vkTrimCommandPoolKHR(VkCommandPool commandPool, VkCommandPoolTrimFlags flags)
{
  return GetFunctionTable().vkTrimCommandPoolKHR(GetDevice(), commandPool, flags);
}

VulkanResult vkEnumeratePhysicalDeviceGroupsKHR(uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties)
{
  return GetFunctionTable().vkEnumeratePhysicalDeviceGroupsKHR(GetInstance(), pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
}

void vkGetPhysicalDeviceExternalBufferPropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties)
{
  return GetFunctionTable().vkGetPhysicalDeviceExternalBufferPropertiesKHR(physicalDevice, pExternalBufferInfo, pExternalBufferProperties);
}

VulkanResult vkGetMemoryFdKHR(const VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd)
{
  return GetFunctionTable().vkGetMemoryFdKHR(GetDevice(), pGetFdInfo, pFd);
}

VulkanResult vkGetMemoryFdPropertiesKHR(VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties)
{
  return GetFunctionTable().vkGetMemoryFdPropertiesKHR(GetDevice(), handleType, fd, pMemoryFdProperties);
}

void vkGetPhysicalDeviceExternalSemaphorePropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties)
{
  return GetFunctionTable().vkGetPhysicalDeviceExternalSemaphorePropertiesKHR(physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
}

VulkanResult vkImportSemaphoreFdKHR(const VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo)
{
  return GetFunctionTable().vkImportSemaphoreFdKHR(GetDevice(), pImportSemaphoreFdInfo);
}

VulkanResult vkGetSemaphoreFdKHR(const VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd)
{
  return GetFunctionTable().vkGetSemaphoreFdKHR(GetDevice(), pGetFdInfo, pFd);
}

VulkanResult vkCreateDescriptorUpdateTemplateKHR(const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate)
{
  return GetFunctionTable().vkCreateDescriptorUpdateTemplateKHR(GetDevice(), pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
}

void vkDestroyDescriptorUpdateTemplateKHR(VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator)
{
  return GetFunctionTable().vkDestroyDescriptorUpdateTemplateKHR(GetDevice(), descriptorUpdateTemplate, pAllocator);
}

void vkUpdateDescriptorSetWithTemplateKHR(VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData)
{
  return GetFunctionTable().vkUpdateDescriptorSetWithTemplateKHR(GetDevice(), descriptorSet, descriptorUpdateTemplate, pData);
}

VulkanResult vkCreateRenderPass2KHR(const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass)
{
  return GetFunctionTable().vkCreateRenderPass2KHR(GetDevice(), pCreateInfo, pAllocator, pRenderPass);
}

VulkanResult vkGetSwapchainStatusKHR(VkSwapchainKHR swapchain)
{
  return GetFunctionTable().vkGetSwapchainStatusKHR(GetDevice(), swapchain);
}

void vkGetPhysicalDeviceExternalFencePropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties)
{
  return GetFunctionTable().vkGetPhysicalDeviceExternalFencePropertiesKHR(physicalDevice, pExternalFenceInfo, pExternalFenceProperties);
}

VulkanResult vkImportFenceFdKHR(const VkImportFenceFdInfoKHR* pImportFenceFdInfo)
{
  return GetFunctionTable().vkImportFenceFdKHR(GetDevice(), pImportFenceFdInfo);
}

VulkanResult vkGetFenceFdKHR(const VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd)
{
  return GetFunctionTable().vkGetFenceFdKHR(GetDevice(), pGetFdInfo, pFd);
}

VulkanResult vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, uint32_t* pCounterCount, VkPerformanceCounterKHR* pCounters, VkPerformanceCounterDescriptionKHR* pCounterDescriptions)
{
  return GetFunctionTable().vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(physicalDevice, queueFamilyIndex, pCounterCount, pCounters, pCounterDescriptions);
}

void vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(VkPhysicalDevice physicalDevice, const VkQueryPoolPerformanceCreateInfoKHR* pPerformanceQueryCreateInfo, uint32_t* pNumPasses)
{
  return GetFunctionTable().vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(physicalDevice, pPerformanceQueryCreateInfo, pNumPasses);
}

VulkanResult vkAcquireProfilingLockKHR(const VkAcquireProfilingLockInfoKHR* pInfo)
{
  return GetFunctionTable().vkAcquireProfilingLockKHR(GetDevice(), pInfo);
}

void vkReleaseProfilingLockKHR()
{
  return GetFunctionTable().vkReleaseProfilingLockKHR(GetDevice());
}

VulkanResult vkGetPhysicalDeviceSurfaceCapabilities2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkSurfaceCapabilities2KHR* pSurfaceCapabilities)
{
  return GetFunctionTable().vkGetPhysicalDeviceSurfaceCapabilities2KHR(physicalDevice, pSurfaceInfo, pSurfaceCapabilities);
}

VulkanResultWithValue<std::vector<VkSurfaceFormat2KHR>> vkGetPhysicalDeviceSurfaceFormats2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo)
{
  const auto& fp = GetFunctionTable().vkGetPhysicalDeviceSurfaceFormats2KHR;
  uint32_t count = 0;
  fp(physicalDevice, pSurfaceInfo, &count, nullptr);
  VulkanResultWithValue<std::vector<VkSurfaceFormat2KHR>> result;
  result.value.resize(count);
  result.result = fp(physicalDevice, pSurfaceInfo, &count, result.value.data());
  return result;
}

VulkanResultWithValue<std::vector<VkDisplayProperties2KHR>> vkGetPhysicalDeviceDisplayProperties2KHR(VkPhysicalDevice physicalDevice)
{
  const auto& fp = GetFunctionTable().vkGetPhysicalDeviceDisplayProperties2KHR;
  uint32_t count = 0;
  fp(physicalDevice, &count, nullptr);
  VulkanResultWithValue<std::vector<VkDisplayProperties2KHR>> result;
  result.value.resize(count);
  result.result = fp(physicalDevice, &count, result.value.data());
  return result;
}

VulkanResultWithValue<std::vector<VkDisplayPlaneProperties2KHR>> vkGetPhysicalDeviceDisplayPlaneProperties2KHR(VkPhysicalDevice physicalDevice)
{
  const auto& fp = GetFunctionTable().vkGetPhysicalDeviceDisplayPlaneProperties2KHR;
  uint32_t count = 0;
  fp(physicalDevice, &count, nullptr);
  VulkanResultWithValue<std::vector<VkDisplayPlaneProperties2KHR>> result;
  result.value.resize(count);
  result.result = fp(physicalDevice, &count, result.value.data());
  return result;
}

VulkanResultWithValue<std::vector<VkDisplayModeProperties2KHR>> vkGetDisplayModeProperties2KHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display)
{
  const auto& fp = GetFunctionTable().vkGetDisplayModeProperties2KHR;
  uint32_t count = 0;
  fp(physicalDevice, display, &count, nullptr);
  VulkanResultWithValue<std::vector<VkDisplayModeProperties2KHR>> result;
  result.value.resize(count);
  result.result = fp(physicalDevice, display, &count, result.value.data());
  return result;
}

VulkanResult vkGetDisplayPlaneCapabilities2KHR(VkPhysicalDevice physicalDevice, const VkDisplayPlaneInfo2KHR* pDisplayPlaneInfo, VkDisplayPlaneCapabilities2KHR* pCapabilities)
{
  return GetFunctionTable().vkGetDisplayPlaneCapabilities2KHR(physicalDevice, pDisplayPlaneInfo, pCapabilities);
}

void vkGetImageMemoryRequirements2KHR(const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
  return GetFunctionTable().vkGetImageMemoryRequirements2KHR(GetDevice(), pInfo, pMemoryRequirements);
}

void vkGetBufferMemoryRequirements2KHR(const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
  return GetFunctionTable().vkGetBufferMemoryRequirements2KHR(GetDevice(), pInfo, pMemoryRequirements);
}

void vkGetImageSparseMemoryRequirements2KHR(const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements)
{
  return GetFunctionTable().vkGetImageSparseMemoryRequirements2KHR(GetDevice(), pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}

VulkanResult vkCreateSamplerYcbcrConversionKHR(const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion)
{
  return GetFunctionTable().vkCreateSamplerYcbcrConversionKHR(GetDevice(), pCreateInfo, pAllocator, pYcbcrConversion);
}

void vkDestroySamplerYcbcrConversionKHR(VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator)
{
  return GetFunctionTable().vkDestroySamplerYcbcrConversionKHR(GetDevice(), ycbcrConversion, pAllocator);
}

VulkanResult vkBindBufferMemory2KHR(uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos)
{
  return GetFunctionTable().vkBindBufferMemory2KHR(GetDevice(), bindInfoCount, pBindInfos);
}

VulkanResult vkBindImageMemory2KHR(uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos)
{
  return GetFunctionTable().vkBindImageMemory2KHR(GetDevice(), bindInfoCount, pBindInfos);
}

void vkGetDescriptorSetLayoutSupportKHR(const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport)
{
  return GetFunctionTable().vkGetDescriptorSetLayoutSupportKHR(GetDevice(), pCreateInfo, pSupport);
}

VulkanResult vkGetSemaphoreCounterValueKHR(VkSemaphore semaphore, uint64_t* pValue)
{
  return GetFunctionTable().vkGetSemaphoreCounterValueKHR(GetDevice(), semaphore, pValue);
}

VulkanResult vkWaitSemaphoresKHR(const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout)
{
  return GetFunctionTable().vkWaitSemaphoresKHR(GetDevice(), pWaitInfo, timeout);
}

VulkanResult vkSignalSemaphoreKHR(const VkSemaphoreSignalInfo* pSignalInfo)
{
  return GetFunctionTable().vkSignalSemaphoreKHR(GetDevice(), pSignalInfo);
}

VulkanResultWithValue<std::vector<VkPhysicalDeviceFragmentShadingRateKHR>> vkGetPhysicalDeviceFragmentShadingRatesKHR(VkPhysicalDevice physicalDevice)
{
  const auto& fp = GetFunctionTable().vkGetPhysicalDeviceFragmentShadingRatesKHR;
  uint32_t count = 0;
  fp(physicalDevice, &count, nullptr);
  VulkanResultWithValue<std::vector<VkPhysicalDeviceFragmentShadingRateKHR>> result;
  result.value.resize(count);
  result.result = fp(physicalDevice, &count, result.value.data());
  return result;
}

VulkanResult vkWaitForPresentKHR(VkSwapchainKHR swapchain, uint64_t presentId, uint64_t timeout)
{
  return GetFunctionTable().vkWaitForPresentKHR(GetDevice(), swapchain, presentId, timeout);
}

VulkanResultWithValue<VkDeviceAddress> vkGetBufferDeviceAddressKHR(const VkBufferDeviceAddressInfo* pInfo)
{
  return GetFunctionTable().vkGetBufferDeviceAddressKHR(GetDevice(), pInfo);
}

VulkanResultWithValue<uint64_t> vkGetBufferOpaqueCaptureAddressKHR(const VkBufferDeviceAddressInfo* pInfo)
{
  return GetFunctionTable().vkGetBufferOpaqueCaptureAddressKHR(GetDevice(), pInfo);
}

VulkanResultWithValue<uint64_t> vkGetDeviceMemoryOpaqueCaptureAddressKHR(const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo)
{
  return GetFunctionTable().vkGetDeviceMemoryOpaqueCaptureAddressKHR(GetDevice(), pInfo);
}

VulkanResult vkCreateDeferredOperationKHR(const VkAllocationCallbacks* pAllocator, VkDeferredOperationKHR* pDeferredOperation)
{
  return GetFunctionTable().vkCreateDeferredOperationKHR(GetDevice(), pAllocator, pDeferredOperation);
}

void vkDestroyDeferredOperationKHR(VkDeferredOperationKHR operation, const VkAllocationCallbacks* pAllocator)
{
  return GetFunctionTable().vkDestroyDeferredOperationKHR(GetDevice(), operation, pAllocator);
}

VulkanResultWithValue<uint32_t> vkGetDeferredOperationMaxConcurrencyKHR(VkDeferredOperationKHR operation)
{
  return GetFunctionTable().vkGetDeferredOperationMaxConcurrencyKHR(GetDevice(), operation);
}

VulkanResult vkGetDeferredOperationResultKHR(VkDeferredOperationKHR operation)
{
  return GetFunctionTable().vkGetDeferredOperationResultKHR(GetDevice(), operation);
}

VulkanResult vkDeferredOperationJoinKHR(VkDeferredOperationKHR operation)
{
  return GetFunctionTable().vkDeferredOperationJoinKHR(GetDevice(), operation);
}

VulkanResult vkGetPipelineExecutablePropertiesKHR(const VkPipelineInfoKHR* pPipelineInfo, uint32_t* pExecutableCount, VkPipelineExecutablePropertiesKHR* pProperties)
{
  return GetFunctionTable().vkGetPipelineExecutablePropertiesKHR(GetDevice(), pPipelineInfo, pExecutableCount, pProperties);
}

VulkanResult vkGetPipelineExecutableStatisticsKHR(const VkPipelineExecutableInfoKHR* pExecutableInfo, uint32_t* pStatisticCount, VkPipelineExecutableStatisticKHR* pStatistics)
{
  return GetFunctionTable().vkGetPipelineExecutableStatisticsKHR(GetDevice(), pExecutableInfo, pStatisticCount, pStatistics);
}

VulkanResult vkGetPipelineExecutableInternalRepresentationsKHR(const VkPipelineExecutableInfoKHR* pExecutableInfo, uint32_t* pInternalRepresentationCount, VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations)
{
  return GetFunctionTable().vkGetPipelineExecutableInternalRepresentationsKHR(GetDevice(), pExecutableInfo, pInternalRepresentationCount, pInternalRepresentations);
}

VulkanResult vkQueueSubmit2KHR(VkQueue queue, uint32_t submitCount, const VkSubmitInfo2* pSubmits, VkFence fence)
{
  return GetFunctionTable().vkQueueSubmit2KHR(queue, submitCount, pSubmits, fence);
}

VulkanResultWithValue<std::vector<VkCheckpointData2NV>> vkGetQueueCheckpointData2NV(VkQueue queue)
{
  const auto& fp = GetFunctionTable().vkGetQueueCheckpointData2NV;
  uint32_t count = 0;
  fp(queue, &count, nullptr);
  VulkanResultWithValue<std::vector<VkCheckpointData2NV>> result;
  result.value.resize(count);
  fp(queue, &count, result.value.data());
  return result;
}

void vkGetDeviceBufferMemoryRequirementsKHR(const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
  return GetFunctionTable().vkGetDeviceBufferMemoryRequirementsKHR(GetDevice(), pInfo, pMemoryRequirements);
}

void vkGetDeviceImageMemoryRequirementsKHR(const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
  return GetFunctionTable().vkGetDeviceImageMemoryRequirementsKHR(GetDevice(), pInfo, pMemoryRequirements);
}

void vkGetDeviceImageSparseMemoryRequirementsKHR(const VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements)
{
  return GetFunctionTable().vkGetDeviceImageSparseMemoryRequirementsKHR(GetDevice(), pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}

VulkanResult vkCreateDebugReportCallbackEXT(const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback)
{
  return GetFunctionTable().vkCreateDebugReportCallbackEXT(GetInstance(), pCreateInfo, pAllocator, pCallback);
}

void vkDestroyDebugReportCallbackEXT(VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator)
{
  return GetFunctionTable().vkDestroyDebugReportCallbackEXT(GetInstance(), callback, pAllocator);
}

void vkDebugReportMessageEXT(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage)
{
  return GetFunctionTable().vkDebugReportMessageEXT(GetInstance(), flags, objectType, object, location, messageCode, pLayerPrefix, pMessage);
}

VulkanResult vkDebugMarkerSetObjectTagEXT(const VkDebugMarkerObjectTagInfoEXT* pTagInfo)
{
  return GetFunctionTable().vkDebugMarkerSetObjectTagEXT(GetDevice(), pTagInfo);
}

VulkanResult vkDebugMarkerSetObjectNameEXT(const VkDebugMarkerObjectNameInfoEXT* pNameInfo)
{
  return GetFunctionTable().vkDebugMarkerSetObjectNameEXT(GetDevice(), pNameInfo);
}

VulkanResult vkCreateCuModuleNVX(const VkCuModuleCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuModuleNVX* pModule)
{
  return GetFunctionTable().vkCreateCuModuleNVX(GetDevice(), pCreateInfo, pAllocator, pModule);
}

VulkanResult vkCreateCuFunctionNVX(const VkCuFunctionCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuFunctionNVX* pFunction)
{
  return GetFunctionTable().vkCreateCuFunctionNVX(GetDevice(), pCreateInfo, pAllocator, pFunction);
}

void vkDestroyCuModuleNVX(VkCuModuleNVX module, const VkAllocationCallbacks* pAllocator)
{
  return GetFunctionTable().vkDestroyCuModuleNVX(GetDevice(), module, pAllocator);
}

void vkDestroyCuFunctionNVX(VkCuFunctionNVX function, const VkAllocationCallbacks* pAllocator)
{
  return GetFunctionTable().vkDestroyCuFunctionNVX(GetDevice(), function, pAllocator);
}

VulkanResultWithValue<uint32_t> vkGetImageViewHandleNVX(const VkImageViewHandleInfoNVX* pInfo)
{
  return GetFunctionTable().vkGetImageViewHandleNVX(GetDevice(), pInfo);
}

VulkanResult vkGetImageViewAddressNVX(VkImageView imageView, VkImageViewAddressPropertiesNVX* pProperties)
{
  return GetFunctionTable().vkGetImageViewAddressNVX(GetDevice(), imageView, pProperties);
}

VulkanResult vkGetShaderInfoAMD(VkPipeline pipeline, VkShaderStageFlagBits shaderStage, VkShaderInfoTypeAMD infoType, size_t* pInfoSize, void* pInfo)
{
  return GetFunctionTable().vkGetShaderInfoAMD(GetDevice(), pipeline, shaderStage, infoType, pInfoSize, pInfo);
}

VulkanResult vkGetPhysicalDeviceExternalImageFormatPropertiesNV(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkExternalMemoryHandleTypeFlagsNV externalHandleType, VkExternalImageFormatPropertiesNV* pExternalImageFormatProperties)
{
  return GetFunctionTable().vkGetPhysicalDeviceExternalImageFormatPropertiesNV(physicalDevice, format, type, tiling, usage, flags, externalHandleType, pExternalImageFormatProperties);
}

VulkanResult vkReleaseDisplayEXT(VkPhysicalDevice physicalDevice, VkDisplayKHR display)
{
  return GetFunctionTable().vkReleaseDisplayEXT(physicalDevice, display);
}

VulkanResult vkGetPhysicalDeviceSurfaceCapabilities2EXT(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilities2EXT* pSurfaceCapabilities)
{
  return GetFunctionTable().vkGetPhysicalDeviceSurfaceCapabilities2EXT(physicalDevice, surface, pSurfaceCapabilities);
}

VulkanResult vkDisplayPowerControlEXT(VkDisplayKHR display, const VkDisplayPowerInfoEXT* pDisplayPowerInfo)
{
  return GetFunctionTable().vkDisplayPowerControlEXT(GetDevice(), display, pDisplayPowerInfo);
}

VulkanResult vkRegisterDeviceEventEXT(const VkDeviceEventInfoEXT* pDeviceEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence)
{
  return GetFunctionTable().vkRegisterDeviceEventEXT(GetDevice(), pDeviceEventInfo, pAllocator, pFence);
}

VulkanResult vkRegisterDisplayEventEXT(VkDisplayKHR display, const VkDisplayEventInfoEXT* pDisplayEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence)
{
  return GetFunctionTable().vkRegisterDisplayEventEXT(GetDevice(), display, pDisplayEventInfo, pAllocator, pFence);
}

VulkanResult vkGetSwapchainCounterEXT(VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue)
{
  return GetFunctionTable().vkGetSwapchainCounterEXT(GetDevice(), swapchain, counter, pCounterValue);
}

VulkanResult vkGetRefreshCycleDurationGOOGLE(VkSwapchainKHR swapchain, VkRefreshCycleDurationGOOGLE* pDisplayTimingProperties)
{
  return GetFunctionTable().vkGetRefreshCycleDurationGOOGLE(GetDevice(), swapchain, pDisplayTimingProperties);
}

VulkanResult vkGetPastPresentationTimingGOOGLE(VkSwapchainKHR swapchain, uint32_t* pPresentationTimingCount, VkPastPresentationTimingGOOGLE* pPresentationTimings)
{
  return GetFunctionTable().vkGetPastPresentationTimingGOOGLE(GetDevice(), swapchain, pPresentationTimingCount, pPresentationTimings);
}

void vkSetHdrMetadataEXT(uint32_t swapchainCount, const VkSwapchainKHR* pSwapchains, const VkHdrMetadataEXT* pMetadata)
{
  return GetFunctionTable().vkSetHdrMetadataEXT(GetDevice(), swapchainCount, pSwapchains, pMetadata);
}

VulkanResult vkSetDebugUtilsObjectNameEXT(const VkDebugUtilsObjectNameInfoEXT* pNameInfo)
{
  return GetFunctionTable().vkSetDebugUtilsObjectNameEXT(GetDevice(), pNameInfo);
}

VulkanResult vkSetDebugUtilsObjectTagEXT(const VkDebugUtilsObjectTagInfoEXT* pTagInfo)
{
  return GetFunctionTable().vkSetDebugUtilsObjectTagEXT(GetDevice(), pTagInfo);
}

void vkQueueBeginDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo)
{
  return GetFunctionTable().vkQueueBeginDebugUtilsLabelEXT(queue, pLabelInfo);
}

void vkQueueEndDebugUtilsLabelEXT(VkQueue queue)
{
  return GetFunctionTable().vkQueueEndDebugUtilsLabelEXT(queue);
}

void vkQueueInsertDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo)
{
  return GetFunctionTable().vkQueueInsertDebugUtilsLabelEXT(queue, pLabelInfo);
}

VulkanResult vkCreateDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger)
{
  return GetFunctionTable().vkCreateDebugUtilsMessengerEXT(GetInstance(), pCreateInfo, pAllocator, pMessenger);
}

void vkDestroyDebugUtilsMessengerEXT(VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks* pAllocator)
{
  return GetFunctionTable().vkDestroyDebugUtilsMessengerEXT(GetInstance(), messenger, pAllocator);
}

void vkSubmitDebugUtilsMessageEXT(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData)
{
  return GetFunctionTable().vkSubmitDebugUtilsMessageEXT(GetInstance(), messageSeverity, messageTypes, pCallbackData);
}

void vkGetPhysicalDeviceMultisamplePropertiesEXT(VkPhysicalDevice physicalDevice, VkSampleCountFlagBits samples, VkMultisamplePropertiesEXT* pMultisampleProperties)
{
  return GetFunctionTable().vkGetPhysicalDeviceMultisamplePropertiesEXT(physicalDevice, samples, pMultisampleProperties);
}

VulkanResult vkGetImageDrmFormatModifierPropertiesEXT(VkImage image, VkImageDrmFormatModifierPropertiesEXT* pProperties)
{
  return GetFunctionTable().vkGetImageDrmFormatModifierPropertiesEXT(GetDevice(), image, pProperties);
}

VulkanResult vkCreateValidationCacheEXT(const VkValidationCacheCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkValidationCacheEXT* pValidationCache)
{
  return GetFunctionTable().vkCreateValidationCacheEXT(GetDevice(), pCreateInfo, pAllocator, pValidationCache);
}

void vkDestroyValidationCacheEXT(VkValidationCacheEXT validationCache, const VkAllocationCallbacks* pAllocator)
{
  return GetFunctionTable().vkDestroyValidationCacheEXT(GetDevice(), validationCache, pAllocator);
}

VulkanResult vkMergeValidationCachesEXT(VkValidationCacheEXT dstCache, uint32_t srcCacheCount, const VkValidationCacheEXT* pSrcCaches)
{
  return GetFunctionTable().vkMergeValidationCachesEXT(GetDevice(), dstCache, srcCacheCount, pSrcCaches);
}

VulkanResult vkGetValidationCacheDataEXT(VkValidationCacheEXT validationCache, size_t* pDataSize, void* pData)
{
  return GetFunctionTable().vkGetValidationCacheDataEXT(GetDevice(), validationCache, pDataSize, pData);
}

VulkanResult vkCreateAccelerationStructureNV(const VkAccelerationStructureCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkAccelerationStructureNV* pAccelerationStructure)
{
  return GetFunctionTable().vkCreateAccelerationStructureNV(GetDevice(), pCreateInfo, pAllocator, pAccelerationStructure);
}

void vkDestroyAccelerationStructureNV(VkAccelerationStructureNV accelerationStructure, const VkAllocationCallbacks* pAllocator)
{
  return GetFunctionTable().vkDestroyAccelerationStructureNV(GetDevice(), accelerationStructure, pAllocator);
}

void vkGetAccelerationStructureMemoryRequirementsNV(const VkAccelerationStructureMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2KHR* pMemoryRequirements)
{
  return GetFunctionTable().vkGetAccelerationStructureMemoryRequirementsNV(GetDevice(), pInfo, pMemoryRequirements);
}

VulkanResult vkBindAccelerationStructureMemoryNV(uint32_t bindInfoCount, const VkBindAccelerationStructureMemoryInfoNV* pBindInfos)
{
  return GetFunctionTable().vkBindAccelerationStructureMemoryNV(GetDevice(), bindInfoCount, pBindInfos);
}

VulkanResult vkCreateRayTracingPipelinesNV(VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoNV* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
{
  return GetFunctionTable().vkCreateRayTracingPipelinesNV(GetDevice(), pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}

VulkanResult vkGetRayTracingShaderGroupHandlesKHR(VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData)
{
  return GetFunctionTable().vkGetRayTracingShaderGroupHandlesKHR(GetDevice(), pipeline, firstGroup, groupCount, dataSize, pData);
}

VulkanResult vkGetRayTracingShaderGroupHandlesNV(VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData)
{
  return GetFunctionTable().vkGetRayTracingShaderGroupHandlesNV(GetDevice(), pipeline, firstGroup, groupCount, dataSize, pData);
}

VulkanResult vkGetAccelerationStructureHandleNV(VkAccelerationStructureNV accelerationStructure, size_t dataSize, void* pData)
{
  return GetFunctionTable().vkGetAccelerationStructureHandleNV(GetDevice(), accelerationStructure, dataSize, pData);
}

VulkanResult vkCompileDeferredNV(VkPipeline pipeline, uint32_t shader)
{
  return GetFunctionTable().vkCompileDeferredNV(GetDevice(), pipeline, shader);
}

VulkanResult vkGetMemoryHostPointerPropertiesEXT(VkExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties)
{
  return GetFunctionTable().vkGetMemoryHostPointerPropertiesEXT(GetDevice(), handleType, pHostPointer, pMemoryHostPointerProperties);
}

VulkanResultWithValue<std::vector<VkTimeDomainEXT>> vkGetPhysicalDeviceCalibrateableTimeDomainsEXT(VkPhysicalDevice physicalDevice)
{
  const auto& fp = GetFunctionTable().vkGetPhysicalDeviceCalibrateableTimeDomainsEXT;
  uint32_t count = 0;
  fp(physicalDevice, &count, nullptr);
  VulkanResultWithValue<std::vector<VkTimeDomainEXT>> result;
  result.value.resize(count);
  result.result = fp(physicalDevice, &count, result.value.data());
  return result;
}

VulkanResult vkGetCalibratedTimestampsEXT(uint32_t timestampCount, const VkCalibratedTimestampInfoEXT* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation)
{
  return GetFunctionTable().vkGetCalibratedTimestampsEXT(GetDevice(), timestampCount, pTimestampInfos, pTimestamps, pMaxDeviation);
}

VulkanResultWithValue<std::vector<VkCheckpointDataNV>> vkGetQueueCheckpointDataNV(VkQueue queue)
{
  const auto& fp = GetFunctionTable().vkGetQueueCheckpointDataNV;
  uint32_t count = 0;
  fp(queue, &count, nullptr);
  VulkanResultWithValue<std::vector<VkCheckpointDataNV>> result;
  result.value.resize(count);
  fp(queue, &count, result.value.data());
  return result;
}

VulkanResult vkInitializePerformanceApiINTEL(const VkInitializePerformanceApiInfoINTEL* pInitializeInfo)
{
  return GetFunctionTable().vkInitializePerformanceApiINTEL(GetDevice(), pInitializeInfo);
}

void vkUninitializePerformanceApiINTEL()
{
  return GetFunctionTable().vkUninitializePerformanceApiINTEL(GetDevice());
}

VulkanResult vkAcquirePerformanceConfigurationINTEL(const VkPerformanceConfigurationAcquireInfoINTEL* pAcquireInfo, VkPerformanceConfigurationINTEL* pConfiguration)
{
  return GetFunctionTable().vkAcquirePerformanceConfigurationINTEL(GetDevice(), pAcquireInfo, pConfiguration);
}

VulkanResult vkReleasePerformanceConfigurationINTEL(VkPerformanceConfigurationINTEL configuration)
{
  return GetFunctionTable().vkReleasePerformanceConfigurationINTEL(GetDevice(), configuration);
}

VulkanResult vkQueueSetPerformanceConfigurationINTEL(VkQueue queue, VkPerformanceConfigurationINTEL configuration)
{
  return GetFunctionTable().vkQueueSetPerformanceConfigurationINTEL(queue, configuration);
}

VulkanResult vkGetPerformanceParameterINTEL(VkPerformanceParameterTypeINTEL parameter, VkPerformanceValueINTEL* pValue)
{
  return GetFunctionTable().vkGetPerformanceParameterINTEL(GetDevice(), parameter, pValue);
}

void vkSetLocalDimmingAMD(VkSwapchainKHR swapChain, VkBool32 localDimmingEnable)
{
  return GetFunctionTable().vkSetLocalDimmingAMD(GetDevice(), swapChain, localDimmingEnable);
}

VulkanResultWithValue<VkDeviceAddress> vkGetBufferDeviceAddressEXT(const VkBufferDeviceAddressInfo* pInfo)
{
  return GetFunctionTable().vkGetBufferDeviceAddressEXT(GetDevice(), pInfo);
}

VulkanResultWithValue<std::vector<VkPhysicalDeviceToolProperties>> vkGetPhysicalDeviceToolPropertiesEXT(VkPhysicalDevice physicalDevice)
{
  const auto& fp = GetFunctionTable().vkGetPhysicalDeviceToolPropertiesEXT;
  uint32_t count = 0;
  fp(physicalDevice, &count, nullptr);
  VulkanResultWithValue<std::vector<VkPhysicalDeviceToolProperties>> result;
  result.value.resize(count);
  result.result = fp(physicalDevice, &count, result.value.data());
  return result;
}

VulkanResultWithValue<std::vector<VkCooperativeMatrixPropertiesNV>> vkGetPhysicalDeviceCooperativeMatrixPropertiesNV(VkPhysicalDevice physicalDevice)
{
  const auto& fp = GetFunctionTable().vkGetPhysicalDeviceCooperativeMatrixPropertiesNV;
  uint32_t count = 0;
  fp(physicalDevice, &count, nullptr);
  VulkanResultWithValue<std::vector<VkCooperativeMatrixPropertiesNV>> result;
  result.value.resize(count);
  result.result = fp(physicalDevice, &count, result.value.data());
  return result;
}

VulkanResultWithValue<std::vector<VkFramebufferMixedSamplesCombinationNV>> vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(VkPhysicalDevice physicalDevice)
{
  const auto& fp = GetFunctionTable().vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV;
  uint32_t count = 0;
  fp(physicalDevice, &count, nullptr);
  VulkanResultWithValue<std::vector<VkFramebufferMixedSamplesCombinationNV>> result;
  result.value.resize(count);
  result.result = fp(physicalDevice, &count, result.value.data());
  return result;
}

VulkanResult vkCreateHeadlessSurfaceEXT(const VkHeadlessSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
  return GetFunctionTable().vkCreateHeadlessSurfaceEXT(GetInstance(), pCreateInfo, pAllocator, pSurface);
}

void vkResetQueryPoolEXT(VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount)
{
  return GetFunctionTable().vkResetQueryPoolEXT(GetDevice(), queryPool, firstQuery, queryCount);
}

void vkGetGeneratedCommandsMemoryRequirementsNV(const VkGeneratedCommandsMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
  return GetFunctionTable().vkGetGeneratedCommandsMemoryRequirementsNV(GetDevice(), pInfo, pMemoryRequirements);
}

VulkanResult vkCreateIndirectCommandsLayoutNV(const VkIndirectCommandsLayoutCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectCommandsLayoutNV* pIndirectCommandsLayout)
{
  return GetFunctionTable().vkCreateIndirectCommandsLayoutNV(GetDevice(), pCreateInfo, pAllocator, pIndirectCommandsLayout);
}

void vkDestroyIndirectCommandsLayoutNV(VkIndirectCommandsLayoutNV indirectCommandsLayout, const VkAllocationCallbacks* pAllocator)
{
  return GetFunctionTable().vkDestroyIndirectCommandsLayoutNV(GetDevice(), indirectCommandsLayout, pAllocator);
}

VulkanResult vkAcquireDrmDisplayEXT(VkPhysicalDevice physicalDevice, int32_t drmFd, VkDisplayKHR display)
{
  return GetFunctionTable().vkAcquireDrmDisplayEXT(physicalDevice, drmFd, display);
}

VulkanResult vkGetDrmDisplayEXT(VkPhysicalDevice physicalDevice, int32_t drmFd, uint32_t connectorId, VkDisplayKHR* display)
{
  return GetFunctionTable().vkGetDrmDisplayEXT(physicalDevice, drmFd, connectorId, display);
}

VulkanResult vkCreatePrivateDataSlotEXT(const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot)
{
  return GetFunctionTable().vkCreatePrivateDataSlotEXT(GetDevice(), pCreateInfo, pAllocator, pPrivateDataSlot);
}

void vkDestroyPrivateDataSlotEXT(VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator)
{
  return GetFunctionTable().vkDestroyPrivateDataSlotEXT(GetDevice(), privateDataSlot, pAllocator);
}

VulkanResult vkSetPrivateDataEXT(VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data)
{
  return GetFunctionTable().vkSetPrivateDataEXT(GetDevice(), objectType, objectHandle, privateDataSlot, data);
}

void vkGetPrivateDataEXT(VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData)
{
  return GetFunctionTable().vkGetPrivateDataEXT(GetDevice(), objectType, objectHandle, privateDataSlot, pData);
}

VulkanResult vkAcquireWinrtDisplayNV(VkPhysicalDevice physicalDevice, VkDisplayKHR display)
{
  return GetFunctionTable().vkAcquireWinrtDisplayNV(physicalDevice, display);
}

VulkanResult vkGetWinrtDisplayNV(VkPhysicalDevice physicalDevice, uint32_t deviceRelativeId, VkDisplayKHR* pDisplay)
{
  return GetFunctionTable().vkGetWinrtDisplayNV(physicalDevice, deviceRelativeId, pDisplay);
}

VulkanResult vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI(VkRenderPass renderpass, VkExtent2D* pMaxWorkgroupSize)
{
  return GetFunctionTable().vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI(GetDevice(), renderpass, pMaxWorkgroupSize);
}

VulkanResult vkGetMemoryRemoteAddressNV(const VkMemoryGetRemoteAddressInfoNV* pMemoryGetRemoteAddressInfo, VkRemoteAddressNV* pAddress)
{
  return GetFunctionTable().vkGetMemoryRemoteAddressNV(GetDevice(), pMemoryGetRemoteAddressInfo, pAddress);
}

void vkSetDeviceMemoryPriorityEXT(VkDeviceMemory memory, float priority)
{
  return GetFunctionTable().vkSetDeviceMemoryPriorityEXT(GetDevice(), memory, priority);
}

void vkGetDescriptorSetLayoutHostMappingInfoVALVE(const VkDescriptorSetBindingReferenceVALVE* pBindingReference, VkDescriptorSetLayoutHostMappingInfoVALVE* pHostMapping)
{
  return GetFunctionTable().vkGetDescriptorSetLayoutHostMappingInfoVALVE(GetDevice(), pBindingReference, pHostMapping);
}

void vkGetDescriptorSetHostMappingVALVE(VkDescriptorSet descriptorSet, void** ppData)
{
  return GetFunctionTable().vkGetDescriptorSetHostMappingVALVE(GetDevice(), descriptorSet, ppData);
}

VulkanResult vkCreateAccelerationStructureKHR(const VkAccelerationStructureCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkAccelerationStructureKHR* pAccelerationStructure)
{
  return GetFunctionTable().vkCreateAccelerationStructureKHR(GetDevice(), pCreateInfo, pAllocator, pAccelerationStructure);
}

void vkDestroyAccelerationStructureKHR(VkAccelerationStructureKHR accelerationStructure, const VkAllocationCallbacks* pAllocator)
{
  return GetFunctionTable().vkDestroyAccelerationStructureKHR(GetDevice(), accelerationStructure, pAllocator);
}

VulkanResult vkBuildAccelerationStructuresKHR(VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos)
{
  return GetFunctionTable().vkBuildAccelerationStructuresKHR(GetDevice(), deferredOperation, infoCount, pInfos, ppBuildRangeInfos);
}

VulkanResult vkCopyAccelerationStructureKHR(VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureInfoKHR* pInfo)
{
  return GetFunctionTable().vkCopyAccelerationStructureKHR(GetDevice(), deferredOperation, pInfo);
}

VulkanResult vkCopyAccelerationStructureToMemoryKHR(VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo)
{
  return GetFunctionTable().vkCopyAccelerationStructureToMemoryKHR(GetDevice(), deferredOperation, pInfo);
}

VulkanResult vkCopyMemoryToAccelerationStructureKHR(VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo)
{
  return GetFunctionTable().vkCopyMemoryToAccelerationStructureKHR(GetDevice(), deferredOperation, pInfo);
}

VulkanResult vkWriteAccelerationStructuresPropertiesKHR(uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, size_t dataSize, void* pData, size_t stride)
{
  return GetFunctionTable().vkWriteAccelerationStructuresPropertiesKHR(GetDevice(), accelerationStructureCount, pAccelerationStructures, queryType, dataSize, pData, stride);
}

VulkanResultWithValue<VkDeviceAddress> vkGetAccelerationStructureDeviceAddressKHR(const VkAccelerationStructureDeviceAddressInfoKHR* pInfo)
{
  return GetFunctionTable().vkGetAccelerationStructureDeviceAddressKHR(GetDevice(), pInfo);
}

void vkGetDeviceAccelerationStructureCompatibilityKHR(const VkAccelerationStructureVersionInfoKHR* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility)
{
  return GetFunctionTable().vkGetDeviceAccelerationStructureCompatibilityKHR(GetDevice(), pVersionInfo, pCompatibility);
}

void vkGetAccelerationStructureBuildSizesKHR(VkAccelerationStructureBuildTypeKHR buildType, const VkAccelerationStructureBuildGeometryInfoKHR* pBuildInfo, const uint32_t* pMaxPrimitiveCounts, VkAccelerationStructureBuildSizesInfoKHR* pSizeInfo)
{
  return GetFunctionTable().vkGetAccelerationStructureBuildSizesKHR(GetDevice(), buildType, pBuildInfo, pMaxPrimitiveCounts, pSizeInfo);
}

VulkanResult vkCreateRayTracingPipelinesKHR(VkDeferredOperationKHR deferredOperation, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
{
  return GetFunctionTable().vkCreateRayTracingPipelinesKHR(GetDevice(), deferredOperation, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}

VulkanResult vkGetRayTracingCaptureReplayShaderGroupHandlesKHR(VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData)
{
  return GetFunctionTable().vkGetRayTracingCaptureReplayShaderGroupHandlesKHR(GetDevice(), pipeline, firstGroup, groupCount, dataSize, pData);
}

VulkanResultWithValue<VkDeviceSize> vkGetRayTracingShaderGroupStackSizeKHR(VkPipeline pipeline, uint32_t group, VkShaderGroupShaderKHR groupShader)
{
  return GetFunctionTable().vkGetRayTracingShaderGroupStackSizeKHR(GetDevice(), pipeline, group, groupShader);
}

VulkanCommandRecorder::VulkanCommandRecorder(VkCommandBuffer command_buffer) : _command_buffer(command_buffer) {}

VulkanResult VulkanCommandRecorder::Begin(const VkCommandBufferBeginInfo* pBeginInfo) const noexcept
{
 return GetFunctionTable().vkBeginCommandBuffer(_command_buffer, pBeginInfo);
}

VulkanResult VulkanCommandRecorder::End() const noexcept
{
 return GetFunctionTable().vkEndCommandBuffer(_command_buffer);
}

VulkanResult VulkanCommandRecorder::Reset(VkCommandBufferResetFlags flags) const noexcept
{
 return GetFunctionTable().vkResetCommandBuffer(_command_buffer, flags);
}

void VulkanCommandRecorder::BindPipeline(VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline) const noexcept
{
  GetFunctionTable().vkCmdBindPipeline(_command_buffer, pipelineBindPoint, pipeline);
}

void VulkanCommandRecorder::SetViewport(uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports) const noexcept
{
  GetFunctionTable().vkCmdSetViewport(_command_buffer, firstViewport, viewportCount, pViewports);
}

void VulkanCommandRecorder::SetScissor(uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors) const noexcept
{
  GetFunctionTable().vkCmdSetScissor(_command_buffer, firstScissor, scissorCount, pScissors);
}

void VulkanCommandRecorder::SetLineWidth(float lineWidth) const noexcept
{
  GetFunctionTable().vkCmdSetLineWidth(_command_buffer, lineWidth);
}

void VulkanCommandRecorder::SetDepthBias(float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) const noexcept
{
  GetFunctionTable().vkCmdSetDepthBias(_command_buffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
}

void VulkanCommandRecorder::SetBlendConstants(const float blendConstants[4]) const noexcept
{
  GetFunctionTable().vkCmdSetBlendConstants(_command_buffer, blendConstants);
}

void VulkanCommandRecorder::SetDepthBounds(float minDepthBounds, float maxDepthBounds) const noexcept
{
  GetFunctionTable().vkCmdSetDepthBounds(_command_buffer, minDepthBounds, maxDepthBounds);
}

void VulkanCommandRecorder::SetStencilCompareMask(VkStencilFaceFlags faceMask, uint32_t compareMask) const noexcept
{
  GetFunctionTable().vkCmdSetStencilCompareMask(_command_buffer, faceMask, compareMask);
}

void VulkanCommandRecorder::SetStencilWriteMask(VkStencilFaceFlags faceMask, uint32_t writeMask) const noexcept
{
  GetFunctionTable().vkCmdSetStencilWriteMask(_command_buffer, faceMask, writeMask);
}

void VulkanCommandRecorder::SetStencilReference(VkStencilFaceFlags faceMask, uint32_t reference) const noexcept
{
  GetFunctionTable().vkCmdSetStencilReference(_command_buffer, faceMask, reference);
}

void VulkanCommandRecorder::BindDescriptorSets(VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets) const noexcept
{
  GetFunctionTable().vkCmdBindDescriptorSets(_command_buffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
}

void VulkanCommandRecorder::BindIndexBuffer(VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) const noexcept
{
  GetFunctionTable().vkCmdBindIndexBuffer(_command_buffer, buffer, offset, indexType);
}

void VulkanCommandRecorder::BindVertexBuffers(uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets) const noexcept
{
  GetFunctionTable().vkCmdBindVertexBuffers(_command_buffer, firstBinding, bindingCount, pBuffers, pOffsets);
}

void VulkanCommandRecorder::Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) const noexcept
{
  GetFunctionTable().vkCmdDraw(_command_buffer, vertexCount, instanceCount, firstVertex, firstInstance);
}

void VulkanCommandRecorder::DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) const noexcept
{
  GetFunctionTable().vkCmdDrawIndexed(_command_buffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

void VulkanCommandRecorder::DrawIndirect(VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) const noexcept
{
  GetFunctionTable().vkCmdDrawIndirect(_command_buffer, buffer, offset, drawCount, stride);
}

void VulkanCommandRecorder::DrawIndexedIndirect(VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) const noexcept
{
  GetFunctionTable().vkCmdDrawIndexedIndirect(_command_buffer, buffer, offset, drawCount, stride);
}

void VulkanCommandRecorder::Dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const noexcept
{
  GetFunctionTable().vkCmdDispatch(_command_buffer, groupCountX, groupCountY, groupCountZ);
}

void VulkanCommandRecorder::DispatchIndirect(VkBuffer buffer, VkDeviceSize offset) const noexcept
{
  GetFunctionTable().vkCmdDispatchIndirect(_command_buffer, buffer, offset);
}

void VulkanCommandRecorder::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions) const noexcept
{
  GetFunctionTable().vkCmdCopyBuffer(_command_buffer, srcBuffer, dstBuffer, regionCount, pRegions);
}

void VulkanCommandRecorder::CopyImage(VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions) const noexcept
{
  GetFunctionTable().vkCmdCopyImage(_command_buffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}

void VulkanCommandRecorder::BlitImage(VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter) const noexcept
{
  GetFunctionTable().vkCmdBlitImage(_command_buffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
}

void VulkanCommandRecorder::CopyBufferToImage(VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions) const noexcept
{
  GetFunctionTable().vkCmdCopyBufferToImage(_command_buffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
}

void VulkanCommandRecorder::CopyImageToBuffer(VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions) const noexcept
{
  GetFunctionTable().vkCmdCopyImageToBuffer(_command_buffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
}

void VulkanCommandRecorder::UpdateBuffer(VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData) const noexcept
{
  GetFunctionTable().vkCmdUpdateBuffer(_command_buffer, dstBuffer, dstOffset, dataSize, pData);
}

void VulkanCommandRecorder::FillBuffer(VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data) const noexcept
{
  GetFunctionTable().vkCmdFillBuffer(_command_buffer, dstBuffer, dstOffset, size, data);
}

void VulkanCommandRecorder::ClearColorImage(VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) const noexcept
{
  GetFunctionTable().vkCmdClearColorImage(_command_buffer, image, imageLayout, pColor, rangeCount, pRanges);
}

void VulkanCommandRecorder::ClearDepthStencilImage(VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) const noexcept
{
  GetFunctionTable().vkCmdClearDepthStencilImage(_command_buffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
}

void VulkanCommandRecorder::ClearAttachments(uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects) const noexcept
{
  GetFunctionTable().vkCmdClearAttachments(_command_buffer, attachmentCount, pAttachments, rectCount, pRects);
}

void VulkanCommandRecorder::ResolveImage(VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions) const noexcept
{
  GetFunctionTable().vkCmdResolveImage(_command_buffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}

void VulkanCommandRecorder::SetEvent(VkEvent event, VkPipelineStageFlags stageMask) const noexcept
{
  GetFunctionTable().vkCmdSetEvent(_command_buffer, event, stageMask);
}

void VulkanCommandRecorder::ResetEvent(VkEvent event, VkPipelineStageFlags stageMask) const noexcept
{
  GetFunctionTable().vkCmdResetEvent(_command_buffer, event, stageMask);
}

void VulkanCommandRecorder::WaitEvents(uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) const noexcept
{
  GetFunctionTable().vkCmdWaitEvents(_command_buffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}

void VulkanCommandRecorder::PipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) const noexcept
{
  GetFunctionTable().vkCmdPipelineBarrier(_command_buffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}

void VulkanCommandRecorder::BeginQuery(VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags) const noexcept
{
  GetFunctionTable().vkCmdBeginQuery(_command_buffer, queryPool, query, flags);
}

void VulkanCommandRecorder::EndQuery(VkQueryPool queryPool, uint32_t query) const noexcept
{
  GetFunctionTable().vkCmdEndQuery(_command_buffer, queryPool, query);
}

void VulkanCommandRecorder::ResetQueryPool(VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) const noexcept
{
  GetFunctionTable().vkCmdResetQueryPool(_command_buffer, queryPool, firstQuery, queryCount);
}

void VulkanCommandRecorder::WriteTimestamp(VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query) const noexcept
{
  GetFunctionTable().vkCmdWriteTimestamp(_command_buffer, pipelineStage, queryPool, query);
}

void VulkanCommandRecorder::CopyQueryPoolResults(VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags) const noexcept
{
  GetFunctionTable().vkCmdCopyQueryPoolResults(_command_buffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
}

void VulkanCommandRecorder::PushConstants(VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues) const noexcept
{
  GetFunctionTable().vkCmdPushConstants(_command_buffer, layout, stageFlags, offset, size, pValues);
}

void VulkanCommandRecorder::BeginRenderPass(const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents) const noexcept
{
  GetFunctionTable().vkCmdBeginRenderPass(_command_buffer, pRenderPassBegin, contents);
}

void VulkanCommandRecorder::NextSubpass(VkSubpassContents contents) const noexcept
{
  GetFunctionTable().vkCmdNextSubpass(_command_buffer, contents);
}

void VulkanCommandRecorder::EndRenderPass() const noexcept
{
  GetFunctionTable().vkCmdEndRenderPass(_command_buffer);
}

void VulkanCommandRecorder::ExecuteCommands(uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) const noexcept
{
  GetFunctionTable().vkCmdExecuteCommands(_command_buffer, commandBufferCount, pCommandBuffers);
}

void VulkanCommandRecorder::SetDeviceMask(uint32_t deviceMask) const noexcept
{
  GetFunctionTable().vkCmdSetDeviceMask(_command_buffer, deviceMask);
}

void VulkanCommandRecorder::DispatchBase(uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const noexcept
{
  GetFunctionTable().vkCmdDispatchBase(_command_buffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
}

void VulkanCommandRecorder::DrawIndirectCount(VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const noexcept
{
  GetFunctionTable().vkCmdDrawIndirectCount(_command_buffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void VulkanCommandRecorder::DrawIndexedIndirectCount(VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const noexcept
{
  GetFunctionTable().vkCmdDrawIndexedIndirectCount(_command_buffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void VulkanCommandRecorder::BeginRenderPass2(const VkRenderPassBeginInfo* pRenderPassBegin, const VkSubpassBeginInfo* pSubpassBeginInfo) const noexcept
{
  GetFunctionTable().vkCmdBeginRenderPass2(_command_buffer, pRenderPassBegin, pSubpassBeginInfo);
}

void VulkanCommandRecorder::NextSubpass2(const VkSubpassBeginInfo* pSubpassBeginInfo, const VkSubpassEndInfo* pSubpassEndInfo) const noexcept
{
  GetFunctionTable().vkCmdNextSubpass2(_command_buffer, pSubpassBeginInfo, pSubpassEndInfo);
}

void VulkanCommandRecorder::EndRenderPass2(const VkSubpassEndInfo* pSubpassEndInfo) const noexcept
{
  GetFunctionTable().vkCmdEndRenderPass2(_command_buffer, pSubpassEndInfo);
}

void VulkanCommandRecorder::SetEvent2(VkEvent event, const VkDependencyInfo* pDependencyInfo) const noexcept
{
  GetFunctionTable().vkCmdSetEvent2(_command_buffer, event, pDependencyInfo);
}

void VulkanCommandRecorder::ResetEvent2(VkEvent event, VkPipelineStageFlags2 stageMask) const noexcept
{
  GetFunctionTable().vkCmdResetEvent2(_command_buffer, event, stageMask);
}

void VulkanCommandRecorder::WaitEvents2(uint32_t eventCount, const VkEvent* pEvents, const VkDependencyInfo* pDependencyInfos) const noexcept
{
  GetFunctionTable().vkCmdWaitEvents2(_command_buffer, eventCount, pEvents, pDependencyInfos);
}

void VulkanCommandRecorder::PipelineBarrier2(const VkDependencyInfo* pDependencyInfo) const noexcept
{
  GetFunctionTable().vkCmdPipelineBarrier2(_command_buffer, pDependencyInfo);
}

void VulkanCommandRecorder::WriteTimestamp2(VkPipelineStageFlags2 stage, VkQueryPool queryPool, uint32_t query) const noexcept
{
  GetFunctionTable().vkCmdWriteTimestamp2(_command_buffer, stage, queryPool, query);
}

void VulkanCommandRecorder::CopyBuffer2(const VkCopyBufferInfo2* pCopyBufferInfo) const noexcept
{
  GetFunctionTable().vkCmdCopyBuffer2(_command_buffer, pCopyBufferInfo);
}

void VulkanCommandRecorder::CopyImage2(const VkCopyImageInfo2* pCopyImageInfo) const noexcept
{
  GetFunctionTable().vkCmdCopyImage2(_command_buffer, pCopyImageInfo);
}

void VulkanCommandRecorder::CopyBufferToImage2(const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo) const noexcept
{
  GetFunctionTable().vkCmdCopyBufferToImage2(_command_buffer, pCopyBufferToImageInfo);
}

void VulkanCommandRecorder::CopyImageToBuffer2(const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo) const noexcept
{
  GetFunctionTable().vkCmdCopyImageToBuffer2(_command_buffer, pCopyImageToBufferInfo);
}

void VulkanCommandRecorder::BlitImage2(const VkBlitImageInfo2* pBlitImageInfo) const noexcept
{
  GetFunctionTable().vkCmdBlitImage2(_command_buffer, pBlitImageInfo);
}

void VulkanCommandRecorder::ResolveImage2(const VkResolveImageInfo2* pResolveImageInfo) const noexcept
{
  GetFunctionTable().vkCmdResolveImage2(_command_buffer, pResolveImageInfo);
}

void VulkanCommandRecorder::BeginRendering(const VkRenderingInfo* pRenderingInfo) const noexcept
{
  GetFunctionTable().vkCmdBeginRendering(_command_buffer, pRenderingInfo);
}

void VulkanCommandRecorder::EndRendering() const noexcept
{
  GetFunctionTable().vkCmdEndRendering(_command_buffer);
}

void VulkanCommandRecorder::SetCullMode(VkCullModeFlags cullMode) const noexcept
{
  GetFunctionTable().vkCmdSetCullMode(_command_buffer, cullMode);
}

void VulkanCommandRecorder::SetFrontFace(VkFrontFace frontFace) const noexcept
{
  GetFunctionTable().vkCmdSetFrontFace(_command_buffer, frontFace);
}

void VulkanCommandRecorder::SetPrimitiveTopology(VkPrimitiveTopology primitiveTopology) const noexcept
{
  GetFunctionTable().vkCmdSetPrimitiveTopology(_command_buffer, primitiveTopology);
}

void VulkanCommandRecorder::SetViewportWithCount(uint32_t viewportCount, const VkViewport* pViewports) const noexcept
{
  GetFunctionTable().vkCmdSetViewportWithCount(_command_buffer, viewportCount, pViewports);
}

void VulkanCommandRecorder::SetScissorWithCount(uint32_t scissorCount, const VkRect2D* pScissors) const noexcept
{
  GetFunctionTable().vkCmdSetScissorWithCount(_command_buffer, scissorCount, pScissors);
}

void VulkanCommandRecorder::BindVertexBuffers2(uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides) const noexcept
{
  GetFunctionTable().vkCmdBindVertexBuffers2(_command_buffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes, pStrides);
}

void VulkanCommandRecorder::SetDepthTestEnable(VkBool32 depthTestEnable) const noexcept
{
  GetFunctionTable().vkCmdSetDepthTestEnable(_command_buffer, depthTestEnable);
}

void VulkanCommandRecorder::SetDepthWriteEnable(VkBool32 depthWriteEnable) const noexcept
{
  GetFunctionTable().vkCmdSetDepthWriteEnable(_command_buffer, depthWriteEnable);
}

void VulkanCommandRecorder::SetDepthCompareOp(VkCompareOp depthCompareOp) const noexcept
{
  GetFunctionTable().vkCmdSetDepthCompareOp(_command_buffer, depthCompareOp);
}

void VulkanCommandRecorder::SetDepthBoundsTestEnable(VkBool32 depthBoundsTestEnable) const noexcept
{
  GetFunctionTable().vkCmdSetDepthBoundsTestEnable(_command_buffer, depthBoundsTestEnable);
}

void VulkanCommandRecorder::SetStencilTestEnable(VkBool32 stencilTestEnable) const noexcept
{
  GetFunctionTable().vkCmdSetStencilTestEnable(_command_buffer, stencilTestEnable);
}

void VulkanCommandRecorder::SetStencilOp(VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp) const noexcept
{
  GetFunctionTable().vkCmdSetStencilOp(_command_buffer, faceMask, failOp, passOp, depthFailOp, compareOp);
}

void VulkanCommandRecorder::SetRasterizerDiscardEnable(VkBool32 rasterizerDiscardEnable) const noexcept
{
  GetFunctionTable().vkCmdSetRasterizerDiscardEnable(_command_buffer, rasterizerDiscardEnable);
}

void VulkanCommandRecorder::SetDepthBiasEnable(VkBool32 depthBiasEnable) const noexcept
{
  GetFunctionTable().vkCmdSetDepthBiasEnable(_command_buffer, depthBiasEnable);
}

void VulkanCommandRecorder::SetPrimitiveRestartEnable(VkBool32 primitiveRestartEnable) const noexcept
{
  GetFunctionTable().vkCmdSetPrimitiveRestartEnable(_command_buffer, primitiveRestartEnable);
}

void VulkanCommandRecorder::BeginRenderingKHR(const VkRenderingInfo* pRenderingInfo) const noexcept
{
  GetFunctionTable().vkCmdBeginRenderingKHR(_command_buffer, pRenderingInfo);
}

void VulkanCommandRecorder::EndRenderingKHR() const noexcept
{
  GetFunctionTable().vkCmdEndRenderingKHR(_command_buffer);
}

void VulkanCommandRecorder::SetDeviceMaskKHR(uint32_t deviceMask) const noexcept
{
  GetFunctionTable().vkCmdSetDeviceMaskKHR(_command_buffer, deviceMask);
}

void VulkanCommandRecorder::DispatchBaseKHR(uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const noexcept
{
  GetFunctionTable().vkCmdDispatchBaseKHR(_command_buffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
}

void VulkanCommandRecorder::PushDescriptorSetKHR(VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites) const noexcept
{
  GetFunctionTable().vkCmdPushDescriptorSetKHR(_command_buffer, pipelineBindPoint, layout, set, descriptorWriteCount, pDescriptorWrites);
}

void VulkanCommandRecorder::PushDescriptorSetWithTemplateKHR(VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData) const noexcept
{
  GetFunctionTable().vkCmdPushDescriptorSetWithTemplateKHR(_command_buffer, descriptorUpdateTemplate, layout, set, pData);
}

void VulkanCommandRecorder::BeginRenderPass2KHR(const VkRenderPassBeginInfo* pRenderPassBegin, const VkSubpassBeginInfo* pSubpassBeginInfo) const noexcept
{
  GetFunctionTable().vkCmdBeginRenderPass2KHR(_command_buffer, pRenderPassBegin, pSubpassBeginInfo);
}

void VulkanCommandRecorder::NextSubpass2KHR(const VkSubpassBeginInfo* pSubpassBeginInfo, const VkSubpassEndInfo* pSubpassEndInfo) const noexcept
{
  GetFunctionTable().vkCmdNextSubpass2KHR(_command_buffer, pSubpassBeginInfo, pSubpassEndInfo);
}

void VulkanCommandRecorder::EndRenderPass2KHR(const VkSubpassEndInfo* pSubpassEndInfo) const noexcept
{
  GetFunctionTable().vkCmdEndRenderPass2KHR(_command_buffer, pSubpassEndInfo);
}

void VulkanCommandRecorder::DrawIndirectCountKHR(VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const noexcept
{
  GetFunctionTable().vkCmdDrawIndirectCountKHR(_command_buffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void VulkanCommandRecorder::DrawIndexedIndirectCountKHR(VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const noexcept
{
  GetFunctionTable().vkCmdDrawIndexedIndirectCountKHR(_command_buffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void VulkanCommandRecorder::SetFragmentShadingRateKHR(const VkExtent2D* pFragmentSize, const VkFragmentShadingRateCombinerOpKHR combinerOps[2]) const noexcept
{
  GetFunctionTable().vkCmdSetFragmentShadingRateKHR(_command_buffer, pFragmentSize, combinerOps);
}

void VulkanCommandRecorder::SetEvent2KHR(VkEvent event, const VkDependencyInfo* pDependencyInfo) const noexcept
{
  GetFunctionTable().vkCmdSetEvent2KHR(_command_buffer, event, pDependencyInfo);
}

void VulkanCommandRecorder::ResetEvent2KHR(VkEvent event, VkPipelineStageFlags2 stageMask) const noexcept
{
  GetFunctionTable().vkCmdResetEvent2KHR(_command_buffer, event, stageMask);
}

void VulkanCommandRecorder::WaitEvents2KHR(uint32_t eventCount, const VkEvent* pEvents, const VkDependencyInfo* pDependencyInfos) const noexcept
{
  GetFunctionTable().vkCmdWaitEvents2KHR(_command_buffer, eventCount, pEvents, pDependencyInfos);
}

void VulkanCommandRecorder::PipelineBarrier2KHR(const VkDependencyInfo* pDependencyInfo) const noexcept
{
  GetFunctionTable().vkCmdPipelineBarrier2KHR(_command_buffer, pDependencyInfo);
}

void VulkanCommandRecorder::WriteTimestamp2KHR(VkPipelineStageFlags2 stage, VkQueryPool queryPool, uint32_t query) const noexcept
{
  GetFunctionTable().vkCmdWriteTimestamp2KHR(_command_buffer, stage, queryPool, query);
}

void VulkanCommandRecorder::WriteBufferMarker2AMD(VkPipelineStageFlags2 stage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker) const noexcept
{
  GetFunctionTable().vkCmdWriteBufferMarker2AMD(_command_buffer, stage, dstBuffer, dstOffset, marker);
}

void VulkanCommandRecorder::CopyBuffer2KHR(const VkCopyBufferInfo2* pCopyBufferInfo) const noexcept
{
  GetFunctionTable().vkCmdCopyBuffer2KHR(_command_buffer, pCopyBufferInfo);
}

void VulkanCommandRecorder::CopyImage2KHR(const VkCopyImageInfo2* pCopyImageInfo) const noexcept
{
  GetFunctionTable().vkCmdCopyImage2KHR(_command_buffer, pCopyImageInfo);
}

void VulkanCommandRecorder::CopyBufferToImage2KHR(const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo) const noexcept
{
  GetFunctionTable().vkCmdCopyBufferToImage2KHR(_command_buffer, pCopyBufferToImageInfo);
}

void VulkanCommandRecorder::CopyImageToBuffer2KHR(const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo) const noexcept
{
  GetFunctionTable().vkCmdCopyImageToBuffer2KHR(_command_buffer, pCopyImageToBufferInfo);
}

void VulkanCommandRecorder::BlitImage2KHR(const VkBlitImageInfo2* pBlitImageInfo) const noexcept
{
  GetFunctionTable().vkCmdBlitImage2KHR(_command_buffer, pBlitImageInfo);
}

void VulkanCommandRecorder::ResolveImage2KHR(const VkResolveImageInfo2* pResolveImageInfo) const noexcept
{
  GetFunctionTable().vkCmdResolveImage2KHR(_command_buffer, pResolveImageInfo);
}

void VulkanCommandRecorder::DebugMarkerBeginEXT(const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) const noexcept
{
  GetFunctionTable().vkCmdDebugMarkerBeginEXT(_command_buffer, pMarkerInfo);
}

void VulkanCommandRecorder::DebugMarkerEndEXT() const noexcept
{
  GetFunctionTable().vkCmdDebugMarkerEndEXT(_command_buffer);
}

void VulkanCommandRecorder::DebugMarkerInsertEXT(const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) const noexcept
{
  GetFunctionTable().vkCmdDebugMarkerInsertEXT(_command_buffer, pMarkerInfo);
}

void VulkanCommandRecorder::BindTransformFeedbackBuffersEXT(uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes) const noexcept
{
  GetFunctionTable().vkCmdBindTransformFeedbackBuffersEXT(_command_buffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes);
}

void VulkanCommandRecorder::BeginTransformFeedbackEXT(uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) const noexcept
{
  GetFunctionTable().vkCmdBeginTransformFeedbackEXT(_command_buffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
}

void VulkanCommandRecorder::EndTransformFeedbackEXT(uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) const noexcept
{
  GetFunctionTable().vkCmdEndTransformFeedbackEXT(_command_buffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
}

void VulkanCommandRecorder::BeginQueryIndexedEXT(VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index) const noexcept
{
  GetFunctionTable().vkCmdBeginQueryIndexedEXT(_command_buffer, queryPool, query, flags, index);
}

void VulkanCommandRecorder::EndQueryIndexedEXT(VkQueryPool queryPool, uint32_t query, uint32_t index) const noexcept
{
  GetFunctionTable().vkCmdEndQueryIndexedEXT(_command_buffer, queryPool, query, index);
}

void VulkanCommandRecorder::DrawIndirectByteCountEXT(uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride) const noexcept
{
  GetFunctionTable().vkCmdDrawIndirectByteCountEXT(_command_buffer, instanceCount, firstInstance, counterBuffer, counterBufferOffset, counterOffset, vertexStride);
}

void VulkanCommandRecorder::CuLaunchKernelNVX(const VkCuLaunchInfoNVX* pLaunchInfo) const noexcept
{
  GetFunctionTable().vkCmdCuLaunchKernelNVX(_command_buffer, pLaunchInfo);
}

void VulkanCommandRecorder::DrawIndirectCountAMD(VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const noexcept
{
  GetFunctionTable().vkCmdDrawIndirectCountAMD(_command_buffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void VulkanCommandRecorder::DrawIndexedIndirectCountAMD(VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const noexcept
{
  GetFunctionTable().vkCmdDrawIndexedIndirectCountAMD(_command_buffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void VulkanCommandRecorder::BeginConditionalRenderingEXT(const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin) const noexcept
{
  GetFunctionTable().vkCmdBeginConditionalRenderingEXT(_command_buffer, pConditionalRenderingBegin);
}

void VulkanCommandRecorder::EndConditionalRenderingEXT() const noexcept
{
  GetFunctionTable().vkCmdEndConditionalRenderingEXT(_command_buffer);
}

void VulkanCommandRecorder::SetViewportWScalingNV(uint32_t firstViewport, uint32_t viewportCount, const VkViewportWScalingNV* pViewportWScalings) const noexcept
{
  GetFunctionTable().vkCmdSetViewportWScalingNV(_command_buffer, firstViewport, viewportCount, pViewportWScalings);
}

void VulkanCommandRecorder::SetDiscardRectangleEXT(uint32_t firstDiscardRectangle, uint32_t discardRectangleCount, const VkRect2D* pDiscardRectangles) const noexcept
{
  GetFunctionTable().vkCmdSetDiscardRectangleEXT(_command_buffer, firstDiscardRectangle, discardRectangleCount, pDiscardRectangles);
}

void VulkanCommandRecorder::BeginDebugUtilsLabelEXT(const VkDebugUtilsLabelEXT* pLabelInfo) const noexcept
{
  GetFunctionTable().vkCmdBeginDebugUtilsLabelEXT(_command_buffer, pLabelInfo);
}

void VulkanCommandRecorder::EndDebugUtilsLabelEXT() const noexcept
{
  GetFunctionTable().vkCmdEndDebugUtilsLabelEXT(_command_buffer);
}

void VulkanCommandRecorder::InsertDebugUtilsLabelEXT(const VkDebugUtilsLabelEXT* pLabelInfo) const noexcept
{
  GetFunctionTable().vkCmdInsertDebugUtilsLabelEXT(_command_buffer, pLabelInfo);
}

void VulkanCommandRecorder::SetSampleLocationsEXT(const VkSampleLocationsInfoEXT* pSampleLocationsInfo) const noexcept
{
  GetFunctionTable().vkCmdSetSampleLocationsEXT(_command_buffer, pSampleLocationsInfo);
}

void VulkanCommandRecorder::BindShadingRateImageNV(VkImageView imageView, VkImageLayout imageLayout) const noexcept
{
  GetFunctionTable().vkCmdBindShadingRateImageNV(_command_buffer, imageView, imageLayout);
}

void VulkanCommandRecorder::SetViewportShadingRatePaletteNV(uint32_t firstViewport, uint32_t viewportCount, const VkShadingRatePaletteNV* pShadingRatePalettes) const noexcept
{
  GetFunctionTable().vkCmdSetViewportShadingRatePaletteNV(_command_buffer, firstViewport, viewportCount, pShadingRatePalettes);
}

void VulkanCommandRecorder::SetCoarseSampleOrderNV(VkCoarseSampleOrderTypeNV sampleOrderType, uint32_t customSampleOrderCount, const VkCoarseSampleOrderCustomNV* pCustomSampleOrders) const noexcept
{
  GetFunctionTable().vkCmdSetCoarseSampleOrderNV(_command_buffer, sampleOrderType, customSampleOrderCount, pCustomSampleOrders);
}

void VulkanCommandRecorder::BuildAccelerationStructureNV(const VkAccelerationStructureInfoNV* pInfo, VkBuffer instanceData, VkDeviceSize instanceOffset, VkBool32 update, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkBuffer scratch, VkDeviceSize scratchOffset) const noexcept
{
  GetFunctionTable().vkCmdBuildAccelerationStructureNV(_command_buffer, pInfo, instanceData, instanceOffset, update, dst, src, scratch, scratchOffset);
}

void VulkanCommandRecorder::CopyAccelerationStructureNV(VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkCopyAccelerationStructureModeKHR mode) const noexcept
{
  GetFunctionTable().vkCmdCopyAccelerationStructureNV(_command_buffer, dst, src, mode);
}

void VulkanCommandRecorder::TraceRaysNV(VkBuffer raygenShaderBindingTableBuffer, VkDeviceSize raygenShaderBindingOffset, VkBuffer missShaderBindingTableBuffer, VkDeviceSize missShaderBindingOffset, VkDeviceSize missShaderBindingStride, VkBuffer hitShaderBindingTableBuffer, VkDeviceSize hitShaderBindingOffset, VkDeviceSize hitShaderBindingStride, VkBuffer callableShaderBindingTableBuffer, VkDeviceSize callableShaderBindingOffset, VkDeviceSize callableShaderBindingStride, uint32_t width, uint32_t height, uint32_t depth) const noexcept
{
  GetFunctionTable().vkCmdTraceRaysNV(_command_buffer, raygenShaderBindingTableBuffer, raygenShaderBindingOffset, missShaderBindingTableBuffer, missShaderBindingOffset, missShaderBindingStride, hitShaderBindingTableBuffer, hitShaderBindingOffset, hitShaderBindingStride, callableShaderBindingTableBuffer, callableShaderBindingOffset, callableShaderBindingStride, width, height, depth);
}

void VulkanCommandRecorder::WriteAccelerationStructuresPropertiesNV(uint32_t accelerationStructureCount, const VkAccelerationStructureNV* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) const noexcept
{
  GetFunctionTable().vkCmdWriteAccelerationStructuresPropertiesNV(_command_buffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
}

void VulkanCommandRecorder::WriteBufferMarkerAMD(VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker) const noexcept
{
  GetFunctionTable().vkCmdWriteBufferMarkerAMD(_command_buffer, pipelineStage, dstBuffer, dstOffset, marker);
}

void VulkanCommandRecorder::DrawMeshTasksNV(uint32_t taskCount, uint32_t firstTask) const noexcept
{
  GetFunctionTable().vkCmdDrawMeshTasksNV(_command_buffer, taskCount, firstTask);
}

void VulkanCommandRecorder::DrawMeshTasksIndirectNV(VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) const noexcept
{
  GetFunctionTable().vkCmdDrawMeshTasksIndirectNV(_command_buffer, buffer, offset, drawCount, stride);
}

void VulkanCommandRecorder::DrawMeshTasksIndirectCountNV(VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const noexcept
{
  GetFunctionTable().vkCmdDrawMeshTasksIndirectCountNV(_command_buffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void VulkanCommandRecorder::SetExclusiveScissorNV(uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkRect2D* pExclusiveScissors) const noexcept
{
  GetFunctionTable().vkCmdSetExclusiveScissorNV(_command_buffer, firstExclusiveScissor, exclusiveScissorCount, pExclusiveScissors);
}

void VulkanCommandRecorder::SetCheckpointNV(const void* pCheckpointMarker) const noexcept
{
  GetFunctionTable().vkCmdSetCheckpointNV(_command_buffer, pCheckpointMarker);
}

VulkanResult VulkanCommandRecorder::SetPerformanceMarkerINTEL(const VkPerformanceMarkerInfoINTEL* pMarkerInfo) const noexcept
{
 return GetFunctionTable().vkCmdSetPerformanceMarkerINTEL(_command_buffer, pMarkerInfo);
}

VulkanResult VulkanCommandRecorder::SetPerformanceStreamMarkerINTEL(const VkPerformanceStreamMarkerInfoINTEL* pMarkerInfo) const noexcept
{
 return GetFunctionTable().vkCmdSetPerformanceStreamMarkerINTEL(_command_buffer, pMarkerInfo);
}

VulkanResult VulkanCommandRecorder::SetPerformanceOverrideINTEL(const VkPerformanceOverrideInfoINTEL* pOverrideInfo) const noexcept
{
 return GetFunctionTable().vkCmdSetPerformanceOverrideINTEL(_command_buffer, pOverrideInfo);
}

void VulkanCommandRecorder::SetLineStippleEXT(uint32_t lineStippleFactor, uint16_t lineStipplePattern) const noexcept
{
  GetFunctionTable().vkCmdSetLineStippleEXT(_command_buffer, lineStippleFactor, lineStipplePattern);
}

void VulkanCommandRecorder::SetCullModeEXT(VkCullModeFlags cullMode) const noexcept
{
  GetFunctionTable().vkCmdSetCullModeEXT(_command_buffer, cullMode);
}

void VulkanCommandRecorder::SetFrontFaceEXT(VkFrontFace frontFace) const noexcept
{
  GetFunctionTable().vkCmdSetFrontFaceEXT(_command_buffer, frontFace);
}

void VulkanCommandRecorder::SetPrimitiveTopologyEXT(VkPrimitiveTopology primitiveTopology) const noexcept
{
  GetFunctionTable().vkCmdSetPrimitiveTopologyEXT(_command_buffer, primitiveTopology);
}

void VulkanCommandRecorder::SetViewportWithCountEXT(uint32_t viewportCount, const VkViewport* pViewports) const noexcept
{
  GetFunctionTable().vkCmdSetViewportWithCountEXT(_command_buffer, viewportCount, pViewports);
}

void VulkanCommandRecorder::SetScissorWithCountEXT(uint32_t scissorCount, const VkRect2D* pScissors) const noexcept
{
  GetFunctionTable().vkCmdSetScissorWithCountEXT(_command_buffer, scissorCount, pScissors);
}

void VulkanCommandRecorder::BindVertexBuffers2EXT(uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides) const noexcept
{
  GetFunctionTable().vkCmdBindVertexBuffers2EXT(_command_buffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes, pStrides);
}

void VulkanCommandRecorder::SetDepthTestEnableEXT(VkBool32 depthTestEnable) const noexcept
{
  GetFunctionTable().vkCmdSetDepthTestEnableEXT(_command_buffer, depthTestEnable);
}

void VulkanCommandRecorder::SetDepthWriteEnableEXT(VkBool32 depthWriteEnable) const noexcept
{
  GetFunctionTable().vkCmdSetDepthWriteEnableEXT(_command_buffer, depthWriteEnable);
}

void VulkanCommandRecorder::SetDepthCompareOpEXT(VkCompareOp depthCompareOp) const noexcept
{
  GetFunctionTable().vkCmdSetDepthCompareOpEXT(_command_buffer, depthCompareOp);
}

void VulkanCommandRecorder::SetDepthBoundsTestEnableEXT(VkBool32 depthBoundsTestEnable) const noexcept
{
  GetFunctionTable().vkCmdSetDepthBoundsTestEnableEXT(_command_buffer, depthBoundsTestEnable);
}

void VulkanCommandRecorder::SetStencilTestEnableEXT(VkBool32 stencilTestEnable) const noexcept
{
  GetFunctionTable().vkCmdSetStencilTestEnableEXT(_command_buffer, stencilTestEnable);
}

void VulkanCommandRecorder::SetStencilOpEXT(VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp) const noexcept
{
  GetFunctionTable().vkCmdSetStencilOpEXT(_command_buffer, faceMask, failOp, passOp, depthFailOp, compareOp);
}

void VulkanCommandRecorder::PreprocessGeneratedCommandsNV(const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo) const noexcept
{
  GetFunctionTable().vkCmdPreprocessGeneratedCommandsNV(_command_buffer, pGeneratedCommandsInfo);
}

void VulkanCommandRecorder::ExecuteGeneratedCommandsNV(VkBool32 isPreprocessed, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo) const noexcept
{
  GetFunctionTable().vkCmdExecuteGeneratedCommandsNV(_command_buffer, isPreprocessed, pGeneratedCommandsInfo);
}

void VulkanCommandRecorder::BindPipelineShaderGroupNV(VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline, uint32_t groupIndex) const noexcept
{
  GetFunctionTable().vkCmdBindPipelineShaderGroupNV(_command_buffer, pipelineBindPoint, pipeline, groupIndex);
}

void VulkanCommandRecorder::SetFragmentShadingRateEnumNV(VkFragmentShadingRateNV shadingRate, const VkFragmentShadingRateCombinerOpKHR combinerOps[2]) const noexcept
{
  GetFunctionTable().vkCmdSetFragmentShadingRateEnumNV(_command_buffer, shadingRate, combinerOps);
}

void VulkanCommandRecorder::SetVertexInputEXT(uint32_t vertexBindingDescriptionCount, const VkVertexInputBindingDescription2EXT* pVertexBindingDescriptions, uint32_t vertexAttributeDescriptionCount, const VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions) const noexcept
{
  GetFunctionTable().vkCmdSetVertexInputEXT(_command_buffer, vertexBindingDescriptionCount, pVertexBindingDescriptions, vertexAttributeDescriptionCount, pVertexAttributeDescriptions);
}

void VulkanCommandRecorder::SubpassShadingHUAWEI() const noexcept
{
  GetFunctionTable().vkCmdSubpassShadingHUAWEI(_command_buffer);
}

void VulkanCommandRecorder::BindInvocationMaskHUAWEI(VkImageView imageView, VkImageLayout imageLayout) const noexcept
{
  GetFunctionTable().vkCmdBindInvocationMaskHUAWEI(_command_buffer, imageView, imageLayout);
}

void VulkanCommandRecorder::SetPatchControlPointsEXT(uint32_t patchControlPoints) const noexcept
{
  GetFunctionTable().vkCmdSetPatchControlPointsEXT(_command_buffer, patchControlPoints);
}

void VulkanCommandRecorder::SetRasterizerDiscardEnableEXT(VkBool32 rasterizerDiscardEnable) const noexcept
{
  GetFunctionTable().vkCmdSetRasterizerDiscardEnableEXT(_command_buffer, rasterizerDiscardEnable);
}

void VulkanCommandRecorder::SetDepthBiasEnableEXT(VkBool32 depthBiasEnable) const noexcept
{
  GetFunctionTable().vkCmdSetDepthBiasEnableEXT(_command_buffer, depthBiasEnable);
}

void VulkanCommandRecorder::SetLogicOpEXT(VkLogicOp logicOp) const noexcept
{
  GetFunctionTable().vkCmdSetLogicOpEXT(_command_buffer, logicOp);
}

void VulkanCommandRecorder::SetPrimitiveRestartEnableEXT(VkBool32 primitiveRestartEnable) const noexcept
{
  GetFunctionTable().vkCmdSetPrimitiveRestartEnableEXT(_command_buffer, primitiveRestartEnable);
}

void VulkanCommandRecorder::SetColorWriteEnableEXT(uint32_t attachmentCount, const VkBool32* pColorWriteEnables) const noexcept
{
  GetFunctionTable().vkCmdSetColorWriteEnableEXT(_command_buffer, attachmentCount, pColorWriteEnables);
}

void VulkanCommandRecorder::DrawMultiEXT(uint32_t drawCount, const VkMultiDrawInfoEXT* pVertexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride) const noexcept
{
  GetFunctionTable().vkCmdDrawMultiEXT(_command_buffer, drawCount, pVertexInfo, instanceCount, firstInstance, stride);
}

void VulkanCommandRecorder::DrawMultiIndexedEXT(uint32_t drawCount, const VkMultiDrawIndexedInfoEXT* pIndexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride, const int32_t* pVertexOffset) const noexcept
{
  GetFunctionTable().vkCmdDrawMultiIndexedEXT(_command_buffer, drawCount, pIndexInfo, instanceCount, firstInstance, stride, pVertexOffset);
}

void VulkanCommandRecorder::BuildAccelerationStructuresKHR(uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos) const noexcept
{
  GetFunctionTable().vkCmdBuildAccelerationStructuresKHR(_command_buffer, infoCount, pInfos, ppBuildRangeInfos);
}

void VulkanCommandRecorder::BuildAccelerationStructuresIndirectKHR(uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkDeviceAddress* pIndirectDeviceAddresses, const uint32_t* pIndirectStrides, const uint32_t* const* ppMaxPrimitiveCounts) const noexcept
{
  GetFunctionTable().vkCmdBuildAccelerationStructuresIndirectKHR(_command_buffer, infoCount, pInfos, pIndirectDeviceAddresses, pIndirectStrides, ppMaxPrimitiveCounts);
}

void VulkanCommandRecorder::CopyAccelerationStructureKHR(const VkCopyAccelerationStructureInfoKHR* pInfo) const noexcept
{
  GetFunctionTable().vkCmdCopyAccelerationStructureKHR(_command_buffer, pInfo);
}

void VulkanCommandRecorder::CopyAccelerationStructureToMemoryKHR(const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo) const noexcept
{
  GetFunctionTable().vkCmdCopyAccelerationStructureToMemoryKHR(_command_buffer, pInfo);
}

void VulkanCommandRecorder::CopyMemoryToAccelerationStructureKHR(const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo) const noexcept
{
  GetFunctionTable().vkCmdCopyMemoryToAccelerationStructureKHR(_command_buffer, pInfo);
}

void VulkanCommandRecorder::WriteAccelerationStructuresPropertiesKHR(uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) const noexcept
{
  GetFunctionTable().vkCmdWriteAccelerationStructuresPropertiesKHR(_command_buffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
}

void VulkanCommandRecorder::TraceRaysKHR(const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, uint32_t width, uint32_t height, uint32_t depth) const noexcept
{
  GetFunctionTable().vkCmdTraceRaysKHR(_command_buffer, pRaygenShaderBindingTable, pMissShaderBindingTable, pHitShaderBindingTable, pCallableShaderBindingTable, width, height, depth);
}

void VulkanCommandRecorder::TraceRaysIndirectKHR(const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, VkDeviceAddress indirectDeviceAddress) const noexcept
{
  GetFunctionTable().vkCmdTraceRaysIndirectKHR(_command_buffer, pRaygenShaderBindingTable, pMissShaderBindingTable, pHitShaderBindingTable, pCallableShaderBindingTable, indirectDeviceAddress);
}

void VulkanCommandRecorder::SetRayTracingPipelineStackSizeKHR(uint32_t pipelineStackSize) const noexcept
{
  GetFunctionTable().vkCmdSetRayTracingPipelineStackSizeKHR(_command_buffer, pipelineStackSize);
}

// clang-format on

} // namespace plex::graphics::vkapi
