#include "vulkan_api.h"

#include "vulkan_function_table.h"
#include "vulkan_loader.h"

namespace plex::graphics::vkapi
{

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

VulkanResultWithValue<std::vector<VkPhysicalDevice>> vkEnumeratePhysicalDevices()
{
  const auto& fp = loader::GetFunctionTable().vkEnumeratePhysicalDevices;
  uint32_t count = 0;
  fp(loader::GetInstance(), &count, nullptr);
  VulkanResultWithValue<std::vector<VkPhysicalDevice>> result;
  result.value.resize(count);
  result.result = fp(loader::GetInstance(), &count, result.value.data());
  return result;
}

void vkGetPhysicalDeviceFeatures(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures)
{
  return loader::GetFunctionTable().vkGetPhysicalDeviceFeatures(physicalDevice, pFeatures);
}

void vkGetPhysicalDeviceFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties* pFormatProperties)
{
  return loader::GetFunctionTable().vkGetPhysicalDeviceFormatProperties(physicalDevice, format, pFormatProperties);
}

VulkanResult vkGetPhysicalDeviceImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties)
{
  return loader::GetFunctionTable().vkGetPhysicalDeviceImageFormatProperties(physicalDevice, format, type, tiling, usage, flags, pImageFormatProperties);
}

void vkGetPhysicalDeviceProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties)
{
  return loader::GetFunctionTable().vkGetPhysicalDeviceProperties(physicalDevice, pProperties);
}

VulkanResultWithValue<std::vector<VkQueueFamilyProperties>> vkGetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice physicalDevice)
{
  const auto& fp = loader::GetFunctionTable().vkGetPhysicalDeviceQueueFamilyProperties;
  uint32_t count = 0;
  fp(physicalDevice, &count, nullptr);
  VulkanResultWithValue<std::vector<VkQueueFamilyProperties>> result;
  result.value.resize(count);
  fp(physicalDevice, &count, result.value.data());
  return result;
}

void vkGetPhysicalDeviceMemoryProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties)
{
  return loader::GetFunctionTable().vkGetPhysicalDeviceMemoryProperties(physicalDevice, pMemoryProperties);
}

VulkanFunctionPointer vkGetInstanceProcAddr(const char* pName)
{
  return loader::GetFunctionTable().vkGetInstanceProcAddr(loader::GetInstance(), pName);
}

VulkanFunctionPointer vkGetDeviceProcAddr(const char* pName)
{
  return loader::GetFunctionTable().vkGetDeviceProcAddr(loader::GetDevice(), pName);
}

VulkanResult vkCreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice)
{
  return loader::GetFunctionTable().vkCreateDevice(physicalDevice, pCreateInfo, pAllocator, pDevice);
}

VulkanResultWithValue<std::vector<VkExtensionProperties>> vkEnumerateInstanceExtensionProperties(const char* pLayerName)
{
  const auto& fp = loader::GetFunctionTable().vkEnumerateInstanceExtensionProperties;
  uint32_t count = 0;
  fp(pLayerName, &count, nullptr);
  VulkanResultWithValue<std::vector<VkExtensionProperties>> result;
  result.value.resize(count);
  result.result = fp(pLayerName, &count, result.value.data());
  return result;
}

VulkanResultWithValue<std::vector<VkExtensionProperties>> vkEnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice, const char* pLayerName)
{
  const auto& fp = loader::GetFunctionTable().vkEnumerateDeviceExtensionProperties;
  uint32_t count = 0;
  fp(physicalDevice, pLayerName, &count, nullptr);
  VulkanResultWithValue<std::vector<VkExtensionProperties>> result;
  result.value.resize(count);
  result.result = fp(physicalDevice, pLayerName, &count, result.value.data());
  return result;
}

VulkanResultWithValue<std::vector<VkLayerProperties>> vkEnumerateInstanceLayerProperties()
{
  const auto& fp = loader::GetFunctionTable().vkEnumerateInstanceLayerProperties;
  uint32_t count = 0;
  fp(&count, nullptr);
  VulkanResultWithValue<std::vector<VkLayerProperties>> result;
  result.value.resize(count);
  result.result = fp(&count, result.value.data());
  return result;
}

VulkanResultWithValue<std::vector<VkLayerProperties>> vkEnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice)
{
  const auto& fp = loader::GetFunctionTable().vkEnumerateDeviceLayerProperties;
  uint32_t count = 0;
  fp(physicalDevice, &count, nullptr);
  VulkanResultWithValue<std::vector<VkLayerProperties>> result;
  result.value.resize(count);
  result.result = fp(physicalDevice, &count, result.value.data());
  return result;
}

void vkGetDeviceQueue(uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue)
{
  return loader::GetFunctionTable().vkGetDeviceQueue(loader::GetDevice(), queueFamilyIndex, queueIndex, pQueue);
}

VulkanResult vkQueueSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence)
{
  return loader::GetFunctionTable().vkQueueSubmit(queue, submitCount, pSubmits, fence);
}

VulkanResult vkQueueWaitIdle(VkQueue queue)
{
  return loader::GetFunctionTable().vkQueueWaitIdle(queue);
}

VulkanResult vkDeviceWaitIdle()
{
  return loader::GetFunctionTable().vkDeviceWaitIdle(loader::GetDevice());
}

VulkanResult vkAllocateMemory(const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory)
{
  return loader::GetFunctionTable().vkAllocateMemory(loader::GetDevice(), pAllocateInfo, pAllocator, pMemory);
}

void vkFreeMemory(VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator)
{
  return loader::GetFunctionTable().vkFreeMemory(loader::GetDevice(), memory, pAllocator);
}

VulkanResult vkMapMemory(VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData)
{
  return loader::GetFunctionTable().vkMapMemory(loader::GetDevice(), memory, offset, size, flags, ppData);
}

void vkUnmapMemory(VkDeviceMemory memory)
{
  return loader::GetFunctionTable().vkUnmapMemory(loader::GetDevice(), memory);
}

VulkanResult vkFlushMappedMemoryRanges(uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges)
{
  return loader::GetFunctionTable().vkFlushMappedMemoryRanges(loader::GetDevice(), memoryRangeCount, pMemoryRanges);
}

VulkanResult vkInvalidateMappedMemoryRanges(uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges)
{
  return loader::GetFunctionTable().vkInvalidateMappedMemoryRanges(loader::GetDevice(), memoryRangeCount, pMemoryRanges);
}

void vkGetDeviceMemoryCommitment(VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes)
{
  return loader::GetFunctionTable().vkGetDeviceMemoryCommitment(loader::GetDevice(), memory, pCommittedMemoryInBytes);
}

VulkanResult vkBindBufferMemory(VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset)
{
  return loader::GetFunctionTable().vkBindBufferMemory(loader::GetDevice(), buffer, memory, memoryOffset);
}

VulkanResult vkBindImageMemory(VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset)
{
  return loader::GetFunctionTable().vkBindImageMemory(loader::GetDevice(), image, memory, memoryOffset);
}

void vkGetBufferMemoryRequirements(VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements)
{
  return loader::GetFunctionTable().vkGetBufferMemoryRequirements(loader::GetDevice(), buffer, pMemoryRequirements);
}

void vkGetImageMemoryRequirements(VkImage image, VkMemoryRequirements* pMemoryRequirements)
{
  return loader::GetFunctionTable().vkGetImageMemoryRequirements(loader::GetDevice(), image, pMemoryRequirements);
}

VulkanResultWithValue<std::vector<VkSparseImageMemoryRequirements>> vkGetImageSparseMemoryRequirements(VkImage image)
{
  const auto& fp = loader::GetFunctionTable().vkGetImageSparseMemoryRequirements;
  uint32_t count = 0;
  fp(loader::GetDevice(), image, &count, nullptr);
  VulkanResultWithValue<std::vector<VkSparseImageMemoryRequirements>> result;
  result.value.resize(count);
  fp(loader::GetDevice(), image, &count, result.value.data());
  return result;
}

VulkanResultWithValue<std::vector<VkSparseImageFormatProperties>> vkGetPhysicalDeviceSparseImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling)
{
  const auto& fp = loader::GetFunctionTable().vkGetPhysicalDeviceSparseImageFormatProperties;
  uint32_t count = 0;
  fp(physicalDevice, format, type, samples, usage, tiling, &count, nullptr);
  VulkanResultWithValue<std::vector<VkSparseImageFormatProperties>> result;
  result.value.resize(count);
  fp(physicalDevice, format, type, samples, usage, tiling, &count, result.value.data());
  return result;
}

VulkanResult vkQueueBindSparse(VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence)
{
  return loader::GetFunctionTable().vkQueueBindSparse(queue, bindInfoCount, pBindInfo, fence);
}

VulkanResult vkCreateFence(const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence)
{
  return loader::GetFunctionTable().vkCreateFence(loader::GetDevice(), pCreateInfo, pAllocator, pFence);
}

void vkDestroyFence(VkFence fence, const VkAllocationCallbacks* pAllocator)
{
  return loader::GetFunctionTable().vkDestroyFence(loader::GetDevice(), fence, pAllocator);
}

VulkanResult vkResetFences(uint32_t fenceCount, const VkFence* pFences)
{
  return loader::GetFunctionTable().vkResetFences(loader::GetDevice(), fenceCount, pFences);
}

VulkanResult vkGetFenceStatus(VkFence fence)
{
  return loader::GetFunctionTable().vkGetFenceStatus(loader::GetDevice(), fence);
}

VulkanResult vkWaitForFences(uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout)
{
  return loader::GetFunctionTable().vkWaitForFences(loader::GetDevice(), fenceCount, pFences, waitAll, timeout);
}

VulkanResult vkCreateSemaphore(const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore)
{
  return loader::GetFunctionTable().vkCreateSemaphore(loader::GetDevice(), pCreateInfo, pAllocator, pSemaphore);
}

void vkDestroySemaphore(VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator)
{
  return loader::GetFunctionTable().vkDestroySemaphore(loader::GetDevice(), semaphore, pAllocator);
}

VulkanResult vkCreateEvent(const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent)
{
  return loader::GetFunctionTable().vkCreateEvent(loader::GetDevice(), pCreateInfo, pAllocator, pEvent);
}

void vkDestroyEvent(VkEvent event, const VkAllocationCallbacks* pAllocator)
{
  return loader::GetFunctionTable().vkDestroyEvent(loader::GetDevice(), event, pAllocator);
}

VulkanResult vkGetEventStatus(VkEvent event)
{
  return loader::GetFunctionTable().vkGetEventStatus(loader::GetDevice(), event);
}

VulkanResult vkSetEvent(VkEvent event)
{
  return loader::GetFunctionTable().vkSetEvent(loader::GetDevice(), event);
}

VulkanResult vkResetEvent(VkEvent event)
{
  return loader::GetFunctionTable().vkResetEvent(loader::GetDevice(), event);
}

VulkanResult vkCreateQueryPool(const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool)
{
  return loader::GetFunctionTable().vkCreateQueryPool(loader::GetDevice(), pCreateInfo, pAllocator, pQueryPool);
}

void vkDestroyQueryPool(VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator)
{
  return loader::GetFunctionTable().vkDestroyQueryPool(loader::GetDevice(), queryPool, pAllocator);
}

VulkanResult vkGetQueryPoolResults(VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags)
{
  return loader::GetFunctionTable().vkGetQueryPoolResults(loader::GetDevice(), queryPool, firstQuery, queryCount, dataSize, pData, stride, flags);
}

VulkanResult vkCreateBuffer(const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer)
{
  return loader::GetFunctionTable().vkCreateBuffer(loader::GetDevice(), pCreateInfo, pAllocator, pBuffer);
}

void vkDestroyBuffer(VkBuffer buffer, const VkAllocationCallbacks* pAllocator)
{
  return loader::GetFunctionTable().vkDestroyBuffer(loader::GetDevice(), buffer, pAllocator);
}

VulkanResult vkCreateBufferView(const VkBufferViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferView* pView)
{
  return loader::GetFunctionTable().vkCreateBufferView(loader::GetDevice(), pCreateInfo, pAllocator, pView);
}

void vkDestroyBufferView(VkBufferView bufferView, const VkAllocationCallbacks* pAllocator)
{
  return loader::GetFunctionTable().vkDestroyBufferView(loader::GetDevice(), bufferView, pAllocator);
}

VulkanResult vkCreateImage(const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage)
{
  return loader::GetFunctionTable().vkCreateImage(loader::GetDevice(), pCreateInfo, pAllocator, pImage);
}

void vkDestroyImage(VkImage image, const VkAllocationCallbacks* pAllocator)
{
  return loader::GetFunctionTable().vkDestroyImage(loader::GetDevice(), image, pAllocator);
}

void vkGetImageSubresourceLayout(VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout)
{
  return loader::GetFunctionTable().vkGetImageSubresourceLayout(loader::GetDevice(), image, pSubresource, pLayout);
}

VulkanResult vkCreateImageView(const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView)
{
  return loader::GetFunctionTable().vkCreateImageView(loader::GetDevice(), pCreateInfo, pAllocator, pView);
}

void vkDestroyImageView(VkImageView imageView, const VkAllocationCallbacks* pAllocator)
{
  return loader::GetFunctionTable().vkDestroyImageView(loader::GetDevice(), imageView, pAllocator);
}

VulkanResult vkCreateShaderModule(const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule)
{
  return loader::GetFunctionTable().vkCreateShaderModule(loader::GetDevice(), pCreateInfo, pAllocator, pShaderModule);
}

void vkDestroyShaderModule(VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator)
{
  return loader::GetFunctionTable().vkDestroyShaderModule(loader::GetDevice(), shaderModule, pAllocator);
}

VulkanResult vkCreatePipelineCache(const VkPipelineCacheCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache)
{
  return loader::GetFunctionTable().vkCreatePipelineCache(loader::GetDevice(), pCreateInfo, pAllocator, pPipelineCache);
}

void vkDestroyPipelineCache(VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator)
{
  return loader::GetFunctionTable().vkDestroyPipelineCache(loader::GetDevice(), pipelineCache, pAllocator);
}

VulkanResult vkGetPipelineCacheData(VkPipelineCache pipelineCache, size_t* pDataSize, void* pData)
{
  return loader::GetFunctionTable().vkGetPipelineCacheData(loader::GetDevice(), pipelineCache, pDataSize, pData);
}

VulkanResult vkMergePipelineCaches(VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches)
{
  return loader::GetFunctionTable().vkMergePipelineCaches(loader::GetDevice(), dstCache, srcCacheCount, pSrcCaches);
}

VulkanResult vkCreateGraphicsPipelines(VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
{
  return loader::GetFunctionTable().vkCreateGraphicsPipelines(loader::GetDevice(), pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}

VulkanResult vkCreateComputePipelines(VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
{
  return loader::GetFunctionTable().vkCreateComputePipelines(loader::GetDevice(), pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}

void vkDestroyPipeline(VkPipeline pipeline, const VkAllocationCallbacks* pAllocator)
{
  return loader::GetFunctionTable().vkDestroyPipeline(loader::GetDevice(), pipeline, pAllocator);
}

VulkanResult vkCreatePipelineLayout(const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout)
{
  return loader::GetFunctionTable().vkCreatePipelineLayout(loader::GetDevice(), pCreateInfo, pAllocator, pPipelineLayout);
}

void vkDestroyPipelineLayout(VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator)
{
  return loader::GetFunctionTable().vkDestroyPipelineLayout(loader::GetDevice(), pipelineLayout, pAllocator);
}

VulkanResult vkCreateSampler(const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler)
{
  return loader::GetFunctionTable().vkCreateSampler(loader::GetDevice(), pCreateInfo, pAllocator, pSampler);
}

void vkDestroySampler(VkSampler sampler, const VkAllocationCallbacks* pAllocator)
{
  return loader::GetFunctionTable().vkDestroySampler(loader::GetDevice(), sampler, pAllocator);
}

VulkanResult vkCreateDescriptorSetLayout(const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout)
{
  return loader::GetFunctionTable().vkCreateDescriptorSetLayout(loader::GetDevice(), pCreateInfo, pAllocator, pSetLayout);
}

void vkDestroyDescriptorSetLayout(VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator)
{
  return loader::GetFunctionTable().vkDestroyDescriptorSetLayout(loader::GetDevice(), descriptorSetLayout, pAllocator);
}

VulkanResult vkCreateDescriptorPool(const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool)
{
  return loader::GetFunctionTable().vkCreateDescriptorPool(loader::GetDevice(), pCreateInfo, pAllocator, pDescriptorPool);
}

void vkDestroyDescriptorPool(VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator)
{
  return loader::GetFunctionTable().vkDestroyDescriptorPool(loader::GetDevice(), descriptorPool, pAllocator);
}

VulkanResult vkResetDescriptorPool(VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags)
{
  return loader::GetFunctionTable().vkResetDescriptorPool(loader::GetDevice(), descriptorPool, flags);
}

VulkanResult vkAllocateDescriptorSets(const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets)
{
  return loader::GetFunctionTable().vkAllocateDescriptorSets(loader::GetDevice(), pAllocateInfo, pDescriptorSets);
}

VulkanResult vkFreeDescriptorSets(VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets)
{
  return loader::GetFunctionTable().vkFreeDescriptorSets(loader::GetDevice(), descriptorPool, descriptorSetCount, pDescriptorSets);
}

void vkUpdateDescriptorSets(uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies)
{
  return loader::GetFunctionTable().vkUpdateDescriptorSets(loader::GetDevice(), descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies);
}

VulkanResult vkCreateFramebuffer(const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer)
{
  return loader::GetFunctionTable().vkCreateFramebuffer(loader::GetDevice(), pCreateInfo, pAllocator, pFramebuffer);
}

void vkDestroyFramebuffer(VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator)
{
  return loader::GetFunctionTable().vkDestroyFramebuffer(loader::GetDevice(), framebuffer, pAllocator);
}

VulkanResult vkCreateRenderPass(const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass)
{
  return loader::GetFunctionTable().vkCreateRenderPass(loader::GetDevice(), pCreateInfo, pAllocator, pRenderPass);
}

void vkDestroyRenderPass(VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator)
{
  return loader::GetFunctionTable().vkDestroyRenderPass(loader::GetDevice(), renderPass, pAllocator);
}

void vkGetRenderAreaGranularity(VkRenderPass renderPass, VkExtent2D* pGranularity)
{
  return loader::GetFunctionTable().vkGetRenderAreaGranularity(loader::GetDevice(), renderPass, pGranularity);
}

VulkanResult vkCreateCommandPool(const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool)
{
  return loader::GetFunctionTable().vkCreateCommandPool(loader::GetDevice(), pCreateInfo, pAllocator, pCommandPool);
}

void vkDestroyCommandPool(VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator)
{
  return loader::GetFunctionTable().vkDestroyCommandPool(loader::GetDevice(), commandPool, pAllocator);
}

VulkanResult vkResetCommandPool(VkCommandPool commandPool, VkCommandPoolResetFlags flags)
{
  return loader::GetFunctionTable().vkResetCommandPool(loader::GetDevice(), commandPool, flags);
}

VulkanResult vkAllocateCommandBuffers(const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers)
{
  return loader::GetFunctionTable().vkAllocateCommandBuffers(loader::GetDevice(), pAllocateInfo, pCommandBuffers);
}

void vkFreeCommandBuffers(VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers)
{
  return loader::GetFunctionTable().vkFreeCommandBuffers(loader::GetDevice(), commandPool, commandBufferCount, pCommandBuffers);
}

VulkanResult vkEnumerateInstanceVersion(uint32_t* pApiVersion)
{
  return loader::GetFunctionTable().vkEnumerateInstanceVersion(pApiVersion);
}

VulkanResult vkBindBufferMemory2(uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos)
{
  return loader::GetFunctionTable().vkBindBufferMemory2(loader::GetDevice(), bindInfoCount, pBindInfos);
}

VulkanResult vkBindImageMemory2(uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos)
{
  return loader::GetFunctionTable().vkBindImageMemory2(loader::GetDevice(), bindInfoCount, pBindInfos);
}

void vkGetDeviceGroupPeerMemoryFeatures(uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures)
{
  return loader::GetFunctionTable().vkGetDeviceGroupPeerMemoryFeatures(loader::GetDevice(), heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
}

VulkanResultWithValue<std::vector<VkPhysicalDeviceGroupProperties>> vkEnumeratePhysicalDeviceGroups()
{
  const auto& fp = loader::GetFunctionTable().vkEnumeratePhysicalDeviceGroups;
  uint32_t count = 0;
  fp(loader::GetInstance(), &count, nullptr);
  VulkanResultWithValue<std::vector<VkPhysicalDeviceGroupProperties>> result;
  result.value.resize(count);
  result.result = fp(loader::GetInstance(), &count, result.value.data());
  return result;
}

void vkGetImageMemoryRequirements2(const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
  return loader::GetFunctionTable().vkGetImageMemoryRequirements2(loader::GetDevice(), pInfo, pMemoryRequirements);
}

void vkGetBufferMemoryRequirements2(const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
  return loader::GetFunctionTable().vkGetBufferMemoryRequirements2(loader::GetDevice(), pInfo, pMemoryRequirements);
}

VulkanResultWithValue<std::vector<VkSparseImageMemoryRequirements2>> vkGetImageSparseMemoryRequirements2(const VkImageSparseMemoryRequirementsInfo2* pInfo)
{
  const auto& fp = loader::GetFunctionTable().vkGetImageSparseMemoryRequirements2;
  uint32_t count = 0;
  fp(loader::GetDevice(), pInfo, &count, nullptr);
  VulkanResultWithValue<std::vector<VkSparseImageMemoryRequirements2>> result;
  result.value.resize(count);
  fp(loader::GetDevice(), pInfo, &count, result.value.data());
  return result;
}

void vkGetPhysicalDeviceFeatures2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures)
{
  return loader::GetFunctionTable().vkGetPhysicalDeviceFeatures2(physicalDevice, pFeatures);
}

void vkGetPhysicalDeviceProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties)
{
  return loader::GetFunctionTable().vkGetPhysicalDeviceProperties2(physicalDevice, pProperties);
}

void vkGetPhysicalDeviceFormatProperties2(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties)
{
  return loader::GetFunctionTable().vkGetPhysicalDeviceFormatProperties2(physicalDevice, format, pFormatProperties);
}

VulkanResult vkGetPhysicalDeviceImageFormatProperties2(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties)
{
  return loader::GetFunctionTable().vkGetPhysicalDeviceImageFormatProperties2(physicalDevice, pImageFormatInfo, pImageFormatProperties);
}

VulkanResultWithValue<std::vector<VkQueueFamilyProperties2>> vkGetPhysicalDeviceQueueFamilyProperties2(VkPhysicalDevice physicalDevice)
{
  const auto& fp = loader::GetFunctionTable().vkGetPhysicalDeviceQueueFamilyProperties2;
  uint32_t count = 0;
  fp(physicalDevice, &count, nullptr);
  VulkanResultWithValue<std::vector<VkQueueFamilyProperties2>> result;
  result.value.resize(count);
  fp(physicalDevice, &count, result.value.data());
  return result;
}

void vkGetPhysicalDeviceMemoryProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties)
{
  return loader::GetFunctionTable().vkGetPhysicalDeviceMemoryProperties2(physicalDevice, pMemoryProperties);
}

VulkanResultWithValue<std::vector<VkSparseImageFormatProperties2>> vkGetPhysicalDeviceSparseImageFormatProperties2(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo)
{
  const auto& fp = loader::GetFunctionTable().vkGetPhysicalDeviceSparseImageFormatProperties2;
  uint32_t count = 0;
  fp(physicalDevice, pFormatInfo, &count, nullptr);
  VulkanResultWithValue<std::vector<VkSparseImageFormatProperties2>> result;
  result.value.resize(count);
  fp(physicalDevice, pFormatInfo, &count, result.value.data());
  return result;
}

void vkTrimCommandPool(VkCommandPool commandPool, VkCommandPoolTrimFlags flags)
{
  return loader::GetFunctionTable().vkTrimCommandPool(loader::GetDevice(), commandPool, flags);
}

void vkGetDeviceQueue2(const VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue)
{
  return loader::GetFunctionTable().vkGetDeviceQueue2(loader::GetDevice(), pQueueInfo, pQueue);
}

VulkanResult vkCreateSamplerYcbcrConversion(const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion)
{
  return loader::GetFunctionTable().vkCreateSamplerYcbcrConversion(loader::GetDevice(), pCreateInfo, pAllocator, pYcbcrConversion);
}

void vkDestroySamplerYcbcrConversion(VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator)
{
  return loader::GetFunctionTable().vkDestroySamplerYcbcrConversion(loader::GetDevice(), ycbcrConversion, pAllocator);
}

VulkanResult vkCreateDescriptorUpdateTemplate(const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate)
{
  return loader::GetFunctionTable().vkCreateDescriptorUpdateTemplate(loader::GetDevice(), pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
}

void vkDestroyDescriptorUpdateTemplate(VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator)
{
  return loader::GetFunctionTable().vkDestroyDescriptorUpdateTemplate(loader::GetDevice(), descriptorUpdateTemplate, pAllocator);
}

void vkUpdateDescriptorSetWithTemplate(VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData)
{
  return loader::GetFunctionTable().vkUpdateDescriptorSetWithTemplate(loader::GetDevice(), descriptorSet, descriptorUpdateTemplate, pData);
}

void vkGetPhysicalDeviceExternalBufferProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties)
{
  return loader::GetFunctionTable().vkGetPhysicalDeviceExternalBufferProperties(physicalDevice, pExternalBufferInfo, pExternalBufferProperties);
}

void vkGetPhysicalDeviceExternalFenceProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties)
{
  return loader::GetFunctionTable().vkGetPhysicalDeviceExternalFenceProperties(physicalDevice, pExternalFenceInfo, pExternalFenceProperties);
}

void vkGetPhysicalDeviceExternalSemaphoreProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties)
{
  return loader::GetFunctionTable().vkGetPhysicalDeviceExternalSemaphoreProperties(physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
}

void vkGetDescriptorSetLayoutSupport(const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport)
{
  return loader::GetFunctionTable().vkGetDescriptorSetLayoutSupport(loader::GetDevice(), pCreateInfo, pSupport);
}

VulkanResult vkCreateRenderPass2(const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass)
{
  return loader::GetFunctionTable().vkCreateRenderPass2(loader::GetDevice(), pCreateInfo, pAllocator, pRenderPass);
}

void vkResetQueryPool(VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount)
{
  return loader::GetFunctionTable().vkResetQueryPool(loader::GetDevice(), queryPool, firstQuery, queryCount);
}

VulkanResult vkGetSemaphoreCounterValue(VkSemaphore semaphore, uint64_t* pValue)
{
  return loader::GetFunctionTable().vkGetSemaphoreCounterValue(loader::GetDevice(), semaphore, pValue);
}

VulkanResult vkWaitSemaphores(const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout)
{
  return loader::GetFunctionTable().vkWaitSemaphores(loader::GetDevice(), pWaitInfo, timeout);
}

VulkanResult vkSignalSemaphore(const VkSemaphoreSignalInfo* pSignalInfo)
{
  return loader::GetFunctionTable().vkSignalSemaphore(loader::GetDevice(), pSignalInfo);
}

VulkanResultWithValue<VkDeviceAddress> vkGetBufferDeviceAddress(const VkBufferDeviceAddressInfo* pInfo)
{
  return loader::GetFunctionTable().vkGetBufferDeviceAddress(loader::GetDevice(), pInfo);
}

VulkanResultWithValue<uint64_t> vkGetBufferOpaqueCaptureAddress(const VkBufferDeviceAddressInfo* pInfo)
{
  return loader::GetFunctionTable().vkGetBufferOpaqueCaptureAddress(loader::GetDevice(), pInfo);
}

VulkanResultWithValue<uint64_t> vkGetDeviceMemoryOpaqueCaptureAddress(const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo)
{
  return loader::GetFunctionTable().vkGetDeviceMemoryOpaqueCaptureAddress(loader::GetDevice(), pInfo);
}

VulkanResultWithValue<std::vector<VkPhysicalDeviceToolProperties>> vkGetPhysicalDeviceToolProperties(VkPhysicalDevice physicalDevice)
{
  const auto& fp = loader::GetFunctionTable().vkGetPhysicalDeviceToolProperties;
  uint32_t count = 0;
  fp(physicalDevice, &count, nullptr);
  VulkanResultWithValue<std::vector<VkPhysicalDeviceToolProperties>> result;
  result.value.resize(count);
  result.result = fp(physicalDevice, &count, result.value.data());
  return result;
}

VulkanResult vkCreatePrivateDataSlot(const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot)
{
  return loader::GetFunctionTable().vkCreatePrivateDataSlot(loader::GetDevice(), pCreateInfo, pAllocator, pPrivateDataSlot);
}

void vkDestroyPrivateDataSlot(VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator)
{
  return loader::GetFunctionTable().vkDestroyPrivateDataSlot(loader::GetDevice(), privateDataSlot, pAllocator);
}

VulkanResult vkSetPrivateData(VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data)
{
  return loader::GetFunctionTable().vkSetPrivateData(loader::GetDevice(), objectType, objectHandle, privateDataSlot, data);
}

void vkGetPrivateData(VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData)
{
  return loader::GetFunctionTable().vkGetPrivateData(loader::GetDevice(), objectType, objectHandle, privateDataSlot, pData);
}

VulkanResult vkQueueSubmit2(VkQueue queue, uint32_t submitCount, const VkSubmitInfo2* pSubmits, VkFence fence)
{
  return loader::GetFunctionTable().vkQueueSubmit2(queue, submitCount, pSubmits, fence);
}

void vkGetDeviceBufferMemoryRequirements(const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
  return loader::GetFunctionTable().vkGetDeviceBufferMemoryRequirements(loader::GetDevice(), pInfo, pMemoryRequirements);
}

void vkGetDeviceImageMemoryRequirements(const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
  return loader::GetFunctionTable().vkGetDeviceImageMemoryRequirements(loader::GetDevice(), pInfo, pMemoryRequirements);
}

VulkanResultWithValue<std::vector<VkSparseImageMemoryRequirements2>> vkGetDeviceImageSparseMemoryRequirements(const VkDeviceImageMemoryRequirements* pInfo)
{
  const auto& fp = loader::GetFunctionTable().vkGetDeviceImageSparseMemoryRequirements;
  uint32_t count = 0;
  fp(loader::GetDevice(), pInfo, &count, nullptr);
  VulkanResultWithValue<std::vector<VkSparseImageMemoryRequirements2>> result;
  result.value.resize(count);
  fp(loader::GetDevice(), pInfo, &count, result.value.data());
  return result;
}

void vkDestroySurfaceKHR(VkSurfaceKHR surface, const VkAllocationCallbacks* pAllocator)
{
  return loader::GetFunctionTable().vkDestroySurfaceKHR(loader::GetInstance(), surface, pAllocator);
}

VulkanResult vkGetPhysicalDeviceSurfaceSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported)
{
  return loader::GetFunctionTable().vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, surface, pSupported);
}

VulkanResult vkGetPhysicalDeviceSurfaceCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities)
{
  return loader::GetFunctionTable().vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, pSurfaceCapabilities);
}

VulkanResultWithValue<std::vector<VkSurfaceFormatKHR>> vkGetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
  const auto& fp = loader::GetFunctionTable().vkGetPhysicalDeviceSurfaceFormatsKHR;
  uint32_t count = 0;
  fp(physicalDevice, surface, &count, nullptr);
  VulkanResultWithValue<std::vector<VkSurfaceFormatKHR>> result;
  result.value.resize(count);
  result.result = fp(physicalDevice, surface, &count, result.value.data());
  return result;
}

VulkanResultWithValue<std::vector<VkPresentModeKHR>> vkGetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
  const auto& fp = loader::GetFunctionTable().vkGetPhysicalDeviceSurfacePresentModesKHR;
  uint32_t count = 0;
  fp(physicalDevice, surface, &count, nullptr);
  VulkanResultWithValue<std::vector<VkPresentModeKHR>> result;
  result.value.resize(count);
  result.result = fp(physicalDevice, surface, &count, result.value.data());
  return result;
}

VulkanResult vkCreateSwapchainKHR(const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain)
{
  return loader::GetFunctionTable().vkCreateSwapchainKHR(loader::GetDevice(), pCreateInfo, pAllocator, pSwapchain);
}

void vkDestroySwapchainKHR(VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator)
{
  return loader::GetFunctionTable().vkDestroySwapchainKHR(loader::GetDevice(), swapchain, pAllocator);
}

VulkanResultWithValue<std::vector<VkImage>> vkGetSwapchainImagesKHR(VkSwapchainKHR swapchain)
{
  const auto& fp = loader::GetFunctionTable().vkGetSwapchainImagesKHR;
  uint32_t count = 0;
  fp(loader::GetDevice(), swapchain, &count, nullptr);
  VulkanResultWithValue<std::vector<VkImage>> result;
  result.value.resize(count);
  result.result = fp(loader::GetDevice(), swapchain, &count, result.value.data());
  return result;
}

VulkanResult vkAcquireNextImageKHR(VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex)
{
  return loader::GetFunctionTable().vkAcquireNextImageKHR(loader::GetDevice(), swapchain, timeout, semaphore, fence, pImageIndex);
}

VulkanResult vkQueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo)
{
  return loader::GetFunctionTable().vkQueuePresentKHR(queue, pPresentInfo);
}

VulkanResult vkGetDeviceGroupPresentCapabilitiesKHR(VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities)
{
  return loader::GetFunctionTable().vkGetDeviceGroupPresentCapabilitiesKHR(loader::GetDevice(), pDeviceGroupPresentCapabilities);
}

VulkanResult vkGetDeviceGroupSurfacePresentModesKHR(VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes)
{
  return loader::GetFunctionTable().vkGetDeviceGroupSurfacePresentModesKHR(loader::GetDevice(), surface, pModes);
}

VulkanResultWithValue<std::vector<VkRect2D>> vkGetPhysicalDevicePresentRectanglesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
  const auto& fp = loader::GetFunctionTable().vkGetPhysicalDevicePresentRectanglesKHR;
  uint32_t count = 0;
  fp(physicalDevice, surface, &count, nullptr);
  VulkanResultWithValue<std::vector<VkRect2D>> result;
  result.value.resize(count);
  result.result = fp(physicalDevice, surface, &count, result.value.data());
  return result;
}

VulkanResult vkAcquireNextImage2KHR(const VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex)
{
  return loader::GetFunctionTable().vkAcquireNextImage2KHR(loader::GetDevice(), pAcquireInfo, pImageIndex);
}

VulkanResultWithValue<std::vector<VkDisplayPropertiesKHR>> vkGetPhysicalDeviceDisplayPropertiesKHR(VkPhysicalDevice physicalDevice)
{
  const auto& fp = loader::GetFunctionTable().vkGetPhysicalDeviceDisplayPropertiesKHR;
  uint32_t count = 0;
  fp(physicalDevice, &count, nullptr);
  VulkanResultWithValue<std::vector<VkDisplayPropertiesKHR>> result;
  result.value.resize(count);
  result.result = fp(physicalDevice, &count, result.value.data());
  return result;
}

VulkanResultWithValue<std::vector<VkDisplayPlanePropertiesKHR>> vkGetPhysicalDeviceDisplayPlanePropertiesKHR(VkPhysicalDevice physicalDevice)
{
  const auto& fp = loader::GetFunctionTable().vkGetPhysicalDeviceDisplayPlanePropertiesKHR;
  uint32_t count = 0;
  fp(physicalDevice, &count, nullptr);
  VulkanResultWithValue<std::vector<VkDisplayPlanePropertiesKHR>> result;
  result.value.resize(count);
  result.result = fp(physicalDevice, &count, result.value.data());
  return result;
}

VulkanResultWithValue<std::vector<VkDisplayKHR>> vkGetDisplayPlaneSupportedDisplaysKHR(VkPhysicalDevice physicalDevice, uint32_t planeIndex)
{
  const auto& fp = loader::GetFunctionTable().vkGetDisplayPlaneSupportedDisplaysKHR;
  uint32_t count = 0;
  fp(physicalDevice, planeIndex, &count, nullptr);
  VulkanResultWithValue<std::vector<VkDisplayKHR>> result;
  result.value.resize(count);
  result.result = fp(physicalDevice, planeIndex, &count, result.value.data());
  return result;
}

VulkanResultWithValue<std::vector<VkDisplayModePropertiesKHR>> vkGetDisplayModePropertiesKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display)
{
  const auto& fp = loader::GetFunctionTable().vkGetDisplayModePropertiesKHR;
  uint32_t count = 0;
  fp(physicalDevice, display, &count, nullptr);
  VulkanResultWithValue<std::vector<VkDisplayModePropertiesKHR>> result;
  result.value.resize(count);
  result.result = fp(physicalDevice, display, &count, result.value.data());
  return result;
}

VulkanResult vkCreateDisplayModeKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, const VkDisplayModeCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDisplayModeKHR* pMode)
{
  return loader::GetFunctionTable().vkCreateDisplayModeKHR(physicalDevice, display, pCreateInfo, pAllocator, pMode);
}

VulkanResult vkGetDisplayPlaneCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkDisplayModeKHR mode, uint32_t planeIndex, VkDisplayPlaneCapabilitiesKHR* pCapabilities)
{
  return loader::GetFunctionTable().vkGetDisplayPlaneCapabilitiesKHR(physicalDevice, mode, planeIndex, pCapabilities);
}

VulkanResult vkCreateDisplayPlaneSurfaceKHR(const VkDisplaySurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
  return loader::GetFunctionTable().vkCreateDisplayPlaneSurfaceKHR(loader::GetInstance(), pCreateInfo, pAllocator, pSurface);
}

VulkanResult vkCreateSharedSwapchainsKHR(uint32_t swapchainCount, const VkSwapchainCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchains)
{
  return loader::GetFunctionTable().vkCreateSharedSwapchainsKHR(loader::GetDevice(), swapchainCount, pCreateInfos, pAllocator, pSwapchains);
}

void vkGetPhysicalDeviceFeatures2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures)
{
  return loader::GetFunctionTable().vkGetPhysicalDeviceFeatures2KHR(physicalDevice, pFeatures);
}

void vkGetPhysicalDeviceProperties2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties)
{
  return loader::GetFunctionTable().vkGetPhysicalDeviceProperties2KHR(physicalDevice, pProperties);
}

void vkGetPhysicalDeviceFormatProperties2KHR(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties)
{
  return loader::GetFunctionTable().vkGetPhysicalDeviceFormatProperties2KHR(physicalDevice, format, pFormatProperties);
}

VulkanResult vkGetPhysicalDeviceImageFormatProperties2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties)
{
  return loader::GetFunctionTable().vkGetPhysicalDeviceImageFormatProperties2KHR(physicalDevice, pImageFormatInfo, pImageFormatProperties);
}

VulkanResultWithValue<std::vector<VkQueueFamilyProperties2>> vkGetPhysicalDeviceQueueFamilyProperties2KHR(VkPhysicalDevice physicalDevice)
{
  const auto& fp = loader::GetFunctionTable().vkGetPhysicalDeviceQueueFamilyProperties2KHR;
  uint32_t count = 0;
  fp(physicalDevice, &count, nullptr);
  VulkanResultWithValue<std::vector<VkQueueFamilyProperties2>> result;
  result.value.resize(count);
  fp(physicalDevice, &count, result.value.data());
  return result;
}

void vkGetPhysicalDeviceMemoryProperties2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties)
{
  return loader::GetFunctionTable().vkGetPhysicalDeviceMemoryProperties2KHR(physicalDevice, pMemoryProperties);
}

VulkanResultWithValue<std::vector<VkSparseImageFormatProperties2>> vkGetPhysicalDeviceSparseImageFormatProperties2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo)
{
  const auto& fp = loader::GetFunctionTable().vkGetPhysicalDeviceSparseImageFormatProperties2KHR;
  uint32_t count = 0;
  fp(physicalDevice, pFormatInfo, &count, nullptr);
  VulkanResultWithValue<std::vector<VkSparseImageFormatProperties2>> result;
  result.value.resize(count);
  fp(physicalDevice, pFormatInfo, &count, result.value.data());
  return result;
}

void vkGetDeviceGroupPeerMemoryFeaturesKHR(uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures)
{
  return loader::GetFunctionTable().vkGetDeviceGroupPeerMemoryFeaturesKHR(loader::GetDevice(), heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
}

void vkTrimCommandPoolKHR(VkCommandPool commandPool, VkCommandPoolTrimFlags flags)
{
  return loader::GetFunctionTable().vkTrimCommandPoolKHR(loader::GetDevice(), commandPool, flags);
}

VulkanResultWithValue<std::vector<VkPhysicalDeviceGroupProperties>> vkEnumeratePhysicalDeviceGroupsKHR()
{
  const auto& fp = loader::GetFunctionTable().vkEnumeratePhysicalDeviceGroupsKHR;
  uint32_t count = 0;
  fp(loader::GetInstance(), &count, nullptr);
  VulkanResultWithValue<std::vector<VkPhysicalDeviceGroupProperties>> result;
  result.value.resize(count);
  result.result = fp(loader::GetInstance(), &count, result.value.data());
  return result;
}

void vkGetPhysicalDeviceExternalBufferPropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties)
{
  return loader::GetFunctionTable().vkGetPhysicalDeviceExternalBufferPropertiesKHR(physicalDevice, pExternalBufferInfo, pExternalBufferProperties);
}

VulkanResult vkGetMemoryFdKHR(const VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd)
{
  return loader::GetFunctionTable().vkGetMemoryFdKHR(loader::GetDevice(), pGetFdInfo, pFd);
}

VulkanResult vkGetMemoryFdPropertiesKHR(VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties)
{
  return loader::GetFunctionTable().vkGetMemoryFdPropertiesKHR(loader::GetDevice(), handleType, fd, pMemoryFdProperties);
}

void vkGetPhysicalDeviceExternalSemaphorePropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties)
{
  return loader::GetFunctionTable().vkGetPhysicalDeviceExternalSemaphorePropertiesKHR(physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
}

VulkanResult vkImportSemaphoreFdKHR(const VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo)
{
  return loader::GetFunctionTable().vkImportSemaphoreFdKHR(loader::GetDevice(), pImportSemaphoreFdInfo);
}

VulkanResult vkGetSemaphoreFdKHR(const VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd)
{
  return loader::GetFunctionTable().vkGetSemaphoreFdKHR(loader::GetDevice(), pGetFdInfo, pFd);
}

VulkanResult vkCreateDescriptorUpdateTemplateKHR(const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate)
{
  return loader::GetFunctionTable().vkCreateDescriptorUpdateTemplateKHR(loader::GetDevice(), pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
}

void vkDestroyDescriptorUpdateTemplateKHR(VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator)
{
  return loader::GetFunctionTable().vkDestroyDescriptorUpdateTemplateKHR(loader::GetDevice(), descriptorUpdateTemplate, pAllocator);
}

void vkUpdateDescriptorSetWithTemplateKHR(VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData)
{
  return loader::GetFunctionTable().vkUpdateDescriptorSetWithTemplateKHR(loader::GetDevice(), descriptorSet, descriptorUpdateTemplate, pData);
}

VulkanResult vkCreateRenderPass2KHR(const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass)
{
  return loader::GetFunctionTable().vkCreateRenderPass2KHR(loader::GetDevice(), pCreateInfo, pAllocator, pRenderPass);
}

VulkanResult vkGetSwapchainStatusKHR(VkSwapchainKHR swapchain)
{
  return loader::GetFunctionTable().vkGetSwapchainStatusKHR(loader::GetDevice(), swapchain);
}

void vkGetPhysicalDeviceExternalFencePropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties)
{
  return loader::GetFunctionTable().vkGetPhysicalDeviceExternalFencePropertiesKHR(physicalDevice, pExternalFenceInfo, pExternalFenceProperties);
}

VulkanResult vkImportFenceFdKHR(const VkImportFenceFdInfoKHR* pImportFenceFdInfo)
{
  return loader::GetFunctionTable().vkImportFenceFdKHR(loader::GetDevice(), pImportFenceFdInfo);
}

VulkanResult vkGetFenceFdKHR(const VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd)
{
  return loader::GetFunctionTable().vkGetFenceFdKHR(loader::GetDevice(), pGetFdInfo, pFd);
}

VulkanResult vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, uint32_t* pCounterCount, VkPerformanceCounterKHR* pCounters, VkPerformanceCounterDescriptionKHR* pCounterDescriptions)
{
  return loader::GetFunctionTable().vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(physicalDevice, queueFamilyIndex, pCounterCount, pCounters, pCounterDescriptions);
}

void vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(VkPhysicalDevice physicalDevice, const VkQueryPoolPerformanceCreateInfoKHR* pPerformanceQueryCreateInfo, uint32_t* pNumPasses)
{
  return loader::GetFunctionTable().vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(physicalDevice, pPerformanceQueryCreateInfo, pNumPasses);
}

VulkanResult vkAcquireProfilingLockKHR(const VkAcquireProfilingLockInfoKHR* pInfo)
{
  return loader::GetFunctionTable().vkAcquireProfilingLockKHR(loader::GetDevice(), pInfo);
}

void vkReleaseProfilingLockKHR()
{
  return loader::GetFunctionTable().vkReleaseProfilingLockKHR(loader::GetDevice());
}

VulkanResult vkGetPhysicalDeviceSurfaceCapabilities2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkSurfaceCapabilities2KHR* pSurfaceCapabilities)
{
  return loader::GetFunctionTable().vkGetPhysicalDeviceSurfaceCapabilities2KHR(physicalDevice, pSurfaceInfo, pSurfaceCapabilities);
}

VulkanResultWithValue<std::vector<VkSurfaceFormat2KHR>> vkGetPhysicalDeviceSurfaceFormats2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo)
{
  const auto& fp = loader::GetFunctionTable().vkGetPhysicalDeviceSurfaceFormats2KHR;
  uint32_t count = 0;
  fp(physicalDevice, pSurfaceInfo, &count, nullptr);
  VulkanResultWithValue<std::vector<VkSurfaceFormat2KHR>> result;
  result.value.resize(count);
  result.result = fp(physicalDevice, pSurfaceInfo, &count, result.value.data());
  return result;
}

VulkanResultWithValue<std::vector<VkDisplayProperties2KHR>> vkGetPhysicalDeviceDisplayProperties2KHR(VkPhysicalDevice physicalDevice)
{
  const auto& fp = loader::GetFunctionTable().vkGetPhysicalDeviceDisplayProperties2KHR;
  uint32_t count = 0;
  fp(physicalDevice, &count, nullptr);
  VulkanResultWithValue<std::vector<VkDisplayProperties2KHR>> result;
  result.value.resize(count);
  result.result = fp(physicalDevice, &count, result.value.data());
  return result;
}

VulkanResultWithValue<std::vector<VkDisplayPlaneProperties2KHR>> vkGetPhysicalDeviceDisplayPlaneProperties2KHR(VkPhysicalDevice physicalDevice)
{
  const auto& fp = loader::GetFunctionTable().vkGetPhysicalDeviceDisplayPlaneProperties2KHR;
  uint32_t count = 0;
  fp(physicalDevice, &count, nullptr);
  VulkanResultWithValue<std::vector<VkDisplayPlaneProperties2KHR>> result;
  result.value.resize(count);
  result.result = fp(physicalDevice, &count, result.value.data());
  return result;
}

VulkanResultWithValue<std::vector<VkDisplayModeProperties2KHR>> vkGetDisplayModeProperties2KHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display)
{
  const auto& fp = loader::GetFunctionTable().vkGetDisplayModeProperties2KHR;
  uint32_t count = 0;
  fp(physicalDevice, display, &count, nullptr);
  VulkanResultWithValue<std::vector<VkDisplayModeProperties2KHR>> result;
  result.value.resize(count);
  result.result = fp(physicalDevice, display, &count, result.value.data());
  return result;
}

VulkanResult vkGetDisplayPlaneCapabilities2KHR(VkPhysicalDevice physicalDevice, const VkDisplayPlaneInfo2KHR* pDisplayPlaneInfo, VkDisplayPlaneCapabilities2KHR* pCapabilities)
{
  return loader::GetFunctionTable().vkGetDisplayPlaneCapabilities2KHR(physicalDevice, pDisplayPlaneInfo, pCapabilities);
}

void vkGetImageMemoryRequirements2KHR(const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
  return loader::GetFunctionTable().vkGetImageMemoryRequirements2KHR(loader::GetDevice(), pInfo, pMemoryRequirements);
}

void vkGetBufferMemoryRequirements2KHR(const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
  return loader::GetFunctionTable().vkGetBufferMemoryRequirements2KHR(loader::GetDevice(), pInfo, pMemoryRequirements);
}

VulkanResultWithValue<std::vector<VkSparseImageMemoryRequirements2>> vkGetImageSparseMemoryRequirements2KHR(const VkImageSparseMemoryRequirementsInfo2* pInfo)
{
  const auto& fp = loader::GetFunctionTable().vkGetImageSparseMemoryRequirements2KHR;
  uint32_t count = 0;
  fp(loader::GetDevice(), pInfo, &count, nullptr);
  VulkanResultWithValue<std::vector<VkSparseImageMemoryRequirements2>> result;
  result.value.resize(count);
  fp(loader::GetDevice(), pInfo, &count, result.value.data());
  return result;
}

VulkanResult vkCreateSamplerYcbcrConversionKHR(const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion)
{
  return loader::GetFunctionTable().vkCreateSamplerYcbcrConversionKHR(loader::GetDevice(), pCreateInfo, pAllocator, pYcbcrConversion);
}

void vkDestroySamplerYcbcrConversionKHR(VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator)
{
  return loader::GetFunctionTable().vkDestroySamplerYcbcrConversionKHR(loader::GetDevice(), ycbcrConversion, pAllocator);
}

VulkanResult vkBindBufferMemory2KHR(uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos)
{
  return loader::GetFunctionTable().vkBindBufferMemory2KHR(loader::GetDevice(), bindInfoCount, pBindInfos);
}

VulkanResult vkBindImageMemory2KHR(uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos)
{
  return loader::GetFunctionTable().vkBindImageMemory2KHR(loader::GetDevice(), bindInfoCount, pBindInfos);
}

void vkGetDescriptorSetLayoutSupportKHR(const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport)
{
  return loader::GetFunctionTable().vkGetDescriptorSetLayoutSupportKHR(loader::GetDevice(), pCreateInfo, pSupport);
}

VulkanResult vkGetSemaphoreCounterValueKHR(VkSemaphore semaphore, uint64_t* pValue)
{
  return loader::GetFunctionTable().vkGetSemaphoreCounterValueKHR(loader::GetDevice(), semaphore, pValue);
}

VulkanResult vkWaitSemaphoresKHR(const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout)
{
  return loader::GetFunctionTable().vkWaitSemaphoresKHR(loader::GetDevice(), pWaitInfo, timeout);
}

VulkanResult vkSignalSemaphoreKHR(const VkSemaphoreSignalInfo* pSignalInfo)
{
  return loader::GetFunctionTable().vkSignalSemaphoreKHR(loader::GetDevice(), pSignalInfo);
}

VulkanResultWithValue<std::vector<VkPhysicalDeviceFragmentShadingRateKHR>> vkGetPhysicalDeviceFragmentShadingRatesKHR(VkPhysicalDevice physicalDevice)
{
  const auto& fp = loader::GetFunctionTable().vkGetPhysicalDeviceFragmentShadingRatesKHR;
  uint32_t count = 0;
  fp(physicalDevice, &count, nullptr);
  VulkanResultWithValue<std::vector<VkPhysicalDeviceFragmentShadingRateKHR>> result;
  result.value.resize(count);
  result.result = fp(physicalDevice, &count, result.value.data());
  return result;
}

VulkanResult vkWaitForPresentKHR(VkSwapchainKHR swapchain, uint64_t presentId, uint64_t timeout)
{
  return loader::GetFunctionTable().vkWaitForPresentKHR(loader::GetDevice(), swapchain, presentId, timeout);
}

VulkanResultWithValue<VkDeviceAddress> vkGetBufferDeviceAddressKHR(const VkBufferDeviceAddressInfo* pInfo)
{
  return loader::GetFunctionTable().vkGetBufferDeviceAddressKHR(loader::GetDevice(), pInfo);
}

VulkanResultWithValue<uint64_t> vkGetBufferOpaqueCaptureAddressKHR(const VkBufferDeviceAddressInfo* pInfo)
{
  return loader::GetFunctionTable().vkGetBufferOpaqueCaptureAddressKHR(loader::GetDevice(), pInfo);
}

VulkanResultWithValue<uint64_t> vkGetDeviceMemoryOpaqueCaptureAddressKHR(const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo)
{
  return loader::GetFunctionTable().vkGetDeviceMemoryOpaqueCaptureAddressKHR(loader::GetDevice(), pInfo);
}

VulkanResult vkCreateDeferredOperationKHR(const VkAllocationCallbacks* pAllocator, VkDeferredOperationKHR* pDeferredOperation)
{
  return loader::GetFunctionTable().vkCreateDeferredOperationKHR(loader::GetDevice(), pAllocator, pDeferredOperation);
}

void vkDestroyDeferredOperationKHR(VkDeferredOperationKHR operation, const VkAllocationCallbacks* pAllocator)
{
  return loader::GetFunctionTable().vkDestroyDeferredOperationKHR(loader::GetDevice(), operation, pAllocator);
}

VulkanResultWithValue<uint32_t> vkGetDeferredOperationMaxConcurrencyKHR(VkDeferredOperationKHR operation)
{
  return loader::GetFunctionTable().vkGetDeferredOperationMaxConcurrencyKHR(loader::GetDevice(), operation);
}

VulkanResult vkGetDeferredOperationResultKHR(VkDeferredOperationKHR operation)
{
  return loader::GetFunctionTable().vkGetDeferredOperationResultKHR(loader::GetDevice(), operation);
}

VulkanResult vkDeferredOperationJoinKHR(VkDeferredOperationKHR operation)
{
  return loader::GetFunctionTable().vkDeferredOperationJoinKHR(loader::GetDevice(), operation);
}

VulkanResultWithValue<std::vector<VkPipelineExecutablePropertiesKHR>> vkGetPipelineExecutablePropertiesKHR(const VkPipelineInfoKHR* pPipelineInfo)
{
  const auto& fp = loader::GetFunctionTable().vkGetPipelineExecutablePropertiesKHR;
  uint32_t count = 0;
  fp(loader::GetDevice(), pPipelineInfo, &count, nullptr);
  VulkanResultWithValue<std::vector<VkPipelineExecutablePropertiesKHR>> result;
  result.value.resize(count);
  result.result = fp(loader::GetDevice(), pPipelineInfo, &count, result.value.data());
  return result;
}

VulkanResultWithValue<std::vector<VkPipelineExecutableStatisticKHR>> vkGetPipelineExecutableStatisticsKHR(const VkPipelineExecutableInfoKHR* pExecutableInfo)
{
  const auto& fp = loader::GetFunctionTable().vkGetPipelineExecutableStatisticsKHR;
  uint32_t count = 0;
  fp(loader::GetDevice(), pExecutableInfo, &count, nullptr);
  VulkanResultWithValue<std::vector<VkPipelineExecutableStatisticKHR>> result;
  result.value.resize(count);
  result.result = fp(loader::GetDevice(), pExecutableInfo, &count, result.value.data());
  return result;
}

VulkanResultWithValue<std::vector<VkPipelineExecutableInternalRepresentationKHR>> vkGetPipelineExecutableInternalRepresentationsKHR(const VkPipelineExecutableInfoKHR* pExecutableInfo)
{
  const auto& fp = loader::GetFunctionTable().vkGetPipelineExecutableInternalRepresentationsKHR;
  uint32_t count = 0;
  fp(loader::GetDevice(), pExecutableInfo, &count, nullptr);
  VulkanResultWithValue<std::vector<VkPipelineExecutableInternalRepresentationKHR>> result;
  result.value.resize(count);
  result.result = fp(loader::GetDevice(), pExecutableInfo, &count, result.value.data());
  return result;
}

VulkanResult vkQueueSubmit2KHR(VkQueue queue, uint32_t submitCount, const VkSubmitInfo2* pSubmits, VkFence fence)
{
  return loader::GetFunctionTable().vkQueueSubmit2KHR(queue, submitCount, pSubmits, fence);
}

VulkanResultWithValue<std::vector<VkCheckpointData2NV>> vkGetQueueCheckpointData2NV(VkQueue queue)
{
  const auto& fp = loader::GetFunctionTable().vkGetQueueCheckpointData2NV;
  uint32_t count = 0;
  fp(queue, &count, nullptr);
  VulkanResultWithValue<std::vector<VkCheckpointData2NV>> result;
  result.value.resize(count);
  fp(queue, &count, result.value.data());
  return result;
}

void vkGetDeviceBufferMemoryRequirementsKHR(const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
  return loader::GetFunctionTable().vkGetDeviceBufferMemoryRequirementsKHR(loader::GetDevice(), pInfo, pMemoryRequirements);
}

void vkGetDeviceImageMemoryRequirementsKHR(const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
  return loader::GetFunctionTable().vkGetDeviceImageMemoryRequirementsKHR(loader::GetDevice(), pInfo, pMemoryRequirements);
}

VulkanResultWithValue<std::vector<VkSparseImageMemoryRequirements2>> vkGetDeviceImageSparseMemoryRequirementsKHR(const VkDeviceImageMemoryRequirements* pInfo)
{
  const auto& fp = loader::GetFunctionTable().vkGetDeviceImageSparseMemoryRequirementsKHR;
  uint32_t count = 0;
  fp(loader::GetDevice(), pInfo, &count, nullptr);
  VulkanResultWithValue<std::vector<VkSparseImageMemoryRequirements2>> result;
  result.value.resize(count);
  fp(loader::GetDevice(), pInfo, &count, result.value.data());
  return result;
}

VulkanResult vkCreateDebugReportCallbackEXT(const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback)
{
  return loader::GetFunctionTable().vkCreateDebugReportCallbackEXT(loader::GetInstance(), pCreateInfo, pAllocator, pCallback);
}

void vkDestroyDebugReportCallbackEXT(VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator)
{
  return loader::GetFunctionTable().vkDestroyDebugReportCallbackEXT(loader::GetInstance(), callback, pAllocator);
}

void vkDebugReportMessageEXT(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage)
{
  return loader::GetFunctionTable().vkDebugReportMessageEXT(loader::GetInstance(), flags, objectType, object, location, messageCode, pLayerPrefix, pMessage);
}

VulkanResult vkDebugMarkerSetObjectTagEXT(const VkDebugMarkerObjectTagInfoEXT* pTagInfo)
{
  return loader::GetFunctionTable().vkDebugMarkerSetObjectTagEXT(loader::GetDevice(), pTagInfo);
}

VulkanResult vkDebugMarkerSetObjectNameEXT(const VkDebugMarkerObjectNameInfoEXT* pNameInfo)
{
  return loader::GetFunctionTable().vkDebugMarkerSetObjectNameEXT(loader::GetDevice(), pNameInfo);
}

VulkanResult vkCreateCuModuleNVX(const VkCuModuleCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuModuleNVX* pModule)
{
  return loader::GetFunctionTable().vkCreateCuModuleNVX(loader::GetDevice(), pCreateInfo, pAllocator, pModule);
}

VulkanResult vkCreateCuFunctionNVX(const VkCuFunctionCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuFunctionNVX* pFunction)
{
  return loader::GetFunctionTable().vkCreateCuFunctionNVX(loader::GetDevice(), pCreateInfo, pAllocator, pFunction);
}

void vkDestroyCuModuleNVX(VkCuModuleNVX module, const VkAllocationCallbacks* pAllocator)
{
  return loader::GetFunctionTable().vkDestroyCuModuleNVX(loader::GetDevice(), module, pAllocator);
}

void vkDestroyCuFunctionNVX(VkCuFunctionNVX function, const VkAllocationCallbacks* pAllocator)
{
  return loader::GetFunctionTable().vkDestroyCuFunctionNVX(loader::GetDevice(), function, pAllocator);
}

VulkanResultWithValue<uint32_t> vkGetImageViewHandleNVX(const VkImageViewHandleInfoNVX* pInfo)
{
  return loader::GetFunctionTable().vkGetImageViewHandleNVX(loader::GetDevice(), pInfo);
}

VulkanResult vkGetImageViewAddressNVX(VkImageView imageView, VkImageViewAddressPropertiesNVX* pProperties)
{
  return loader::GetFunctionTable().vkGetImageViewAddressNVX(loader::GetDevice(), imageView, pProperties);
}

VulkanResult vkGetShaderInfoAMD(VkPipeline pipeline, VkShaderStageFlagBits shaderStage, VkShaderInfoTypeAMD infoType, size_t* pInfoSize, void* pInfo)
{
  return loader::GetFunctionTable().vkGetShaderInfoAMD(loader::GetDevice(), pipeline, shaderStage, infoType, pInfoSize, pInfo);
}

VulkanResult vkGetPhysicalDeviceExternalImageFormatPropertiesNV(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkExternalMemoryHandleTypeFlagsNV externalHandleType, VkExternalImageFormatPropertiesNV* pExternalImageFormatProperties)
{
  return loader::GetFunctionTable().vkGetPhysicalDeviceExternalImageFormatPropertiesNV(physicalDevice, format, type, tiling, usage, flags, externalHandleType, pExternalImageFormatProperties);
}

VulkanResult vkReleaseDisplayEXT(VkPhysicalDevice physicalDevice, VkDisplayKHR display)
{
  return loader::GetFunctionTable().vkReleaseDisplayEXT(physicalDevice, display);
}

VulkanResult vkGetPhysicalDeviceSurfaceCapabilities2EXT(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilities2EXT* pSurfaceCapabilities)
{
  return loader::GetFunctionTable().vkGetPhysicalDeviceSurfaceCapabilities2EXT(physicalDevice, surface, pSurfaceCapabilities);
}

VulkanResult vkDisplayPowerControlEXT(VkDisplayKHR display, const VkDisplayPowerInfoEXT* pDisplayPowerInfo)
{
  return loader::GetFunctionTable().vkDisplayPowerControlEXT(loader::GetDevice(), display, pDisplayPowerInfo);
}

VulkanResult vkRegisterDeviceEventEXT(const VkDeviceEventInfoEXT* pDeviceEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence)
{
  return loader::GetFunctionTable().vkRegisterDeviceEventEXT(loader::GetDevice(), pDeviceEventInfo, pAllocator, pFence);
}

VulkanResult vkRegisterDisplayEventEXT(VkDisplayKHR display, const VkDisplayEventInfoEXT* pDisplayEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence)
{
  return loader::GetFunctionTable().vkRegisterDisplayEventEXT(loader::GetDevice(), display, pDisplayEventInfo, pAllocator, pFence);
}

VulkanResult vkGetSwapchainCounterEXT(VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue)
{
  return loader::GetFunctionTable().vkGetSwapchainCounterEXT(loader::GetDevice(), swapchain, counter, pCounterValue);
}

VulkanResult vkGetRefreshCycleDurationGOOGLE(VkSwapchainKHR swapchain, VkRefreshCycleDurationGOOGLE* pDisplayTimingProperties)
{
  return loader::GetFunctionTable().vkGetRefreshCycleDurationGOOGLE(loader::GetDevice(), swapchain, pDisplayTimingProperties);
}

VulkanResultWithValue<std::vector<VkPastPresentationTimingGOOGLE>> vkGetPastPresentationTimingGOOGLE(VkSwapchainKHR swapchain)
{
  const auto& fp = loader::GetFunctionTable().vkGetPastPresentationTimingGOOGLE;
  uint32_t count = 0;
  fp(loader::GetDevice(), swapchain, &count, nullptr);
  VulkanResultWithValue<std::vector<VkPastPresentationTimingGOOGLE>> result;
  result.value.resize(count);
  result.result = fp(loader::GetDevice(), swapchain, &count, result.value.data());
  return result;
}

void vkSetHdrMetadataEXT(uint32_t swapchainCount, const VkSwapchainKHR* pSwapchains, const VkHdrMetadataEXT* pMetadata)
{
  return loader::GetFunctionTable().vkSetHdrMetadataEXT(loader::GetDevice(), swapchainCount, pSwapchains, pMetadata);
}

VulkanResult vkSetDebugUtilsObjectNameEXT(const VkDebugUtilsObjectNameInfoEXT* pNameInfo)
{
  return loader::GetFunctionTable().vkSetDebugUtilsObjectNameEXT(loader::GetDevice(), pNameInfo);
}

VulkanResult vkSetDebugUtilsObjectTagEXT(const VkDebugUtilsObjectTagInfoEXT* pTagInfo)
{
  return loader::GetFunctionTable().vkSetDebugUtilsObjectTagEXT(loader::GetDevice(), pTagInfo);
}

void vkQueueBeginDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo)
{
  return loader::GetFunctionTable().vkQueueBeginDebugUtilsLabelEXT(queue, pLabelInfo);
}

void vkQueueEndDebugUtilsLabelEXT(VkQueue queue)
{
  return loader::GetFunctionTable().vkQueueEndDebugUtilsLabelEXT(queue);
}

void vkQueueInsertDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo)
{
  return loader::GetFunctionTable().vkQueueInsertDebugUtilsLabelEXT(queue, pLabelInfo);
}

VulkanResult vkCreateDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger)
{
  return loader::GetFunctionTable().vkCreateDebugUtilsMessengerEXT(loader::GetInstance(), pCreateInfo, pAllocator, pMessenger);
}

void vkDestroyDebugUtilsMessengerEXT(VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks* pAllocator)
{
  return loader::GetFunctionTable().vkDestroyDebugUtilsMessengerEXT(loader::GetInstance(), messenger, pAllocator);
}

void vkSubmitDebugUtilsMessageEXT(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData)
{
  return loader::GetFunctionTable().vkSubmitDebugUtilsMessageEXT(loader::GetInstance(), messageSeverity, messageTypes, pCallbackData);
}

void vkGetPhysicalDeviceMultisamplePropertiesEXT(VkPhysicalDevice physicalDevice, VkSampleCountFlagBits samples, VkMultisamplePropertiesEXT* pMultisampleProperties)
{
  return loader::GetFunctionTable().vkGetPhysicalDeviceMultisamplePropertiesEXT(physicalDevice, samples, pMultisampleProperties);
}

VulkanResult vkGetImageDrmFormatModifierPropertiesEXT(VkImage image, VkImageDrmFormatModifierPropertiesEXT* pProperties)
{
  return loader::GetFunctionTable().vkGetImageDrmFormatModifierPropertiesEXT(loader::GetDevice(), image, pProperties);
}

VulkanResult vkCreateValidationCacheEXT(const VkValidationCacheCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkValidationCacheEXT* pValidationCache)
{
  return loader::GetFunctionTable().vkCreateValidationCacheEXT(loader::GetDevice(), pCreateInfo, pAllocator, pValidationCache);
}

void vkDestroyValidationCacheEXT(VkValidationCacheEXT validationCache, const VkAllocationCallbacks* pAllocator)
{
  return loader::GetFunctionTable().vkDestroyValidationCacheEXT(loader::GetDevice(), validationCache, pAllocator);
}

VulkanResult vkMergeValidationCachesEXT(VkValidationCacheEXT dstCache, uint32_t srcCacheCount, const VkValidationCacheEXT* pSrcCaches)
{
  return loader::GetFunctionTable().vkMergeValidationCachesEXT(loader::GetDevice(), dstCache, srcCacheCount, pSrcCaches);
}

VulkanResult vkGetValidationCacheDataEXT(VkValidationCacheEXT validationCache, size_t* pDataSize, void* pData)
{
  return loader::GetFunctionTable().vkGetValidationCacheDataEXT(loader::GetDevice(), validationCache, pDataSize, pData);
}

VulkanResult vkCreateAccelerationStructureNV(const VkAccelerationStructureCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkAccelerationStructureNV* pAccelerationStructure)
{
  return loader::GetFunctionTable().vkCreateAccelerationStructureNV(loader::GetDevice(), pCreateInfo, pAllocator, pAccelerationStructure);
}

void vkDestroyAccelerationStructureNV(VkAccelerationStructureNV accelerationStructure, const VkAllocationCallbacks* pAllocator)
{
  return loader::GetFunctionTable().vkDestroyAccelerationStructureNV(loader::GetDevice(), accelerationStructure, pAllocator);
}

void vkGetAccelerationStructureMemoryRequirementsNV(const VkAccelerationStructureMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2KHR* pMemoryRequirements)
{
  return loader::GetFunctionTable().vkGetAccelerationStructureMemoryRequirementsNV(loader::GetDevice(), pInfo, pMemoryRequirements);
}

VulkanResult vkBindAccelerationStructureMemoryNV(uint32_t bindInfoCount, const VkBindAccelerationStructureMemoryInfoNV* pBindInfos)
{
  return loader::GetFunctionTable().vkBindAccelerationStructureMemoryNV(loader::GetDevice(), bindInfoCount, pBindInfos);
}

VulkanResult vkCreateRayTracingPipelinesNV(VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoNV* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
{
  return loader::GetFunctionTable().vkCreateRayTracingPipelinesNV(loader::GetDevice(), pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}

VulkanResult vkGetRayTracingShaderGroupHandlesKHR(VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData)
{
  return loader::GetFunctionTable().vkGetRayTracingShaderGroupHandlesKHR(loader::GetDevice(), pipeline, firstGroup, groupCount, dataSize, pData);
}

VulkanResult vkGetRayTracingShaderGroupHandlesNV(VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData)
{
  return loader::GetFunctionTable().vkGetRayTracingShaderGroupHandlesNV(loader::GetDevice(), pipeline, firstGroup, groupCount, dataSize, pData);
}

VulkanResult vkGetAccelerationStructureHandleNV(VkAccelerationStructureNV accelerationStructure, size_t dataSize, void* pData)
{
  return loader::GetFunctionTable().vkGetAccelerationStructureHandleNV(loader::GetDevice(), accelerationStructure, dataSize, pData);
}

VulkanResult vkCompileDeferredNV(VkPipeline pipeline, uint32_t shader)
{
  return loader::GetFunctionTable().vkCompileDeferredNV(loader::GetDevice(), pipeline, shader);
}

VulkanResult vkGetMemoryHostPointerPropertiesEXT(VkExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties)
{
  return loader::GetFunctionTable().vkGetMemoryHostPointerPropertiesEXT(loader::GetDevice(), handleType, pHostPointer, pMemoryHostPointerProperties);
}

VulkanResultWithValue<std::vector<VkTimeDomainEXT>> vkGetPhysicalDeviceCalibrateableTimeDomainsEXT(VkPhysicalDevice physicalDevice)
{
  const auto& fp = loader::GetFunctionTable().vkGetPhysicalDeviceCalibrateableTimeDomainsEXT;
  uint32_t count = 0;
  fp(physicalDevice, &count, nullptr);
  VulkanResultWithValue<std::vector<VkTimeDomainEXT>> result;
  result.value.resize(count);
  result.result = fp(physicalDevice, &count, result.value.data());
  return result;
}

VulkanResult vkGetCalibratedTimestampsEXT(uint32_t timestampCount, const VkCalibratedTimestampInfoEXT* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation)
{
  return loader::GetFunctionTable().vkGetCalibratedTimestampsEXT(loader::GetDevice(), timestampCount, pTimestampInfos, pTimestamps, pMaxDeviation);
}

VulkanResultWithValue<std::vector<VkCheckpointDataNV>> vkGetQueueCheckpointDataNV(VkQueue queue)
{
  const auto& fp = loader::GetFunctionTable().vkGetQueueCheckpointDataNV;
  uint32_t count = 0;
  fp(queue, &count, nullptr);
  VulkanResultWithValue<std::vector<VkCheckpointDataNV>> result;
  result.value.resize(count);
  fp(queue, &count, result.value.data());
  return result;
}

VulkanResult vkInitializePerformanceApiINTEL(const VkInitializePerformanceApiInfoINTEL* pInitializeInfo)
{
  return loader::GetFunctionTable().vkInitializePerformanceApiINTEL(loader::GetDevice(), pInitializeInfo);
}

void vkUninitializePerformanceApiINTEL()
{
  return loader::GetFunctionTable().vkUninitializePerformanceApiINTEL(loader::GetDevice());
}

VulkanResult vkAcquirePerformanceConfigurationINTEL(const VkPerformanceConfigurationAcquireInfoINTEL* pAcquireInfo, VkPerformanceConfigurationINTEL* pConfiguration)
{
  return loader::GetFunctionTable().vkAcquirePerformanceConfigurationINTEL(loader::GetDevice(), pAcquireInfo, pConfiguration);
}

VulkanResult vkReleasePerformanceConfigurationINTEL(VkPerformanceConfigurationINTEL configuration)
{
  return loader::GetFunctionTable().vkReleasePerformanceConfigurationINTEL(loader::GetDevice(), configuration);
}

VulkanResult vkQueueSetPerformanceConfigurationINTEL(VkQueue queue, VkPerformanceConfigurationINTEL configuration)
{
  return loader::GetFunctionTable().vkQueueSetPerformanceConfigurationINTEL(queue, configuration);
}

VulkanResult vkGetPerformanceParameterINTEL(VkPerformanceParameterTypeINTEL parameter, VkPerformanceValueINTEL* pValue)
{
  return loader::GetFunctionTable().vkGetPerformanceParameterINTEL(loader::GetDevice(), parameter, pValue);
}

void vkSetLocalDimmingAMD(VkSwapchainKHR swapChain, VkBool32 localDimmingEnable)
{
  return loader::GetFunctionTable().vkSetLocalDimmingAMD(loader::GetDevice(), swapChain, localDimmingEnable);
}

VulkanResultWithValue<VkDeviceAddress> vkGetBufferDeviceAddressEXT(const VkBufferDeviceAddressInfo* pInfo)
{
  return loader::GetFunctionTable().vkGetBufferDeviceAddressEXT(loader::GetDevice(), pInfo);
}

VulkanResultWithValue<std::vector<VkPhysicalDeviceToolProperties>> vkGetPhysicalDeviceToolPropertiesEXT(VkPhysicalDevice physicalDevice)
{
  const auto& fp = loader::GetFunctionTable().vkGetPhysicalDeviceToolPropertiesEXT;
  uint32_t count = 0;
  fp(physicalDevice, &count, nullptr);
  VulkanResultWithValue<std::vector<VkPhysicalDeviceToolProperties>> result;
  result.value.resize(count);
  result.result = fp(physicalDevice, &count, result.value.data());
  return result;
}

VulkanResultWithValue<std::vector<VkCooperativeMatrixPropertiesNV>> vkGetPhysicalDeviceCooperativeMatrixPropertiesNV(VkPhysicalDevice physicalDevice)
{
  const auto& fp = loader::GetFunctionTable().vkGetPhysicalDeviceCooperativeMatrixPropertiesNV;
  uint32_t count = 0;
  fp(physicalDevice, &count, nullptr);
  VulkanResultWithValue<std::vector<VkCooperativeMatrixPropertiesNV>> result;
  result.value.resize(count);
  result.result = fp(physicalDevice, &count, result.value.data());
  return result;
}

VulkanResultWithValue<std::vector<VkFramebufferMixedSamplesCombinationNV>> vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(VkPhysicalDevice physicalDevice)
{
  const auto& fp = loader::GetFunctionTable().vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV;
  uint32_t count = 0;
  fp(physicalDevice, &count, nullptr);
  VulkanResultWithValue<std::vector<VkFramebufferMixedSamplesCombinationNV>> result;
  result.value.resize(count);
  result.result = fp(physicalDevice, &count, result.value.data());
  return result;
}

VulkanResult vkCreateHeadlessSurfaceEXT(const VkHeadlessSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
  return loader::GetFunctionTable().vkCreateHeadlessSurfaceEXT(loader::GetInstance(), pCreateInfo, pAllocator, pSurface);
}

void vkResetQueryPoolEXT(VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount)
{
  return loader::GetFunctionTable().vkResetQueryPoolEXT(loader::GetDevice(), queryPool, firstQuery, queryCount);
}

void vkGetGeneratedCommandsMemoryRequirementsNV(const VkGeneratedCommandsMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
  return loader::GetFunctionTable().vkGetGeneratedCommandsMemoryRequirementsNV(loader::GetDevice(), pInfo, pMemoryRequirements);
}

VulkanResult vkCreateIndirectCommandsLayoutNV(const VkIndirectCommandsLayoutCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectCommandsLayoutNV* pIndirectCommandsLayout)
{
  return loader::GetFunctionTable().vkCreateIndirectCommandsLayoutNV(loader::GetDevice(), pCreateInfo, pAllocator, pIndirectCommandsLayout);
}

void vkDestroyIndirectCommandsLayoutNV(VkIndirectCommandsLayoutNV indirectCommandsLayout, const VkAllocationCallbacks* pAllocator)
{
  return loader::GetFunctionTable().vkDestroyIndirectCommandsLayoutNV(loader::GetDevice(), indirectCommandsLayout, pAllocator);
}

VulkanResult vkAcquireDrmDisplayEXT(VkPhysicalDevice physicalDevice, int32_t drmFd, VkDisplayKHR display)
{
  return loader::GetFunctionTable().vkAcquireDrmDisplayEXT(physicalDevice, drmFd, display);
}

VulkanResult vkGetDrmDisplayEXT(VkPhysicalDevice physicalDevice, int32_t drmFd, uint32_t connectorId, VkDisplayKHR* display)
{
  return loader::GetFunctionTable().vkGetDrmDisplayEXT(physicalDevice, drmFd, connectorId, display);
}

VulkanResult vkCreatePrivateDataSlotEXT(const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot)
{
  return loader::GetFunctionTable().vkCreatePrivateDataSlotEXT(loader::GetDevice(), pCreateInfo, pAllocator, pPrivateDataSlot);
}

void vkDestroyPrivateDataSlotEXT(VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator)
{
  return loader::GetFunctionTable().vkDestroyPrivateDataSlotEXT(loader::GetDevice(), privateDataSlot, pAllocator);
}

VulkanResult vkSetPrivateDataEXT(VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data)
{
  return loader::GetFunctionTable().vkSetPrivateDataEXT(loader::GetDevice(), objectType, objectHandle, privateDataSlot, data);
}

void vkGetPrivateDataEXT(VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData)
{
  return loader::GetFunctionTable().vkGetPrivateDataEXT(loader::GetDevice(), objectType, objectHandle, privateDataSlot, pData);
}

VulkanResult vkAcquireWinrtDisplayNV(VkPhysicalDevice physicalDevice, VkDisplayKHR display)
{
  return loader::GetFunctionTable().vkAcquireWinrtDisplayNV(physicalDevice, display);
}

VulkanResult vkGetWinrtDisplayNV(VkPhysicalDevice physicalDevice, uint32_t deviceRelativeId, VkDisplayKHR* pDisplay)
{
  return loader::GetFunctionTable().vkGetWinrtDisplayNV(physicalDevice, deviceRelativeId, pDisplay);
}

VulkanResult vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI(VkRenderPass renderpass, VkExtent2D* pMaxWorkgroupSize)
{
  return loader::GetFunctionTable().vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI(loader::GetDevice(), renderpass, pMaxWorkgroupSize);
}

VulkanResult vkGetMemoryRemoteAddressNV(const VkMemoryGetRemoteAddressInfoNV* pMemoryGetRemoteAddressInfo, VkRemoteAddressNV* pAddress)
{
  return loader::GetFunctionTable().vkGetMemoryRemoteAddressNV(loader::GetDevice(), pMemoryGetRemoteAddressInfo, pAddress);
}

void vkSetDeviceMemoryPriorityEXT(VkDeviceMemory memory, float priority)
{
  return loader::GetFunctionTable().vkSetDeviceMemoryPriorityEXT(loader::GetDevice(), memory, priority);
}

void vkGetDescriptorSetLayoutHostMappingInfoVALVE(const VkDescriptorSetBindingReferenceVALVE* pBindingReference, VkDescriptorSetLayoutHostMappingInfoVALVE* pHostMapping)
{
  return loader::GetFunctionTable().vkGetDescriptorSetLayoutHostMappingInfoVALVE(loader::GetDevice(), pBindingReference, pHostMapping);
}

void vkGetDescriptorSetHostMappingVALVE(VkDescriptorSet descriptorSet, void** ppData)
{
  return loader::GetFunctionTable().vkGetDescriptorSetHostMappingVALVE(loader::GetDevice(), descriptorSet, ppData);
}

VulkanResult vkCreateAccelerationStructureKHR(const VkAccelerationStructureCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkAccelerationStructureKHR* pAccelerationStructure)
{
  return loader::GetFunctionTable().vkCreateAccelerationStructureKHR(loader::GetDevice(), pCreateInfo, pAllocator, pAccelerationStructure);
}

void vkDestroyAccelerationStructureKHR(VkAccelerationStructureKHR accelerationStructure, const VkAllocationCallbacks* pAllocator)
{
  return loader::GetFunctionTable().vkDestroyAccelerationStructureKHR(loader::GetDevice(), accelerationStructure, pAllocator);
}

VulkanResult vkBuildAccelerationStructuresKHR(VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos)
{
  return loader::GetFunctionTable().vkBuildAccelerationStructuresKHR(loader::GetDevice(), deferredOperation, infoCount, pInfos, ppBuildRangeInfos);
}

VulkanResult vkCopyAccelerationStructureKHR(VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureInfoKHR* pInfo)
{
  return loader::GetFunctionTable().vkCopyAccelerationStructureKHR(loader::GetDevice(), deferredOperation, pInfo);
}

VulkanResult vkCopyAccelerationStructureToMemoryKHR(VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo)
{
  return loader::GetFunctionTable().vkCopyAccelerationStructureToMemoryKHR(loader::GetDevice(), deferredOperation, pInfo);
}

VulkanResult vkCopyMemoryToAccelerationStructureKHR(VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo)
{
  return loader::GetFunctionTable().vkCopyMemoryToAccelerationStructureKHR(loader::GetDevice(), deferredOperation, pInfo);
}

VulkanResult vkWriteAccelerationStructuresPropertiesKHR(uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, size_t dataSize, void* pData, size_t stride)
{
  return loader::GetFunctionTable().vkWriteAccelerationStructuresPropertiesKHR(loader::GetDevice(), accelerationStructureCount, pAccelerationStructures, queryType, dataSize, pData, stride);
}

VulkanResultWithValue<VkDeviceAddress> vkGetAccelerationStructureDeviceAddressKHR(const VkAccelerationStructureDeviceAddressInfoKHR* pInfo)
{
  return loader::GetFunctionTable().vkGetAccelerationStructureDeviceAddressKHR(loader::GetDevice(), pInfo);
}

void vkGetDeviceAccelerationStructureCompatibilityKHR(const VkAccelerationStructureVersionInfoKHR* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility)
{
  return loader::GetFunctionTable().vkGetDeviceAccelerationStructureCompatibilityKHR(loader::GetDevice(), pVersionInfo, pCompatibility);
}

void vkGetAccelerationStructureBuildSizesKHR(VkAccelerationStructureBuildTypeKHR buildType, const VkAccelerationStructureBuildGeometryInfoKHR* pBuildInfo, const uint32_t* pMaxPrimitiveCounts, VkAccelerationStructureBuildSizesInfoKHR* pSizeInfo)
{
  return loader::GetFunctionTable().vkGetAccelerationStructureBuildSizesKHR(loader::GetDevice(), buildType, pBuildInfo, pMaxPrimitiveCounts, pSizeInfo);
}

VulkanResult vkCreateRayTracingPipelinesKHR(VkDeferredOperationKHR deferredOperation, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
{
  return loader::GetFunctionTable().vkCreateRayTracingPipelinesKHR(loader::GetDevice(), deferredOperation, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}

VulkanResult vkGetRayTracingCaptureReplayShaderGroupHandlesKHR(VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData)
{
  return loader::GetFunctionTable().vkGetRayTracingCaptureReplayShaderGroupHandlesKHR(loader::GetDevice(), pipeline, firstGroup, groupCount, dataSize, pData);
}

VulkanResultWithValue<VkDeviceSize> vkGetRayTracingShaderGroupStackSizeKHR(VkPipeline pipeline, uint32_t group, VkShaderGroupShaderKHR groupShader)
{
  return loader::GetFunctionTable().vkGetRayTracingShaderGroupStackSizeKHR(loader::GetDevice(), pipeline, group, groupShader);
}

VulkanCommandRecorder::VulkanCommandRecorder(VkCommandBuffer command_buffer) : _command_buffer(command_buffer) {}

VulkanResult VulkanCommandRecorder::Begin(const VkCommandBufferBeginInfo* pBeginInfo) const noexcept
{
 return loader::GetFunctionTable().vkBeginCommandBuffer(_command_buffer, pBeginInfo);
}

VulkanResult VulkanCommandRecorder::End() const noexcept
{
 return loader::GetFunctionTable().vkEndCommandBuffer(_command_buffer);
}

VulkanResult VulkanCommandRecorder::Reset(VkCommandBufferResetFlags flags) const noexcept
{
 return loader::GetFunctionTable().vkResetCommandBuffer(_command_buffer, flags);
}

void VulkanCommandRecorder::BindPipeline(VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline) const noexcept
{
  loader::GetFunctionTable().vkCmdBindPipeline(_command_buffer, pipelineBindPoint, pipeline);
}

void VulkanCommandRecorder::SetViewport(uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports) const noexcept
{
  loader::GetFunctionTable().vkCmdSetViewport(_command_buffer, firstViewport, viewportCount, pViewports);
}

void VulkanCommandRecorder::SetScissor(uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors) const noexcept
{
  loader::GetFunctionTable().vkCmdSetScissor(_command_buffer, firstScissor, scissorCount, pScissors);
}

void VulkanCommandRecorder::SetLineWidth(float lineWidth) const noexcept
{
  loader::GetFunctionTable().vkCmdSetLineWidth(_command_buffer, lineWidth);
}

void VulkanCommandRecorder::SetDepthBias(float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) const noexcept
{
  loader::GetFunctionTable().vkCmdSetDepthBias(_command_buffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
}

void VulkanCommandRecorder::SetBlendConstants(const float blendConstants[4]) const noexcept
{
  loader::GetFunctionTable().vkCmdSetBlendConstants(_command_buffer, blendConstants);
}

void VulkanCommandRecorder::SetDepthBounds(float minDepthBounds, float maxDepthBounds) const noexcept
{
  loader::GetFunctionTable().vkCmdSetDepthBounds(_command_buffer, minDepthBounds, maxDepthBounds);
}

void VulkanCommandRecorder::SetStencilCompareMask(VkStencilFaceFlags faceMask, uint32_t compareMask) const noexcept
{
  loader::GetFunctionTable().vkCmdSetStencilCompareMask(_command_buffer, faceMask, compareMask);
}

void VulkanCommandRecorder::SetStencilWriteMask(VkStencilFaceFlags faceMask, uint32_t writeMask) const noexcept
{
  loader::GetFunctionTable().vkCmdSetStencilWriteMask(_command_buffer, faceMask, writeMask);
}

void VulkanCommandRecorder::SetStencilReference(VkStencilFaceFlags faceMask, uint32_t reference) const noexcept
{
  loader::GetFunctionTable().vkCmdSetStencilReference(_command_buffer, faceMask, reference);
}

void VulkanCommandRecorder::BindDescriptorSets(VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets) const noexcept
{
  loader::GetFunctionTable().vkCmdBindDescriptorSets(_command_buffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
}

void VulkanCommandRecorder::BindIndexBuffer(VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) const noexcept
{
  loader::GetFunctionTable().vkCmdBindIndexBuffer(_command_buffer, buffer, offset, indexType);
}

void VulkanCommandRecorder::BindVertexBuffers(uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets) const noexcept
{
  loader::GetFunctionTable().vkCmdBindVertexBuffers(_command_buffer, firstBinding, bindingCount, pBuffers, pOffsets);
}

void VulkanCommandRecorder::Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) const noexcept
{
  loader::GetFunctionTable().vkCmdDraw(_command_buffer, vertexCount, instanceCount, firstVertex, firstInstance);
}

void VulkanCommandRecorder::DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) const noexcept
{
  loader::GetFunctionTable().vkCmdDrawIndexed(_command_buffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

void VulkanCommandRecorder::DrawIndirect(VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) const noexcept
{
  loader::GetFunctionTable().vkCmdDrawIndirect(_command_buffer, buffer, offset, drawCount, stride);
}

void VulkanCommandRecorder::DrawIndexedIndirect(VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) const noexcept
{
  loader::GetFunctionTable().vkCmdDrawIndexedIndirect(_command_buffer, buffer, offset, drawCount, stride);
}

void VulkanCommandRecorder::Dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const noexcept
{
  loader::GetFunctionTable().vkCmdDispatch(_command_buffer, groupCountX, groupCountY, groupCountZ);
}

void VulkanCommandRecorder::DispatchIndirect(VkBuffer buffer, VkDeviceSize offset) const noexcept
{
  loader::GetFunctionTable().vkCmdDispatchIndirect(_command_buffer, buffer, offset);
}

void VulkanCommandRecorder::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions) const noexcept
{
  loader::GetFunctionTable().vkCmdCopyBuffer(_command_buffer, srcBuffer, dstBuffer, regionCount, pRegions);
}

void VulkanCommandRecorder::CopyImage(VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions) const noexcept
{
  loader::GetFunctionTable().vkCmdCopyImage(_command_buffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}

void VulkanCommandRecorder::BlitImage(VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter) const noexcept
{
  loader::GetFunctionTable().vkCmdBlitImage(_command_buffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
}

void VulkanCommandRecorder::CopyBufferToImage(VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions) const noexcept
{
  loader::GetFunctionTable().vkCmdCopyBufferToImage(_command_buffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
}

void VulkanCommandRecorder::CopyImageToBuffer(VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions) const noexcept
{
  loader::GetFunctionTable().vkCmdCopyImageToBuffer(_command_buffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
}

void VulkanCommandRecorder::UpdateBuffer(VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData) const noexcept
{
  loader::GetFunctionTable().vkCmdUpdateBuffer(_command_buffer, dstBuffer, dstOffset, dataSize, pData);
}

void VulkanCommandRecorder::FillBuffer(VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data) const noexcept
{
  loader::GetFunctionTable().vkCmdFillBuffer(_command_buffer, dstBuffer, dstOffset, size, data);
}

void VulkanCommandRecorder::ClearColorImage(VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) const noexcept
{
  loader::GetFunctionTable().vkCmdClearColorImage(_command_buffer, image, imageLayout, pColor, rangeCount, pRanges);
}

void VulkanCommandRecorder::ClearDepthStencilImage(VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) const noexcept
{
  loader::GetFunctionTable().vkCmdClearDepthStencilImage(_command_buffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
}

void VulkanCommandRecorder::ClearAttachments(uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects) const noexcept
{
  loader::GetFunctionTable().vkCmdClearAttachments(_command_buffer, attachmentCount, pAttachments, rectCount, pRects);
}

void VulkanCommandRecorder::ResolveImage(VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions) const noexcept
{
  loader::GetFunctionTable().vkCmdResolveImage(_command_buffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}

void VulkanCommandRecorder::SetEvent(VkEvent event, VkPipelineStageFlags stageMask) const noexcept
{
  loader::GetFunctionTable().vkCmdSetEvent(_command_buffer, event, stageMask);
}

void VulkanCommandRecorder::ResetEvent(VkEvent event, VkPipelineStageFlags stageMask) const noexcept
{
  loader::GetFunctionTable().vkCmdResetEvent(_command_buffer, event, stageMask);
}

void VulkanCommandRecorder::WaitEvents(uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) const noexcept
{
  loader::GetFunctionTable().vkCmdWaitEvents(_command_buffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}

void VulkanCommandRecorder::PipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) const noexcept
{
  loader::GetFunctionTable().vkCmdPipelineBarrier(_command_buffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}

void VulkanCommandRecorder::BeginQuery(VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags) const noexcept
{
  loader::GetFunctionTable().vkCmdBeginQuery(_command_buffer, queryPool, query, flags);
}

void VulkanCommandRecorder::EndQuery(VkQueryPool queryPool, uint32_t query) const noexcept
{
  loader::GetFunctionTable().vkCmdEndQuery(_command_buffer, queryPool, query);
}

void VulkanCommandRecorder::ResetQueryPool(VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) const noexcept
{
  loader::GetFunctionTable().vkCmdResetQueryPool(_command_buffer, queryPool, firstQuery, queryCount);
}

void VulkanCommandRecorder::WriteTimestamp(VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query) const noexcept
{
  loader::GetFunctionTable().vkCmdWriteTimestamp(_command_buffer, pipelineStage, queryPool, query);
}

void VulkanCommandRecorder::CopyQueryPoolResults(VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags) const noexcept
{
  loader::GetFunctionTable().vkCmdCopyQueryPoolResults(_command_buffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
}

void VulkanCommandRecorder::PushConstants(VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues) const noexcept
{
  loader::GetFunctionTable().vkCmdPushConstants(_command_buffer, layout, stageFlags, offset, size, pValues);
}

void VulkanCommandRecorder::BeginRenderPass(const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents) const noexcept
{
  loader::GetFunctionTable().vkCmdBeginRenderPass(_command_buffer, pRenderPassBegin, contents);
}

void VulkanCommandRecorder::NextSubpass(VkSubpassContents contents) const noexcept
{
  loader::GetFunctionTable().vkCmdNextSubpass(_command_buffer, contents);
}

void VulkanCommandRecorder::EndRenderPass() const noexcept
{
  loader::GetFunctionTable().vkCmdEndRenderPass(_command_buffer);
}

void VulkanCommandRecorder::ExecuteCommands(uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) const noexcept
{
  loader::GetFunctionTable().vkCmdExecuteCommands(_command_buffer, commandBufferCount, pCommandBuffers);
}

void VulkanCommandRecorder::SetDeviceMask(uint32_t deviceMask) const noexcept
{
  loader::GetFunctionTable().vkCmdSetDeviceMask(_command_buffer, deviceMask);
}

void VulkanCommandRecorder::DispatchBase(uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const noexcept
{
  loader::GetFunctionTable().vkCmdDispatchBase(_command_buffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
}

void VulkanCommandRecorder::DrawIndirectCount(VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const noexcept
{
  loader::GetFunctionTable().vkCmdDrawIndirectCount(_command_buffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void VulkanCommandRecorder::DrawIndexedIndirectCount(VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const noexcept
{
  loader::GetFunctionTable().vkCmdDrawIndexedIndirectCount(_command_buffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void VulkanCommandRecorder::BeginRenderPass2(const VkRenderPassBeginInfo* pRenderPassBegin, const VkSubpassBeginInfo* pSubpassBeginInfo) const noexcept
{
  loader::GetFunctionTable().vkCmdBeginRenderPass2(_command_buffer, pRenderPassBegin, pSubpassBeginInfo);
}

void VulkanCommandRecorder::NextSubpass2(const VkSubpassBeginInfo* pSubpassBeginInfo, const VkSubpassEndInfo* pSubpassEndInfo) const noexcept
{
  loader::GetFunctionTable().vkCmdNextSubpass2(_command_buffer, pSubpassBeginInfo, pSubpassEndInfo);
}

void VulkanCommandRecorder::EndRenderPass2(const VkSubpassEndInfo* pSubpassEndInfo) const noexcept
{
  loader::GetFunctionTable().vkCmdEndRenderPass2(_command_buffer, pSubpassEndInfo);
}

void VulkanCommandRecorder::SetEvent2(VkEvent event, const VkDependencyInfo* pDependencyInfo) const noexcept
{
  loader::GetFunctionTable().vkCmdSetEvent2(_command_buffer, event, pDependencyInfo);
}

void VulkanCommandRecorder::ResetEvent2(VkEvent event, VkPipelineStageFlags2 stageMask) const noexcept
{
  loader::GetFunctionTable().vkCmdResetEvent2(_command_buffer, event, stageMask);
}

void VulkanCommandRecorder::WaitEvents2(uint32_t eventCount, const VkEvent* pEvents, const VkDependencyInfo* pDependencyInfos) const noexcept
{
  loader::GetFunctionTable().vkCmdWaitEvents2(_command_buffer, eventCount, pEvents, pDependencyInfos);
}

void VulkanCommandRecorder::PipelineBarrier2(const VkDependencyInfo* pDependencyInfo) const noexcept
{
  loader::GetFunctionTable().vkCmdPipelineBarrier2(_command_buffer, pDependencyInfo);
}

void VulkanCommandRecorder::WriteTimestamp2(VkPipelineStageFlags2 stage, VkQueryPool queryPool, uint32_t query) const noexcept
{
  loader::GetFunctionTable().vkCmdWriteTimestamp2(_command_buffer, stage, queryPool, query);
}

void VulkanCommandRecorder::CopyBuffer2(const VkCopyBufferInfo2* pCopyBufferInfo) const noexcept
{
  loader::GetFunctionTable().vkCmdCopyBuffer2(_command_buffer, pCopyBufferInfo);
}

void VulkanCommandRecorder::CopyImage2(const VkCopyImageInfo2* pCopyImageInfo) const noexcept
{
  loader::GetFunctionTable().vkCmdCopyImage2(_command_buffer, pCopyImageInfo);
}

void VulkanCommandRecorder::CopyBufferToImage2(const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo) const noexcept
{
  loader::GetFunctionTable().vkCmdCopyBufferToImage2(_command_buffer, pCopyBufferToImageInfo);
}

void VulkanCommandRecorder::CopyImageToBuffer2(const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo) const noexcept
{
  loader::GetFunctionTable().vkCmdCopyImageToBuffer2(_command_buffer, pCopyImageToBufferInfo);
}

void VulkanCommandRecorder::BlitImage2(const VkBlitImageInfo2* pBlitImageInfo) const noexcept
{
  loader::GetFunctionTable().vkCmdBlitImage2(_command_buffer, pBlitImageInfo);
}

void VulkanCommandRecorder::ResolveImage2(const VkResolveImageInfo2* pResolveImageInfo) const noexcept
{
  loader::GetFunctionTable().vkCmdResolveImage2(_command_buffer, pResolveImageInfo);
}

void VulkanCommandRecorder::BeginRendering(const VkRenderingInfo* pRenderingInfo) const noexcept
{
  loader::GetFunctionTable().vkCmdBeginRendering(_command_buffer, pRenderingInfo);
}

void VulkanCommandRecorder::EndRendering() const noexcept
{
  loader::GetFunctionTable().vkCmdEndRendering(_command_buffer);
}

void VulkanCommandRecorder::SetCullMode(VkCullModeFlags cullMode) const noexcept
{
  loader::GetFunctionTable().vkCmdSetCullMode(_command_buffer, cullMode);
}

void VulkanCommandRecorder::SetFrontFace(VkFrontFace frontFace) const noexcept
{
  loader::GetFunctionTable().vkCmdSetFrontFace(_command_buffer, frontFace);
}

void VulkanCommandRecorder::SetPrimitiveTopology(VkPrimitiveTopology primitiveTopology) const noexcept
{
  loader::GetFunctionTable().vkCmdSetPrimitiveTopology(_command_buffer, primitiveTopology);
}

void VulkanCommandRecorder::SetViewportWithCount(uint32_t viewportCount, const VkViewport* pViewports) const noexcept
{
  loader::GetFunctionTable().vkCmdSetViewportWithCount(_command_buffer, viewportCount, pViewports);
}

void VulkanCommandRecorder::SetScissorWithCount(uint32_t scissorCount, const VkRect2D* pScissors) const noexcept
{
  loader::GetFunctionTable().vkCmdSetScissorWithCount(_command_buffer, scissorCount, pScissors);
}

void VulkanCommandRecorder::BindVertexBuffers2(uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides) const noexcept
{
  loader::GetFunctionTable().vkCmdBindVertexBuffers2(_command_buffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes, pStrides);
}

void VulkanCommandRecorder::SetDepthTestEnable(VkBool32 depthTestEnable) const noexcept
{
  loader::GetFunctionTable().vkCmdSetDepthTestEnable(_command_buffer, depthTestEnable);
}

void VulkanCommandRecorder::SetDepthWriteEnable(VkBool32 depthWriteEnable) const noexcept
{
  loader::GetFunctionTable().vkCmdSetDepthWriteEnable(_command_buffer, depthWriteEnable);
}

void VulkanCommandRecorder::SetDepthCompareOp(VkCompareOp depthCompareOp) const noexcept
{
  loader::GetFunctionTable().vkCmdSetDepthCompareOp(_command_buffer, depthCompareOp);
}

void VulkanCommandRecorder::SetDepthBoundsTestEnable(VkBool32 depthBoundsTestEnable) const noexcept
{
  loader::GetFunctionTable().vkCmdSetDepthBoundsTestEnable(_command_buffer, depthBoundsTestEnable);
}

void VulkanCommandRecorder::SetStencilTestEnable(VkBool32 stencilTestEnable) const noexcept
{
  loader::GetFunctionTable().vkCmdSetStencilTestEnable(_command_buffer, stencilTestEnable);
}

void VulkanCommandRecorder::SetStencilOp(VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp) const noexcept
{
  loader::GetFunctionTable().vkCmdSetStencilOp(_command_buffer, faceMask, failOp, passOp, depthFailOp, compareOp);
}

void VulkanCommandRecorder::SetRasterizerDiscardEnable(VkBool32 rasterizerDiscardEnable) const noexcept
{
  loader::GetFunctionTable().vkCmdSetRasterizerDiscardEnable(_command_buffer, rasterizerDiscardEnable);
}

void VulkanCommandRecorder::SetDepthBiasEnable(VkBool32 depthBiasEnable) const noexcept
{
  loader::GetFunctionTable().vkCmdSetDepthBiasEnable(_command_buffer, depthBiasEnable);
}

void VulkanCommandRecorder::SetPrimitiveRestartEnable(VkBool32 primitiveRestartEnable) const noexcept
{
  loader::GetFunctionTable().vkCmdSetPrimitiveRestartEnable(_command_buffer, primitiveRestartEnable);
}

void VulkanCommandRecorder::BeginRenderingKHR(const VkRenderingInfo* pRenderingInfo) const noexcept
{
  loader::GetFunctionTable().vkCmdBeginRenderingKHR(_command_buffer, pRenderingInfo);
}

void VulkanCommandRecorder::EndRenderingKHR() const noexcept
{
  loader::GetFunctionTable().vkCmdEndRenderingKHR(_command_buffer);
}

void VulkanCommandRecorder::SetDeviceMaskKHR(uint32_t deviceMask) const noexcept
{
  loader::GetFunctionTable().vkCmdSetDeviceMaskKHR(_command_buffer, deviceMask);
}

void VulkanCommandRecorder::DispatchBaseKHR(uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const noexcept
{
  loader::GetFunctionTable().vkCmdDispatchBaseKHR(_command_buffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
}

void VulkanCommandRecorder::PushDescriptorSetKHR(VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites) const noexcept
{
  loader::GetFunctionTable().vkCmdPushDescriptorSetKHR(_command_buffer, pipelineBindPoint, layout, set, descriptorWriteCount, pDescriptorWrites);
}

void VulkanCommandRecorder::PushDescriptorSetWithTemplateKHR(VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData) const noexcept
{
  loader::GetFunctionTable().vkCmdPushDescriptorSetWithTemplateKHR(_command_buffer, descriptorUpdateTemplate, layout, set, pData);
}

void VulkanCommandRecorder::BeginRenderPass2KHR(const VkRenderPassBeginInfo* pRenderPassBegin, const VkSubpassBeginInfo* pSubpassBeginInfo) const noexcept
{
  loader::GetFunctionTable().vkCmdBeginRenderPass2KHR(_command_buffer, pRenderPassBegin, pSubpassBeginInfo);
}

void VulkanCommandRecorder::NextSubpass2KHR(const VkSubpassBeginInfo* pSubpassBeginInfo, const VkSubpassEndInfo* pSubpassEndInfo) const noexcept
{
  loader::GetFunctionTable().vkCmdNextSubpass2KHR(_command_buffer, pSubpassBeginInfo, pSubpassEndInfo);
}

void VulkanCommandRecorder::EndRenderPass2KHR(const VkSubpassEndInfo* pSubpassEndInfo) const noexcept
{
  loader::GetFunctionTable().vkCmdEndRenderPass2KHR(_command_buffer, pSubpassEndInfo);
}

void VulkanCommandRecorder::DrawIndirectCountKHR(VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const noexcept
{
  loader::GetFunctionTable().vkCmdDrawIndirectCountKHR(_command_buffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void VulkanCommandRecorder::DrawIndexedIndirectCountKHR(VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const noexcept
{
  loader::GetFunctionTable().vkCmdDrawIndexedIndirectCountKHR(_command_buffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void VulkanCommandRecorder::SetFragmentShadingRateKHR(const VkExtent2D* pFragmentSize, const VkFragmentShadingRateCombinerOpKHR combinerOps[2]) const noexcept
{
  loader::GetFunctionTable().vkCmdSetFragmentShadingRateKHR(_command_buffer, pFragmentSize, combinerOps);
}

void VulkanCommandRecorder::SetEvent2KHR(VkEvent event, const VkDependencyInfo* pDependencyInfo) const noexcept
{
  loader::GetFunctionTable().vkCmdSetEvent2KHR(_command_buffer, event, pDependencyInfo);
}

void VulkanCommandRecorder::ResetEvent2KHR(VkEvent event, VkPipelineStageFlags2 stageMask) const noexcept
{
  loader::GetFunctionTable().vkCmdResetEvent2KHR(_command_buffer, event, stageMask);
}

void VulkanCommandRecorder::WaitEvents2KHR(uint32_t eventCount, const VkEvent* pEvents, const VkDependencyInfo* pDependencyInfos) const noexcept
{
  loader::GetFunctionTable().vkCmdWaitEvents2KHR(_command_buffer, eventCount, pEvents, pDependencyInfos);
}

void VulkanCommandRecorder::PipelineBarrier2KHR(const VkDependencyInfo* pDependencyInfo) const noexcept
{
  loader::GetFunctionTable().vkCmdPipelineBarrier2KHR(_command_buffer, pDependencyInfo);
}

void VulkanCommandRecorder::WriteTimestamp2KHR(VkPipelineStageFlags2 stage, VkQueryPool queryPool, uint32_t query) const noexcept
{
  loader::GetFunctionTable().vkCmdWriteTimestamp2KHR(_command_buffer, stage, queryPool, query);
}

void VulkanCommandRecorder::WriteBufferMarker2AMD(VkPipelineStageFlags2 stage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker) const noexcept
{
  loader::GetFunctionTable().vkCmdWriteBufferMarker2AMD(_command_buffer, stage, dstBuffer, dstOffset, marker);
}

void VulkanCommandRecorder::CopyBuffer2KHR(const VkCopyBufferInfo2* pCopyBufferInfo) const noexcept
{
  loader::GetFunctionTable().vkCmdCopyBuffer2KHR(_command_buffer, pCopyBufferInfo);
}

void VulkanCommandRecorder::CopyImage2KHR(const VkCopyImageInfo2* pCopyImageInfo) const noexcept
{
  loader::GetFunctionTable().vkCmdCopyImage2KHR(_command_buffer, pCopyImageInfo);
}

void VulkanCommandRecorder::CopyBufferToImage2KHR(const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo) const noexcept
{
  loader::GetFunctionTable().vkCmdCopyBufferToImage2KHR(_command_buffer, pCopyBufferToImageInfo);
}

void VulkanCommandRecorder::CopyImageToBuffer2KHR(const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo) const noexcept
{
  loader::GetFunctionTable().vkCmdCopyImageToBuffer2KHR(_command_buffer, pCopyImageToBufferInfo);
}

void VulkanCommandRecorder::BlitImage2KHR(const VkBlitImageInfo2* pBlitImageInfo) const noexcept
{
  loader::GetFunctionTable().vkCmdBlitImage2KHR(_command_buffer, pBlitImageInfo);
}

void VulkanCommandRecorder::ResolveImage2KHR(const VkResolveImageInfo2* pResolveImageInfo) const noexcept
{
  loader::GetFunctionTable().vkCmdResolveImage2KHR(_command_buffer, pResolveImageInfo);
}

void VulkanCommandRecorder::DebugMarkerBeginEXT(const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) const noexcept
{
  loader::GetFunctionTable().vkCmdDebugMarkerBeginEXT(_command_buffer, pMarkerInfo);
}

void VulkanCommandRecorder::DebugMarkerEndEXT() const noexcept
{
  loader::GetFunctionTable().vkCmdDebugMarkerEndEXT(_command_buffer);
}

void VulkanCommandRecorder::DebugMarkerInsertEXT(const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) const noexcept
{
  loader::GetFunctionTable().vkCmdDebugMarkerInsertEXT(_command_buffer, pMarkerInfo);
}

void VulkanCommandRecorder::BindTransformFeedbackBuffersEXT(uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes) const noexcept
{
  loader::GetFunctionTable().vkCmdBindTransformFeedbackBuffersEXT(_command_buffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes);
}

void VulkanCommandRecorder::BeginTransformFeedbackEXT(uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) const noexcept
{
  loader::GetFunctionTable().vkCmdBeginTransformFeedbackEXT(_command_buffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
}

void VulkanCommandRecorder::EndTransformFeedbackEXT(uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) const noexcept
{
  loader::GetFunctionTable().vkCmdEndTransformFeedbackEXT(_command_buffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
}

void VulkanCommandRecorder::BeginQueryIndexedEXT(VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index) const noexcept
{
  loader::GetFunctionTable().vkCmdBeginQueryIndexedEXT(_command_buffer, queryPool, query, flags, index);
}

void VulkanCommandRecorder::EndQueryIndexedEXT(VkQueryPool queryPool, uint32_t query, uint32_t index) const noexcept
{
  loader::GetFunctionTable().vkCmdEndQueryIndexedEXT(_command_buffer, queryPool, query, index);
}

void VulkanCommandRecorder::DrawIndirectByteCountEXT(uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride) const noexcept
{
  loader::GetFunctionTable().vkCmdDrawIndirectByteCountEXT(_command_buffer, instanceCount, firstInstance, counterBuffer, counterBufferOffset, counterOffset, vertexStride);
}

void VulkanCommandRecorder::CuLaunchKernelNVX(const VkCuLaunchInfoNVX* pLaunchInfo) const noexcept
{
  loader::GetFunctionTable().vkCmdCuLaunchKernelNVX(_command_buffer, pLaunchInfo);
}

void VulkanCommandRecorder::DrawIndirectCountAMD(VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const noexcept
{
  loader::GetFunctionTable().vkCmdDrawIndirectCountAMD(_command_buffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void VulkanCommandRecorder::DrawIndexedIndirectCountAMD(VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const noexcept
{
  loader::GetFunctionTable().vkCmdDrawIndexedIndirectCountAMD(_command_buffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void VulkanCommandRecorder::BeginConditionalRenderingEXT(const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin) const noexcept
{
  loader::GetFunctionTable().vkCmdBeginConditionalRenderingEXT(_command_buffer, pConditionalRenderingBegin);
}

void VulkanCommandRecorder::EndConditionalRenderingEXT() const noexcept
{
  loader::GetFunctionTable().vkCmdEndConditionalRenderingEXT(_command_buffer);
}

void VulkanCommandRecorder::SetViewportWScalingNV(uint32_t firstViewport, uint32_t viewportCount, const VkViewportWScalingNV* pViewportWScalings) const noexcept
{
  loader::GetFunctionTable().vkCmdSetViewportWScalingNV(_command_buffer, firstViewport, viewportCount, pViewportWScalings);
}

void VulkanCommandRecorder::SetDiscardRectangleEXT(uint32_t firstDiscardRectangle, uint32_t discardRectangleCount, const VkRect2D* pDiscardRectangles) const noexcept
{
  loader::GetFunctionTable().vkCmdSetDiscardRectangleEXT(_command_buffer, firstDiscardRectangle, discardRectangleCount, pDiscardRectangles);
}

void VulkanCommandRecorder::BeginDebugUtilsLabelEXT(const VkDebugUtilsLabelEXT* pLabelInfo) const noexcept
{
  loader::GetFunctionTable().vkCmdBeginDebugUtilsLabelEXT(_command_buffer, pLabelInfo);
}

void VulkanCommandRecorder::EndDebugUtilsLabelEXT() const noexcept
{
  loader::GetFunctionTable().vkCmdEndDebugUtilsLabelEXT(_command_buffer);
}

void VulkanCommandRecorder::InsertDebugUtilsLabelEXT(const VkDebugUtilsLabelEXT* pLabelInfo) const noexcept
{
  loader::GetFunctionTable().vkCmdInsertDebugUtilsLabelEXT(_command_buffer, pLabelInfo);
}

void VulkanCommandRecorder::SetSampleLocationsEXT(const VkSampleLocationsInfoEXT* pSampleLocationsInfo) const noexcept
{
  loader::GetFunctionTable().vkCmdSetSampleLocationsEXT(_command_buffer, pSampleLocationsInfo);
}

void VulkanCommandRecorder::BindShadingRateImageNV(VkImageView imageView, VkImageLayout imageLayout) const noexcept
{
  loader::GetFunctionTable().vkCmdBindShadingRateImageNV(_command_buffer, imageView, imageLayout);
}

void VulkanCommandRecorder::SetViewportShadingRatePaletteNV(uint32_t firstViewport, uint32_t viewportCount, const VkShadingRatePaletteNV* pShadingRatePalettes) const noexcept
{
  loader::GetFunctionTable().vkCmdSetViewportShadingRatePaletteNV(_command_buffer, firstViewport, viewportCount, pShadingRatePalettes);
}

void VulkanCommandRecorder::SetCoarseSampleOrderNV(VkCoarseSampleOrderTypeNV sampleOrderType, uint32_t customSampleOrderCount, const VkCoarseSampleOrderCustomNV* pCustomSampleOrders) const noexcept
{
  loader::GetFunctionTable().vkCmdSetCoarseSampleOrderNV(_command_buffer, sampleOrderType, customSampleOrderCount, pCustomSampleOrders);
}

void VulkanCommandRecorder::BuildAccelerationStructureNV(const VkAccelerationStructureInfoNV* pInfo, VkBuffer instanceData, VkDeviceSize instanceOffset, VkBool32 update, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkBuffer scratch, VkDeviceSize scratchOffset) const noexcept
{
  loader::GetFunctionTable().vkCmdBuildAccelerationStructureNV(_command_buffer, pInfo, instanceData, instanceOffset, update, dst, src, scratch, scratchOffset);
}

void VulkanCommandRecorder::CopyAccelerationStructureNV(VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkCopyAccelerationStructureModeKHR mode) const noexcept
{
  loader::GetFunctionTable().vkCmdCopyAccelerationStructureNV(_command_buffer, dst, src, mode);
}

void VulkanCommandRecorder::TraceRaysNV(VkBuffer raygenShaderBindingTableBuffer, VkDeviceSize raygenShaderBindingOffset, VkBuffer missShaderBindingTableBuffer, VkDeviceSize missShaderBindingOffset, VkDeviceSize missShaderBindingStride, VkBuffer hitShaderBindingTableBuffer, VkDeviceSize hitShaderBindingOffset, VkDeviceSize hitShaderBindingStride, VkBuffer callableShaderBindingTableBuffer, VkDeviceSize callableShaderBindingOffset, VkDeviceSize callableShaderBindingStride, uint32_t width, uint32_t height, uint32_t depth) const noexcept
{
  loader::GetFunctionTable().vkCmdTraceRaysNV(_command_buffer, raygenShaderBindingTableBuffer, raygenShaderBindingOffset, missShaderBindingTableBuffer, missShaderBindingOffset, missShaderBindingStride, hitShaderBindingTableBuffer, hitShaderBindingOffset, hitShaderBindingStride, callableShaderBindingTableBuffer, callableShaderBindingOffset, callableShaderBindingStride, width, height, depth);
}

void VulkanCommandRecorder::WriteAccelerationStructuresPropertiesNV(uint32_t accelerationStructureCount, const VkAccelerationStructureNV* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) const noexcept
{
  loader::GetFunctionTable().vkCmdWriteAccelerationStructuresPropertiesNV(_command_buffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
}

void VulkanCommandRecorder::WriteBufferMarkerAMD(VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker) const noexcept
{
  loader::GetFunctionTable().vkCmdWriteBufferMarkerAMD(_command_buffer, pipelineStage, dstBuffer, dstOffset, marker);
}

void VulkanCommandRecorder::DrawMeshTasksNV(uint32_t taskCount, uint32_t firstTask) const noexcept
{
  loader::GetFunctionTable().vkCmdDrawMeshTasksNV(_command_buffer, taskCount, firstTask);
}

void VulkanCommandRecorder::DrawMeshTasksIndirectNV(VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) const noexcept
{
  loader::GetFunctionTable().vkCmdDrawMeshTasksIndirectNV(_command_buffer, buffer, offset, drawCount, stride);
}

void VulkanCommandRecorder::DrawMeshTasksIndirectCountNV(VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const noexcept
{
  loader::GetFunctionTable().vkCmdDrawMeshTasksIndirectCountNV(_command_buffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void VulkanCommandRecorder::SetExclusiveScissorNV(uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkRect2D* pExclusiveScissors) const noexcept
{
  loader::GetFunctionTable().vkCmdSetExclusiveScissorNV(_command_buffer, firstExclusiveScissor, exclusiveScissorCount, pExclusiveScissors);
}

void VulkanCommandRecorder::SetCheckpointNV(const void* pCheckpointMarker) const noexcept
{
  loader::GetFunctionTable().vkCmdSetCheckpointNV(_command_buffer, pCheckpointMarker);
}

VulkanResult VulkanCommandRecorder::SetPerformanceMarkerINTEL(const VkPerformanceMarkerInfoINTEL* pMarkerInfo) const noexcept
{
 return loader::GetFunctionTable().vkCmdSetPerformanceMarkerINTEL(_command_buffer, pMarkerInfo);
}

VulkanResult VulkanCommandRecorder::SetPerformanceStreamMarkerINTEL(const VkPerformanceStreamMarkerInfoINTEL* pMarkerInfo) const noexcept
{
 return loader::GetFunctionTable().vkCmdSetPerformanceStreamMarkerINTEL(_command_buffer, pMarkerInfo);
}

VulkanResult VulkanCommandRecorder::SetPerformanceOverrideINTEL(const VkPerformanceOverrideInfoINTEL* pOverrideInfo) const noexcept
{
 return loader::GetFunctionTable().vkCmdSetPerformanceOverrideINTEL(_command_buffer, pOverrideInfo);
}

void VulkanCommandRecorder::SetLineStippleEXT(uint32_t lineStippleFactor, uint16_t lineStipplePattern) const noexcept
{
  loader::GetFunctionTable().vkCmdSetLineStippleEXT(_command_buffer, lineStippleFactor, lineStipplePattern);
}

void VulkanCommandRecorder::SetCullModeEXT(VkCullModeFlags cullMode) const noexcept
{
  loader::GetFunctionTable().vkCmdSetCullModeEXT(_command_buffer, cullMode);
}

void VulkanCommandRecorder::SetFrontFaceEXT(VkFrontFace frontFace) const noexcept
{
  loader::GetFunctionTable().vkCmdSetFrontFaceEXT(_command_buffer, frontFace);
}

void VulkanCommandRecorder::SetPrimitiveTopologyEXT(VkPrimitiveTopology primitiveTopology) const noexcept
{
  loader::GetFunctionTable().vkCmdSetPrimitiveTopologyEXT(_command_buffer, primitiveTopology);
}

void VulkanCommandRecorder::SetViewportWithCountEXT(uint32_t viewportCount, const VkViewport* pViewports) const noexcept
{
  loader::GetFunctionTable().vkCmdSetViewportWithCountEXT(_command_buffer, viewportCount, pViewports);
}

void VulkanCommandRecorder::SetScissorWithCountEXT(uint32_t scissorCount, const VkRect2D* pScissors) const noexcept
{
  loader::GetFunctionTable().vkCmdSetScissorWithCountEXT(_command_buffer, scissorCount, pScissors);
}

void VulkanCommandRecorder::BindVertexBuffers2EXT(uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides) const noexcept
{
  loader::GetFunctionTable().vkCmdBindVertexBuffers2EXT(_command_buffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes, pStrides);
}

void VulkanCommandRecorder::SetDepthTestEnableEXT(VkBool32 depthTestEnable) const noexcept
{
  loader::GetFunctionTable().vkCmdSetDepthTestEnableEXT(_command_buffer, depthTestEnable);
}

void VulkanCommandRecorder::SetDepthWriteEnableEXT(VkBool32 depthWriteEnable) const noexcept
{
  loader::GetFunctionTable().vkCmdSetDepthWriteEnableEXT(_command_buffer, depthWriteEnable);
}

void VulkanCommandRecorder::SetDepthCompareOpEXT(VkCompareOp depthCompareOp) const noexcept
{
  loader::GetFunctionTable().vkCmdSetDepthCompareOpEXT(_command_buffer, depthCompareOp);
}

void VulkanCommandRecorder::SetDepthBoundsTestEnableEXT(VkBool32 depthBoundsTestEnable) const noexcept
{
  loader::GetFunctionTable().vkCmdSetDepthBoundsTestEnableEXT(_command_buffer, depthBoundsTestEnable);
}

void VulkanCommandRecorder::SetStencilTestEnableEXT(VkBool32 stencilTestEnable) const noexcept
{
  loader::GetFunctionTable().vkCmdSetStencilTestEnableEXT(_command_buffer, stencilTestEnable);
}

void VulkanCommandRecorder::SetStencilOpEXT(VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp) const noexcept
{
  loader::GetFunctionTable().vkCmdSetStencilOpEXT(_command_buffer, faceMask, failOp, passOp, depthFailOp, compareOp);
}

void VulkanCommandRecorder::PreprocessGeneratedCommandsNV(const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo) const noexcept
{
  loader::GetFunctionTable().vkCmdPreprocessGeneratedCommandsNV(_command_buffer, pGeneratedCommandsInfo);
}

void VulkanCommandRecorder::ExecuteGeneratedCommandsNV(VkBool32 isPreprocessed, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo) const noexcept
{
  loader::GetFunctionTable().vkCmdExecuteGeneratedCommandsNV(_command_buffer, isPreprocessed, pGeneratedCommandsInfo);
}

void VulkanCommandRecorder::BindPipelineShaderGroupNV(VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline, uint32_t groupIndex) const noexcept
{
  loader::GetFunctionTable().vkCmdBindPipelineShaderGroupNV(_command_buffer, pipelineBindPoint, pipeline, groupIndex);
}

void VulkanCommandRecorder::SetFragmentShadingRateEnumNV(VkFragmentShadingRateNV shadingRate, const VkFragmentShadingRateCombinerOpKHR combinerOps[2]) const noexcept
{
  loader::GetFunctionTable().vkCmdSetFragmentShadingRateEnumNV(_command_buffer, shadingRate, combinerOps);
}

void VulkanCommandRecorder::SetVertexInputEXT(uint32_t vertexBindingDescriptionCount, const VkVertexInputBindingDescription2EXT* pVertexBindingDescriptions, uint32_t vertexAttributeDescriptionCount, const VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions) const noexcept
{
  loader::GetFunctionTable().vkCmdSetVertexInputEXT(_command_buffer, vertexBindingDescriptionCount, pVertexBindingDescriptions, vertexAttributeDescriptionCount, pVertexAttributeDescriptions);
}

void VulkanCommandRecorder::SubpassShadingHUAWEI() const noexcept
{
  loader::GetFunctionTable().vkCmdSubpassShadingHUAWEI(_command_buffer);
}

void VulkanCommandRecorder::BindInvocationMaskHUAWEI(VkImageView imageView, VkImageLayout imageLayout) const noexcept
{
  loader::GetFunctionTable().vkCmdBindInvocationMaskHUAWEI(_command_buffer, imageView, imageLayout);
}

void VulkanCommandRecorder::SetPatchControlPointsEXT(uint32_t patchControlPoints) const noexcept
{
  loader::GetFunctionTable().vkCmdSetPatchControlPointsEXT(_command_buffer, patchControlPoints);
}

void VulkanCommandRecorder::SetRasterizerDiscardEnableEXT(VkBool32 rasterizerDiscardEnable) const noexcept
{
  loader::GetFunctionTable().vkCmdSetRasterizerDiscardEnableEXT(_command_buffer, rasterizerDiscardEnable);
}

void VulkanCommandRecorder::SetDepthBiasEnableEXT(VkBool32 depthBiasEnable) const noexcept
{
  loader::GetFunctionTable().vkCmdSetDepthBiasEnableEXT(_command_buffer, depthBiasEnable);
}

void VulkanCommandRecorder::SetLogicOpEXT(VkLogicOp logicOp) const noexcept
{
  loader::GetFunctionTable().vkCmdSetLogicOpEXT(_command_buffer, logicOp);
}

void VulkanCommandRecorder::SetPrimitiveRestartEnableEXT(VkBool32 primitiveRestartEnable) const noexcept
{
  loader::GetFunctionTable().vkCmdSetPrimitiveRestartEnableEXT(_command_buffer, primitiveRestartEnable);
}

void VulkanCommandRecorder::SetColorWriteEnableEXT(uint32_t attachmentCount, const VkBool32* pColorWriteEnables) const noexcept
{
  loader::GetFunctionTable().vkCmdSetColorWriteEnableEXT(_command_buffer, attachmentCount, pColorWriteEnables);
}

void VulkanCommandRecorder::DrawMultiEXT(uint32_t drawCount, const VkMultiDrawInfoEXT* pVertexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride) const noexcept
{
  loader::GetFunctionTable().vkCmdDrawMultiEXT(_command_buffer, drawCount, pVertexInfo, instanceCount, firstInstance, stride);
}

void VulkanCommandRecorder::DrawMultiIndexedEXT(uint32_t drawCount, const VkMultiDrawIndexedInfoEXT* pIndexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride, const int32_t* pVertexOffset) const noexcept
{
  loader::GetFunctionTable().vkCmdDrawMultiIndexedEXT(_command_buffer, drawCount, pIndexInfo, instanceCount, firstInstance, stride, pVertexOffset);
}

void VulkanCommandRecorder::BuildAccelerationStructuresKHR(uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos) const noexcept
{
  loader::GetFunctionTable().vkCmdBuildAccelerationStructuresKHR(_command_buffer, infoCount, pInfos, ppBuildRangeInfos);
}

void VulkanCommandRecorder::BuildAccelerationStructuresIndirectKHR(uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkDeviceAddress* pIndirectDeviceAddresses, const uint32_t* pIndirectStrides, const uint32_t* const* ppMaxPrimitiveCounts) const noexcept
{
  loader::GetFunctionTable().vkCmdBuildAccelerationStructuresIndirectKHR(_command_buffer, infoCount, pInfos, pIndirectDeviceAddresses, pIndirectStrides, ppMaxPrimitiveCounts);
}

void VulkanCommandRecorder::CopyAccelerationStructureKHR(const VkCopyAccelerationStructureInfoKHR* pInfo) const noexcept
{
  loader::GetFunctionTable().vkCmdCopyAccelerationStructureKHR(_command_buffer, pInfo);
}

void VulkanCommandRecorder::CopyAccelerationStructureToMemoryKHR(const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo) const noexcept
{
  loader::GetFunctionTable().vkCmdCopyAccelerationStructureToMemoryKHR(_command_buffer, pInfo);
}

void VulkanCommandRecorder::CopyMemoryToAccelerationStructureKHR(const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo) const noexcept
{
  loader::GetFunctionTable().vkCmdCopyMemoryToAccelerationStructureKHR(_command_buffer, pInfo);
}

void VulkanCommandRecorder::WriteAccelerationStructuresPropertiesKHR(uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) const noexcept
{
  loader::GetFunctionTable().vkCmdWriteAccelerationStructuresPropertiesKHR(_command_buffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
}

void VulkanCommandRecorder::TraceRaysKHR(const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, uint32_t width, uint32_t height, uint32_t depth) const noexcept
{
  loader::GetFunctionTable().vkCmdTraceRaysKHR(_command_buffer, pRaygenShaderBindingTable, pMissShaderBindingTable, pHitShaderBindingTable, pCallableShaderBindingTable, width, height, depth);
}

void VulkanCommandRecorder::TraceRaysIndirectKHR(const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, VkDeviceAddress indirectDeviceAddress) const noexcept
{
  loader::GetFunctionTable().vkCmdTraceRaysIndirectKHR(_command_buffer, pRaygenShaderBindingTable, pMissShaderBindingTable, pHitShaderBindingTable, pCallableShaderBindingTable, indirectDeviceAddress);
}

void VulkanCommandRecorder::SetRayTracingPipelineStackSizeKHR(uint32_t pipelineStackSize) const noexcept
{
  loader::GetFunctionTable().vkCmdSetRayTracingPipelineStackSizeKHR(_command_buffer, pipelineStackSize);
}

// clang-format on

} // namespace plex::graphics::vkapi
