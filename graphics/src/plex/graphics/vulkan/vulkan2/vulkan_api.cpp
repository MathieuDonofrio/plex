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

VkResult vkCreateInstance(const VkInstanceCreateInfo* pCreateInfo,
                          const VkAllocationCallbacks* pAllocator,
                          VkInstance* pInstance)
{
    return ::vkCreateInstance(pCreateInfo, pAllocator, pInstance);
}

void vkDestroyInstance(VkInstance instance, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyInstance(instance, pAllocator);
}

VkResult vkEnumeratePhysicalDevices(VkInstance instance,
                                    uint32_t* pPhysicalDeviceCount,
                                    VkPhysicalDevice* pPhysicalDevices)
{
    return ::vkEnumeratePhysicalDevices(instance, pPhysicalDeviceCount, pPhysicalDevices);
}

void vkGetPhysicalDeviceFeatures(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures)
{
    return ::vkGetPhysicalDeviceFeatures(physicalDevice, pFeatures);
}

void vkGetPhysicalDeviceFormatProperties(VkPhysicalDevice physicalDevice,
                                         VkFormat format,
                                         VkFormatProperties* pFormatProperties)
{
    return ::vkGetPhysicalDeviceFormatProperties(physicalDevice, format, pFormatProperties);
}

VkResult vkGetPhysicalDeviceImageFormatProperties(VkPhysicalDevice physicalDevice,
                                                  VkFormat format,
                                                  VkImageType type,
                                                  VkImageTiling tiling,
                                                  VkImageUsageFlags usage,
                                                  VkImageCreateFlags flags,
                                                  VkImageFormatProperties* pImageFormatProperties)
{
    return ::vkGetPhysicalDeviceImageFormatProperties(physicalDevice,
                                                      format,
                                                      type,
                                                      tiling,
                                                      usage,
                                                      flags,
                                                      pImageFormatProperties);
}

void vkGetPhysicalDeviceProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties)
{
    return ::vkGetPhysicalDeviceProperties(physicalDevice, pProperties);
}

void vkGetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice physicalDevice,
                                              uint32_t* pQueueFamilyPropertyCount,
                                              VkQueueFamilyProperties* pQueueFamilyProperties)
{
    return ::vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice,
                                                      pQueueFamilyPropertyCount,
                                                      pQueueFamilyProperties);
}

void vkGetPhysicalDeviceMemoryProperties(VkPhysicalDevice physicalDevice,
                                         VkPhysicalDeviceMemoryProperties* pMemoryProperties)
{
    return ::vkGetPhysicalDeviceMemoryProperties(physicalDevice, pMemoryProperties);
}

PFN_vkVoidFunction vkGetInstanceProcAddr(VkInstance instance, const char* pName)
{
    return ::vkGetInstanceProcAddr(instance, pName);
}

PFN_vkVoidFunction vkGetDeviceProcAddr(const char* pName)
{
    return ::vkGetDeviceProcAddr(device_, pName);
}

VkResult vkCreateDevice(VkPhysicalDevice physicalDevice,
                        const VkDeviceCreateInfo* pCreateInfo,
                        const VkAllocationCallbacks* pAllocator,
                        VkDevice* pDevice)
{
    return ::vkCreateDevice(physicalDevice, pCreateInfo, pAllocator, pDevice);
}

void vkDestroyDevice(const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyDevice(device_, pAllocator);
}

VkResult vkEnumerateInstanceExtensionProperties(const char* pLayerName,
                                                uint32_t* pPropertyCount,
                                                VkExtensionProperties* pProperties)
{
    return ::vkEnumerateInstanceExtensionProperties(pLayerName, pPropertyCount, pProperties);
}

VkResult vkEnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice,
                                              const char* pLayerName,
                                              uint32_t* pPropertyCount,
                                              VkExtensionProperties* pProperties)
{
    return ::vkEnumerateDeviceExtensionProperties(physicalDevice, pLayerName, pPropertyCount, pProperties);
}

VkResult vkEnumerateInstanceLayerProperties(uint32_t* pPropertyCount, VkLayerProperties* pProperties)
{
    return ::vkEnumerateInstanceLayerProperties(pPropertyCount, pProperties);
}

VkResult vkEnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice,
                                          uint32_t* pPropertyCount,
                                          VkLayerProperties* pProperties)
{
    return ::vkEnumerateDeviceLayerProperties(physicalDevice, pPropertyCount, pProperties);
}

void vkGetDeviceQueue(uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue)
{
    return ::vkGetDeviceQueue(device_, queueFamilyIndex, queueIndex, pQueue);
}

VkResult vkQueueSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence)
{
    return ::vkQueueSubmit(queue, submitCount, pSubmits, fence);
}

VkResult vkQueueWaitIdle(VkQueue queue)
{
    return ::vkQueueWaitIdle(queue);
}

VkResult vkDeviceWaitIdle()
{
    return ::vkDeviceWaitIdle(device_);
}

VkResult vkAllocateMemory(const VkMemoryAllocateInfo* pAllocateInfo,
                          const VkAllocationCallbacks* pAllocator,
                          VkDeviceMemory* pMemory)
{
    return ::vkAllocateMemory(device_, pAllocateInfo, pAllocator, pMemory);
}

void vkFreeMemory(VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator)
{
    return ::vkFreeMemory(device_, memory, pAllocator);
}

VkResult vkMapMemory(VkDeviceMemory memory,
                     VkDeviceSize offset,
                     VkDeviceSize size,
                     VkMemoryMapFlags flags,
                     void** ppData)
{
    return ::vkMapMemory(device_, memory, offset, size, flags, ppData);
}

void vkUnmapMemory(VkDeviceMemory memory)
{
    return ::vkUnmapMemory(device_, memory);
}

VkResult vkFlushMappedMemoryRanges(uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges)
{
    return ::vkFlushMappedMemoryRanges(device_, memoryRangeCount, pMemoryRanges);
}

VkResult vkInvalidateMappedMemoryRanges(uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges)
{
    return ::vkInvalidateMappedMemoryRanges(device_, memoryRangeCount, pMemoryRanges);
}

void vkGetDeviceMemoryCommitment(VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes)
{
    return ::vkGetDeviceMemoryCommitment(device_, memory, pCommittedMemoryInBytes);
}

VkResult vkBindBufferMemory(VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset)
{
    return ::vkBindBufferMemory(device_, buffer, memory, memoryOffset);
}

VkResult vkBindImageMemory(VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset)
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

void vkGetImageSparseMemoryRequirements(VkImage image,
                                        uint32_t* pSparseMemoryRequirementCount,
                                        VkSparseImageMemoryRequirements* pSparseMemoryRequirements)
{
    return ::vkGetImageSparseMemoryRequirements(device_,
                                                image,
                                                pSparseMemoryRequirementCount,
                                                pSparseMemoryRequirements);
}

void vkGetPhysicalDeviceSparseImageFormatProperties(VkPhysicalDevice physicalDevice,
                                                    VkFormat format,
                                                    VkImageType type,
                                                    VkSampleCountFlagBits samples,
                                                    VkImageUsageFlags usage,
                                                    VkImageTiling tiling,
                                                    uint32_t* pPropertyCount,
                                                    VkSparseImageFormatProperties* pProperties)
{
    return ::vkGetPhysicalDeviceSparseImageFormatProperties(physicalDevice,
                                                            format,
                                                            type,
                                                            samples,
                                                            usage,
                                                            tiling,
                                                            pPropertyCount,
                                                            pProperties);
}

VkResult vkQueueBindSparse(VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence)
{
    return ::vkQueueBindSparse(queue, bindInfoCount, pBindInfo, fence);
}

VkResult vkCreateFence(const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence)
{
    return ::vkCreateFence(device_, pCreateInfo, pAllocator, pFence);
}

void vkDestroyFence(VkFence fence, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyFence(device_, fence, pAllocator);
}

VkResult vkResetFences(uint32_t fenceCount, const VkFence* pFences)
{
    return ::vkResetFences(device_, fenceCount, pFences);
}

VkResult vkGetFenceStatus(VkFence fence)
{
    return ::vkGetFenceStatus(device_, fence);
}

VkResult vkWaitForFences(uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout)
{
    return ::vkWaitForFences(device_, fenceCount, pFences, waitAll, timeout);
}

VkResult vkCreateSemaphore(const VkSemaphoreCreateInfo* pCreateInfo,
                           const VkAllocationCallbacks* pAllocator,
                           VkSemaphore* pSemaphore)
{
    return ::vkCreateSemaphore(device_, pCreateInfo, pAllocator, pSemaphore);
}

void vkDestroySemaphore(VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroySemaphore(device_, semaphore, pAllocator);
}

VkResult vkCreateEvent(const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent)
{
    return ::vkCreateEvent(device_, pCreateInfo, pAllocator, pEvent);
}

void vkDestroyEvent(VkEvent event, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyEvent(device_, event, pAllocator);
}

VkResult vkGetEventStatus(VkEvent event)
{
    return ::vkGetEventStatus(device_, event);
}

VkResult vkSetEvent(VkEvent event)
{
    return ::vkSetEvent(device_, event);
}

VkResult vkResetEvent(VkEvent event)
{
    return ::vkResetEvent(device_, event);
}

VkResult vkCreateQueryPool(const VkQueryPoolCreateInfo* pCreateInfo,
                           const VkAllocationCallbacks* pAllocator,
                           VkQueryPool* pQueryPool)
{
    return ::vkCreateQueryPool(device_, pCreateInfo, pAllocator, pQueryPool);
}

void vkDestroyQueryPool(VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyQueryPool(device_, queryPool, pAllocator);
}

VkResult vkGetQueryPoolResults(VkQueryPool queryPool,
                               uint32_t firstQuery,
                               uint32_t queryCount,
                               size_t dataSize,
                               void* pData,
                               VkDeviceSize stride,
                               VkQueryResultFlags flags)
{
    return ::vkGetQueryPoolResults(device_, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags);
}

VkResult vkCreateBuffer(const VkBufferCreateInfo* pCreateInfo,
                        const VkAllocationCallbacks* pAllocator,
                        VkBuffer* pBuffer)
{
    return ::vkCreateBuffer(device_, pCreateInfo, pAllocator, pBuffer);
}

void vkDestroyBuffer(VkBuffer buffer, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyBuffer(device_, buffer, pAllocator);
}

VkResult vkCreateBufferView(const VkBufferViewCreateInfo* pCreateInfo,
                            const VkAllocationCallbacks* pAllocator,
                            VkBufferView* pView)
{
    return ::vkCreateBufferView(device_, pCreateInfo, pAllocator, pView);
}

void vkDestroyBufferView(VkBufferView bufferView, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyBufferView(device_, bufferView, pAllocator);
}

VkResult vkCreateImage(const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage)
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

VkResult vkCreateImageView(const VkImageViewCreateInfo* pCreateInfo,
                           const VkAllocationCallbacks* pAllocator,
                           VkImageView* pView)
{
    return ::vkCreateImageView(device_, pCreateInfo, pAllocator, pView);
}

void vkDestroyImageView(VkImageView imageView, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyImageView(device_, imageView, pAllocator);
}

VkResult vkCreateShaderModule(const VkShaderModuleCreateInfo* pCreateInfo,
                              const VkAllocationCallbacks* pAllocator,
                              VkShaderModule* pShaderModule)
{
    return ::vkCreateShaderModule(device_, pCreateInfo, pAllocator, pShaderModule);
}

void vkDestroyShaderModule(VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyShaderModule(device_, shaderModule, pAllocator);
}

VkResult vkCreatePipelineCache(const VkPipelineCacheCreateInfo* pCreateInfo,
                               const VkAllocationCallbacks* pAllocator,
                               VkPipelineCache* pPipelineCache)
{
    return ::vkCreatePipelineCache(device_, pCreateInfo, pAllocator, pPipelineCache);
}

void vkDestroyPipelineCache(VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyPipelineCache(device_, pipelineCache, pAllocator);
}

VkResult vkGetPipelineCacheData(VkPipelineCache pipelineCache, size_t* pDataSize, void* pData)
{
    return ::vkGetPipelineCacheData(device_, pipelineCache, pDataSize, pData);
}

VkResult vkMergePipelineCaches(VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches)
{
    return ::vkMergePipelineCaches(device_, dstCache, srcCacheCount, pSrcCaches);
}

VkResult vkCreateGraphicsPipelines(VkPipelineCache pipelineCache,
                                   uint32_t createInfoCount,
                                   const VkGraphicsPipelineCreateInfo* pCreateInfos,
                                   const VkAllocationCallbacks* pAllocator,
                                   VkPipeline* pPipelines)
{
    return ::vkCreateGraphicsPipelines(device_,
                                       pipelineCache,
                                       createInfoCount,
                                       pCreateInfos,
                                       pAllocator,
                                       pPipelines);
}

VkResult vkCreateComputePipelines(VkPipelineCache pipelineCache,
                                  uint32_t createInfoCount,
                                  const VkComputePipelineCreateInfo* pCreateInfos,
                                  const VkAllocationCallbacks* pAllocator,
                                  VkPipeline* pPipelines)
{
    return ::vkCreateComputePipelines(device_, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}

void vkDestroyPipeline(VkPipeline pipeline, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyPipeline(device_, pipeline, pAllocator);
}

VkResult vkCreatePipelineLayout(const VkPipelineLayoutCreateInfo* pCreateInfo,
                                const VkAllocationCallbacks* pAllocator,
                                VkPipelineLayout* pPipelineLayout)
{
    return ::vkCreatePipelineLayout(device_, pCreateInfo, pAllocator, pPipelineLayout);
}

void vkDestroyPipelineLayout(VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyPipelineLayout(device_, pipelineLayout, pAllocator);
}

VkResult vkCreateSampler(const VkSamplerCreateInfo* pCreateInfo,
                         const VkAllocationCallbacks* pAllocator,
                         VkSampler* pSampler)
{
    return ::vkCreateSampler(device_, pCreateInfo, pAllocator, pSampler);
}

void vkDestroySampler(VkSampler sampler, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroySampler(device_, sampler, pAllocator);
}

VkResult vkCreateDescriptorSetLayout(const VkDescriptorSetLayoutCreateInfo* pCreateInfo,
                                     const VkAllocationCallbacks* pAllocator,
                                     VkDescriptorSetLayout* pSetLayout)
{
    return ::vkCreateDescriptorSetLayout(device_, pCreateInfo, pAllocator, pSetLayout);
}

void vkDestroyDescriptorSetLayout(VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyDescriptorSetLayout(device_, descriptorSetLayout, pAllocator);
}

VkResult vkCreateDescriptorPool(const VkDescriptorPoolCreateInfo* pCreateInfo,
                                const VkAllocationCallbacks* pAllocator,
                                VkDescriptorPool* pDescriptorPool)
{
    return ::vkCreateDescriptorPool(device_, pCreateInfo, pAllocator, pDescriptorPool);
}

void vkDestroyDescriptorPool(VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyDescriptorPool(device_, descriptorPool, pAllocator);
}

VkResult vkResetDescriptorPool(VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags)
{
    return ::vkResetDescriptorPool(device_, descriptorPool, flags);
}

VkResult vkAllocateDescriptorSets(const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets)
{
    return ::vkAllocateDescriptorSets(device_, pAllocateInfo, pDescriptorSets);
}

VkResult vkFreeDescriptorSets(VkDescriptorPool descriptorPool,
                              uint32_t descriptorSetCount,
                              const VkDescriptorSet* pDescriptorSets)
{
    return ::vkFreeDescriptorSets(device_, descriptorPool, descriptorSetCount, pDescriptorSets);
}

void vkUpdateDescriptorSets(uint32_t descriptorWriteCount,
                            const VkWriteDescriptorSet* pDescriptorWrites,
                            uint32_t descriptorCopyCount,
                            const VkCopyDescriptorSet* pDescriptorCopies)
{
    return ::vkUpdateDescriptorSets(device_,
                                    descriptorWriteCount,
                                    pDescriptorWrites,
                                    descriptorCopyCount,
                                    pDescriptorCopies);
}

VkResult vkCreateFramebuffer(const VkFramebufferCreateInfo* pCreateInfo,
                             const VkAllocationCallbacks* pAllocator,
                             VkFramebuffer* pFramebuffer)
{
    return ::vkCreateFramebuffer(device_, pCreateInfo, pAllocator, pFramebuffer);
}

void vkDestroyFramebuffer(VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyFramebuffer(device_, framebuffer, pAllocator);
}

VkResult vkCreateRenderPass(const VkRenderPassCreateInfo* pCreateInfo,
                            const VkAllocationCallbacks* pAllocator,
                            VkRenderPass* pRenderPass)
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

VkResult vkCreateCommandPool(const VkCommandPoolCreateInfo* pCreateInfo,
                             const VkAllocationCallbacks* pAllocator,
                             VkCommandPool* pCommandPool)
{
    return ::vkCreateCommandPool(device_, pCreateInfo, pAllocator, pCommandPool);
}

void vkDestroyCommandPool(VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyCommandPool(device_, commandPool, pAllocator);
}

VkResult vkResetCommandPool(VkCommandPool commandPool, VkCommandPoolResetFlags flags)
{
    return ::vkResetCommandPool(device_, commandPool, flags);
}

VkResult vkAllocateCommandBuffers(const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers)
{
    return ::vkAllocateCommandBuffers(device_, pAllocateInfo, pCommandBuffers);
}

void vkFreeCommandBuffers(VkCommandPool commandPool,
                          uint32_t commandBufferCount,
                          const VkCommandBuffer* pCommandBuffers)
{
    return ::vkFreeCommandBuffers(device_, commandPool, commandBufferCount, pCommandBuffers);
}

VkResult vkBeginCommandBuffer(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo)
{
    return ::vkBeginCommandBuffer(commandBuffer, pBeginInfo);
}

VkResult vkEndCommandBuffer(VkCommandBuffer commandBuffer)
{
    return ::vkEndCommandBuffer(commandBuffer);
}

VkResult vkResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags)
{
    return ::vkResetCommandBuffer(commandBuffer, flags);
}

void vkCmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline)
{
    return ::vkCmdBindPipeline(commandBuffer, pipelineBindPoint, pipeline);
}

void vkCmdSetViewport(VkCommandBuffer commandBuffer,
                      uint32_t firstViewport,
                      uint32_t viewportCount,
                      const VkViewport* pViewports)
{
    return ::vkCmdSetViewport(commandBuffer, firstViewport, viewportCount, pViewports);
}

void vkCmdSetScissor(VkCommandBuffer commandBuffer,
                     uint32_t firstScissor,
                     uint32_t scissorCount,
                     const VkRect2D* pScissors)
{
    return ::vkCmdSetScissor(commandBuffer, firstScissor, scissorCount, pScissors);
}

void vkCmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth)
{
    return ::vkCmdSetLineWidth(commandBuffer, lineWidth);
}

void vkCmdSetDepthBias(VkCommandBuffer commandBuffer,
                       float depthBiasConstantFactor,
                       float depthBiasClamp,
                       float depthBiasSlopeFactor)
{
    return ::vkCmdSetDepthBias(commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
}

void vkCmdSetBlendConstants(VkCommandBuffer commandBuffer, const float blendConstants[4])
{
    return ::vkCmdSetBlendConstants(commandBuffer, blendConstants);
}

void vkCmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds)
{
    return ::vkCmdSetDepthBounds(commandBuffer, minDepthBounds, maxDepthBounds);
}

void vkCmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask)
{
    return ::vkCmdSetStencilCompareMask(commandBuffer, faceMask, compareMask);
}

void vkCmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask)
{
    return ::vkCmdSetStencilWriteMask(commandBuffer, faceMask, writeMask);
}

void vkCmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference)
{
    return ::vkCmdSetStencilReference(commandBuffer, faceMask, reference);
}

void vkCmdBindDescriptorSets(VkCommandBuffer commandBuffer,
                             VkPipelineBindPoint pipelineBindPoint,
                             VkPipelineLayout layout,
                             uint32_t firstSet,
                             uint32_t descriptorSetCount,
                             const VkDescriptorSet* pDescriptorSets,
                             uint32_t dynamicOffsetCount,
                             const uint32_t* pDynamicOffsets)
{
    return ::vkCmdBindDescriptorSets(commandBuffer,
                                     pipelineBindPoint,
                                     layout,
                                     firstSet,
                                     descriptorSetCount,
                                     pDescriptorSets,
                                     dynamicOffsetCount,
                                     pDynamicOffsets);
}

void vkCmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType)
{
    return ::vkCmdBindIndexBuffer(commandBuffer, buffer, offset, indexType);
}

void vkCmdBindVertexBuffers(VkCommandBuffer commandBuffer,
                            uint32_t firstBinding,
                            uint32_t bindingCount,
                            const VkBuffer* pBuffers,
                            const VkDeviceSize* pOffsets)
{
    return ::vkCmdBindVertexBuffers(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
}

void vkCmdDraw(VkCommandBuffer commandBuffer,
               uint32_t vertexCount,
               uint32_t instanceCount,
               uint32_t firstVertex,
               uint32_t firstInstance)
{
    return ::vkCmdDraw(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
}

void vkCmdDrawIndexed(VkCommandBuffer commandBuffer,
                      uint32_t indexCount,
                      uint32_t instanceCount,
                      uint32_t firstIndex,
                      int32_t vertexOffset,
                      uint32_t firstInstance)
{
    return ::vkCmdDrawIndexed(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

void vkCmdDrawIndirect(VkCommandBuffer commandBuffer,
                       VkBuffer buffer,
                       VkDeviceSize offset,
                       uint32_t drawCount,
                       uint32_t stride)
{
    return ::vkCmdDrawIndirect(commandBuffer, buffer, offset, drawCount, stride);
}

void vkCmdDrawIndexedIndirect(VkCommandBuffer commandBuffer,
                              VkBuffer buffer,
                              VkDeviceSize offset,
                              uint32_t drawCount,
                              uint32_t stride)
{
    return ::vkCmdDrawIndexedIndirect(commandBuffer, buffer, offset, drawCount, stride);
}

void vkCmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
{
    return ::vkCmdDispatch(commandBuffer, groupCountX, groupCountY, groupCountZ);
}

void vkCmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset)
{
    return ::vkCmdDispatchIndirect(commandBuffer, buffer, offset);
}

void vkCmdCopyBuffer(VkCommandBuffer commandBuffer,
                     VkBuffer srcBuffer,
                     VkBuffer dstBuffer,
                     uint32_t regionCount,
                     const VkBufferCopy* pRegions)
{
    return ::vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
}

void vkCmdCopyImage(VkCommandBuffer commandBuffer,
                    VkImage srcImage,
                    VkImageLayout srcImageLayout,
                    VkImage dstImage,
                    VkImageLayout dstImageLayout,
                    uint32_t regionCount,
                    const VkImageCopy* pRegions)
{
    return ::vkCmdCopyImage(commandBuffer,
                            srcImage,
                            srcImageLayout,
                            dstImage,
                            dstImageLayout,
                            regionCount,
                            pRegions);
}

void vkCmdBlitImage(VkCommandBuffer commandBuffer,
                    VkImage srcImage,
                    VkImageLayout srcImageLayout,
                    VkImage dstImage,
                    VkImageLayout dstImageLayout,
                    uint32_t regionCount,
                    const VkImageBlit* pRegions,
                    VkFilter filter)
{
    return ::vkCmdBlitImage(commandBuffer,
                            srcImage,
                            srcImageLayout,
                            dstImage,
                            dstImageLayout,
                            regionCount,
                            pRegions,
                            filter);
}

void vkCmdCopyBufferToImage(VkCommandBuffer commandBuffer,
                            VkBuffer srcBuffer,
                            VkImage dstImage,
                            VkImageLayout dstImageLayout,
                            uint32_t regionCount,
                            const VkBufferImageCopy* pRegions)
{
    return ::vkCmdCopyBufferToImage(commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
}

void vkCmdCopyImageToBuffer(VkCommandBuffer commandBuffer,
                            VkImage srcImage,
                            VkImageLayout srcImageLayout,
                            VkBuffer dstBuffer,
                            uint32_t regionCount,
                            const VkBufferImageCopy* pRegions)
{
    return ::vkCmdCopyImageToBuffer(commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
}

void vkCmdUpdateBuffer(VkCommandBuffer commandBuffer,
                       VkBuffer dstBuffer,
                       VkDeviceSize dstOffset,
                       VkDeviceSize dataSize,
                       const void* pData)
{
    return ::vkCmdUpdateBuffer(commandBuffer, dstBuffer, dstOffset, dataSize, pData);
}

void vkCmdFillBuffer(VkCommandBuffer commandBuffer,
                     VkBuffer dstBuffer,
                     VkDeviceSize dstOffset,
                     VkDeviceSize size,
                     uint32_t data)
{
    return ::vkCmdFillBuffer(commandBuffer, dstBuffer, dstOffset, size, data);
}

void vkCmdClearColorImage(VkCommandBuffer commandBuffer,
                          VkImage image,
                          VkImageLayout imageLayout,
                          const VkClearColorValue* pColor,
                          uint32_t rangeCount,
                          const VkImageSubresourceRange* pRanges)
{
    return ::vkCmdClearColorImage(commandBuffer, image, imageLayout, pColor, rangeCount, pRanges);
}

void vkCmdClearDepthStencilImage(VkCommandBuffer commandBuffer,
                                 VkImage image,
                                 VkImageLayout imageLayout,
                                 const VkClearDepthStencilValue* pDepthStencil,
                                 uint32_t rangeCount,
                                 const VkImageSubresourceRange* pRanges)
{
    return ::vkCmdClearDepthStencilImage(commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
}

void vkCmdClearAttachments(VkCommandBuffer commandBuffer,
                           uint32_t attachmentCount,
                           const VkClearAttachment* pAttachments,
                           uint32_t rectCount,
                           const VkClearRect* pRects)
{
    return ::vkCmdClearAttachments(commandBuffer, attachmentCount, pAttachments, rectCount, pRects);
}

void vkCmdResolveImage(VkCommandBuffer commandBuffer,
                       VkImage srcImage,
                       VkImageLayout srcImageLayout,
                       VkImage dstImage,
                       VkImageLayout dstImageLayout,
                       uint32_t regionCount,
                       const VkImageResolve* pRegions)
{
    return ::vkCmdResolveImage(commandBuffer,
                               srcImage,
                               srcImageLayout,
                               dstImage,
                               dstImageLayout,
                               regionCount,
                               pRegions);
}

void vkCmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask)
{
    return ::vkCmdSetEvent(commandBuffer, event, stageMask);
}

void vkCmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask)
{
    return ::vkCmdResetEvent(commandBuffer, event, stageMask);
}

void vkCmdWaitEvents(VkCommandBuffer commandBuffer,
                     uint32_t eventCount,
                     const VkEvent* pEvents,
                     VkPipelineStageFlags srcStageMask,
                     VkPipelineStageFlags dstStageMask,
                     uint32_t memoryBarrierCount,
                     const VkMemoryBarrier* pMemoryBarriers,
                     uint32_t bufferMemoryBarrierCount,
                     const VkBufferMemoryBarrier* pBufferMemoryBarriers,
                     uint32_t imageMemoryBarrierCount,
                     const VkImageMemoryBarrier* pImageMemoryBarriers)
{
    return ::vkCmdWaitEvents(commandBuffer,
                             eventCount,
                             pEvents,
                             srcStageMask,
                             dstStageMask,
                             memoryBarrierCount,
                             pMemoryBarriers,
                             bufferMemoryBarrierCount,
                             pBufferMemoryBarriers,
                             imageMemoryBarrierCount,
                             pImageMemoryBarriers);
}

void vkCmdPipelineBarrier(VkCommandBuffer commandBuffer,
                          VkPipelineStageFlags srcStageMask,
                          VkPipelineStageFlags dstStageMask,
                          VkDependencyFlags dependencyFlags,
                          uint32_t memoryBarrierCount,
                          const VkMemoryBarrier* pMemoryBarriers,
                          uint32_t bufferMemoryBarrierCount,
                          const VkBufferMemoryBarrier* pBufferMemoryBarriers,
                          uint32_t imageMemoryBarrierCount,
                          const VkImageMemoryBarrier* pImageMemoryBarriers)
{
    return ::vkCmdPipelineBarrier(commandBuffer,
                                  srcStageMask,
                                  dstStageMask,
                                  dependencyFlags,
                                  memoryBarrierCount,
                                  pMemoryBarriers,
                                  bufferMemoryBarrierCount,
                                  pBufferMemoryBarriers,
                                  imageMemoryBarrierCount,
                                  pImageMemoryBarriers);
}

void vkCmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags)
{
    return ::vkCmdBeginQuery(commandBuffer, queryPool, query, flags);
}

void vkCmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query)
{
    return ::vkCmdEndQuery(commandBuffer, queryPool, query);
}

void vkCmdResetQueryPool(VkCommandBuffer commandBuffer,
                         VkQueryPool queryPool,
                         uint32_t firstQuery,
                         uint32_t queryCount)
{
    return ::vkCmdResetQueryPool(commandBuffer, queryPool, firstQuery, queryCount);
}

void vkCmdWriteTimestamp(VkCommandBuffer commandBuffer,
                         VkPipelineStageFlagBits pipelineStage,
                         VkQueryPool queryPool,
                         uint32_t query)
{
    return ::vkCmdWriteTimestamp(commandBuffer, pipelineStage, queryPool, query);
}

void vkCmdCopyQueryPoolResults(VkCommandBuffer commandBuffer,
                               VkQueryPool queryPool,
                               uint32_t firstQuery,
                               uint32_t queryCount,
                               VkBuffer dstBuffer,
                               VkDeviceSize dstOffset,
                               VkDeviceSize stride,
                               VkQueryResultFlags flags)
{
    return ::vkCmdCopyQueryPoolResults(commandBuffer,
                                       queryPool,
                                       firstQuery,
                                       queryCount,
                                       dstBuffer,
                                       dstOffset,
                                       stride,
                                       flags);
}

void vkCmdPushConstants(VkCommandBuffer commandBuffer,
                        VkPipelineLayout layout,
                        VkShaderStageFlags stageFlags,
                        uint32_t offset,
                        uint32_t size,
                        const void* pValues)
{
    return ::vkCmdPushConstants(commandBuffer, layout, stageFlags, offset, size, pValues);
}

void vkCmdBeginRenderPass(VkCommandBuffer commandBuffer,
                          const VkRenderPassBeginInfo* pRenderPassBegin,
                          VkSubpassContents contents)
{
    return ::vkCmdBeginRenderPass(commandBuffer, pRenderPassBegin, contents);
}

void vkCmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents)
{
    return ::vkCmdNextSubpass(commandBuffer, contents);
}

void vkCmdEndRenderPass(VkCommandBuffer commandBuffer)
{
    return ::vkCmdEndRenderPass(commandBuffer);
}

void vkCmdExecuteCommands(VkCommandBuffer commandBuffer,
                          uint32_t commandBufferCount,
                          const VkCommandBuffer* pCommandBuffers)
{
    return ::vkCmdExecuteCommands(commandBuffer, commandBufferCount, pCommandBuffers);
}

VkResult vkEnumerateInstanceVersion(uint32_t* pApiVersion)
{
    return ::vkEnumerateInstanceVersion(pApiVersion);
}

VkResult vkBindBufferMemory2(uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos)
{
    return ::vkBindBufferMemory2(device_, bindInfoCount, pBindInfos);
}

VkResult vkBindImageMemory2(uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos)
{
    return ::vkBindImageMemory2(device_, bindInfoCount, pBindInfos);
}

void vkGetDeviceGroupPeerMemoryFeatures(uint32_t heapIndex,
                                        uint32_t localDeviceIndex,
                                        uint32_t remoteDeviceIndex,
                                        VkPeerMemoryFeatureFlags* pPeerMemoryFeatures)
{
    return ::vkGetDeviceGroupPeerMemoryFeatures(device_,
                                                heapIndex,
                                                localDeviceIndex,
                                                remoteDeviceIndex,
                                                pPeerMemoryFeatures);
}

void vkCmdSetDeviceMask(VkCommandBuffer commandBuffer, uint32_t deviceMask)
{
    return ::vkCmdSetDeviceMask(commandBuffer, deviceMask);
}

void vkCmdDispatchBase(VkCommandBuffer commandBuffer,
                       uint32_t baseGroupX,
                       uint32_t baseGroupY,
                       uint32_t baseGroupZ,
                       uint32_t groupCountX,
                       uint32_t groupCountY,
                       uint32_t groupCountZ)
{
    return ::vkCmdDispatchBase(commandBuffer,
                               baseGroupX,
                               baseGroupY,
                               baseGroupZ,
                               groupCountX,
                               groupCountY,
                               groupCountZ);
}

VkResult vkEnumeratePhysicalDeviceGroups(VkInstance instance,
                                         uint32_t* pPhysicalDeviceGroupCount,
                                         VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties)
{
    return ::vkEnumeratePhysicalDeviceGroups(instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
}

void vkGetImageMemoryRequirements2(const VkImageMemoryRequirementsInfo2* pInfo,
                                   VkMemoryRequirements2* pMemoryRequirements)
{
    return ::vkGetImageMemoryRequirements2(device_, pInfo, pMemoryRequirements);
}

void vkGetBufferMemoryRequirements2(const VkBufferMemoryRequirementsInfo2* pInfo,
                                    VkMemoryRequirements2* pMemoryRequirements)
{
    return ::vkGetBufferMemoryRequirements2(device_, pInfo, pMemoryRequirements);
}

void vkGetImageSparseMemoryRequirements2(const VkImageSparseMemoryRequirementsInfo2* pInfo,
                                         uint32_t* pSparseMemoryRequirementCount,
                                         VkSparseImageMemoryRequirements2* pSparseMemoryRequirements)
{
    return ::vkGetImageSparseMemoryRequirements2(device_,
                                                 pInfo,
                                                 pSparseMemoryRequirementCount,
                                                 pSparseMemoryRequirements);
}

void vkGetPhysicalDeviceFeatures2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures)
{
    return ::vkGetPhysicalDeviceFeatures2(physicalDevice, pFeatures);
}

void vkGetPhysicalDeviceProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties)
{
    return ::vkGetPhysicalDeviceProperties2(physicalDevice, pProperties);
}

void vkGetPhysicalDeviceFormatProperties2(VkPhysicalDevice physicalDevice,
                                          VkFormat format,
                                          VkFormatProperties2* pFormatProperties)
{
    return ::vkGetPhysicalDeviceFormatProperties2(physicalDevice, format, pFormatProperties);
}

VkResult vkGetPhysicalDeviceImageFormatProperties2(VkPhysicalDevice physicalDevice,
                                                   const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo,
                                                   VkImageFormatProperties2* pImageFormatProperties)
{
    return ::vkGetPhysicalDeviceImageFormatProperties2(physicalDevice, pImageFormatInfo, pImageFormatProperties);
}

void vkGetPhysicalDeviceQueueFamilyProperties2(VkPhysicalDevice physicalDevice,
                                               uint32_t* pQueueFamilyPropertyCount,
                                               VkQueueFamilyProperties2* pQueueFamilyProperties)
{
    return ::vkGetPhysicalDeviceQueueFamilyProperties2(physicalDevice,
                                                       pQueueFamilyPropertyCount,
                                                       pQueueFamilyProperties);
}

void vkGetPhysicalDeviceMemoryProperties2(VkPhysicalDevice physicalDevice,
                                          VkPhysicalDeviceMemoryProperties2* pMemoryProperties)
{
    return ::vkGetPhysicalDeviceMemoryProperties2(physicalDevice, pMemoryProperties);
}

void vkGetPhysicalDeviceSparseImageFormatProperties2(VkPhysicalDevice physicalDevice,
                                                     const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo,
                                                     uint32_t* pPropertyCount,
                                                     VkSparseImageFormatProperties2* pProperties)
{
    return ::vkGetPhysicalDeviceSparseImageFormatProperties2(physicalDevice,
                                                             pFormatInfo,
                                                             pPropertyCount,
                                                             pProperties);
}

void vkTrimCommandPool(VkCommandPool commandPool, VkCommandPoolTrimFlags flags)
{
    return ::vkTrimCommandPool(device_, commandPool, flags);
}

void vkGetDeviceQueue2(const VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue)
{
    return ::vkGetDeviceQueue2(device_, pQueueInfo, pQueue);
}

VkResult vkCreateSamplerYcbcrConversion(const VkSamplerYcbcrConversionCreateInfo* pCreateInfo,
                                        const VkAllocationCallbacks* pAllocator,
                                        VkSamplerYcbcrConversion* pYcbcrConversion)
{
    return ::vkCreateSamplerYcbcrConversion(device_, pCreateInfo, pAllocator, pYcbcrConversion);
}

void vkDestroySamplerYcbcrConversion(VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroySamplerYcbcrConversion(device_, ycbcrConversion, pAllocator);
}

VkResult vkCreateDescriptorUpdateTemplate(const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo,
                                          const VkAllocationCallbacks* pAllocator,
                                          VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate)
{
    return ::vkCreateDescriptorUpdateTemplate(device_, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
}

void vkDestroyDescriptorUpdateTemplate(VkDescriptorUpdateTemplate descriptorUpdateTemplate,
                                       const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyDescriptorUpdateTemplate(device_, descriptorUpdateTemplate, pAllocator);
}

void vkUpdateDescriptorSetWithTemplate(VkDescriptorSet descriptorSet,
                                       VkDescriptorUpdateTemplate descriptorUpdateTemplate,
                                       const void* pData)
{
    return ::vkUpdateDescriptorSetWithTemplate(device_, descriptorSet, descriptorUpdateTemplate, pData);
}

void vkGetPhysicalDeviceExternalBufferProperties(VkPhysicalDevice physicalDevice,
                                                 const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo,
                                                 VkExternalBufferProperties* pExternalBufferProperties)
{
    return ::vkGetPhysicalDeviceExternalBufferProperties(physicalDevice,
                                                         pExternalBufferInfo,
                                                         pExternalBufferProperties);
}

void vkGetPhysicalDeviceExternalFenceProperties(VkPhysicalDevice physicalDevice,
                                                const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo,
                                                VkExternalFenceProperties* pExternalFenceProperties)
{
    return ::vkGetPhysicalDeviceExternalFenceProperties(physicalDevice,
                                                        pExternalFenceInfo,
                                                        pExternalFenceProperties);
}

void vkGetPhysicalDeviceExternalSemaphoreProperties(VkPhysicalDevice physicalDevice,
                                                    const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo,
                                                    VkExternalSemaphoreProperties* pExternalSemaphoreProperties)
{
    return ::vkGetPhysicalDeviceExternalSemaphoreProperties(physicalDevice,
                                                            pExternalSemaphoreInfo,
                                                            pExternalSemaphoreProperties);
}

void vkGetDescriptorSetLayoutSupport(const VkDescriptorSetLayoutCreateInfo* pCreateInfo,
                                     VkDescriptorSetLayoutSupport* pSupport)
{
    return ::vkGetDescriptorSetLayoutSupport(device_, pCreateInfo, pSupport);
}

void vkCmdDrawIndirectCount(VkCommandBuffer commandBuffer,
                            VkBuffer buffer,
                            VkDeviceSize offset,
                            VkBuffer countBuffer,
                            VkDeviceSize countBufferOffset,
                            uint32_t maxDrawCount,
                            uint32_t stride)
{
    return ::vkCmdDrawIndirectCount(commandBuffer,
                                    buffer,
                                    offset,
                                    countBuffer,
                                    countBufferOffset,
                                    maxDrawCount,
                                    stride);
}

void vkCmdDrawIndexedIndirectCount(VkCommandBuffer commandBuffer,
                                   VkBuffer buffer,
                                   VkDeviceSize offset,
                                   VkBuffer countBuffer,
                                   VkDeviceSize countBufferOffset,
                                   uint32_t maxDrawCount,
                                   uint32_t stride)
{
    return ::vkCmdDrawIndexedIndirectCount(commandBuffer,
                                           buffer,
                                           offset,
                                           countBuffer,
                                           countBufferOffset,
                                           maxDrawCount,
                                           stride);
}

VkResult vkCreateRenderPass2(const VkRenderPassCreateInfo2* pCreateInfo,
                             const VkAllocationCallbacks* pAllocator,
                             VkRenderPass* pRenderPass)
{
    return ::vkCreateRenderPass2(device_, pCreateInfo, pAllocator, pRenderPass);
}

void vkCmdBeginRenderPass2(VkCommandBuffer commandBuffer,
                           const VkRenderPassBeginInfo* pRenderPassBegin,
                           const VkSubpassBeginInfo* pSubpassBeginInfo)
{
    return ::vkCmdBeginRenderPass2(commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
}

void vkCmdNextSubpass2(VkCommandBuffer commandBuffer,
                       const VkSubpassBeginInfo* pSubpassBeginInfo,
                       const VkSubpassEndInfo* pSubpassEndInfo)
{
    return ::vkCmdNextSubpass2(commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);
}

void vkCmdEndRenderPass2(VkCommandBuffer commandBuffer, const VkSubpassEndInfo* pSubpassEndInfo)
{
    return ::vkCmdEndRenderPass2(commandBuffer, pSubpassEndInfo);
}

void vkResetQueryPool(VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount)
{
    return ::vkResetQueryPool(device_, queryPool, firstQuery, queryCount);
}

VkResult vkGetSemaphoreCounterValue(VkSemaphore semaphore, uint64_t* pValue)
{
    return ::vkGetSemaphoreCounterValue(device_, semaphore, pValue);
}

VkResult vkWaitSemaphores(const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout)
{
    return ::vkWaitSemaphores(device_, pWaitInfo, timeout);
}

VkResult vkSignalSemaphore(const VkSemaphoreSignalInfo* pSignalInfo)
{
    return ::vkSignalSemaphore(device_, pSignalInfo);
}

VkDeviceAddress vkGetBufferDeviceAddress(const VkBufferDeviceAddressInfo* pInfo)
{
    return ::vkGetBufferDeviceAddress(device_, pInfo);
}

uint64_t vkGetBufferOpaqueCaptureAddress(const VkBufferDeviceAddressInfo* pInfo)
{
    return ::vkGetBufferOpaqueCaptureAddress(device_, pInfo);
}

uint64_t vkGetDeviceMemoryOpaqueCaptureAddress(const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo)
{
    return ::vkGetDeviceMemoryOpaqueCaptureAddress(device_, pInfo);
}

VkResult vkGetPhysicalDeviceToolProperties(VkPhysicalDevice physicalDevice,
                                           uint32_t* pToolCount,
                                           VkPhysicalDeviceToolProperties* pToolProperties)
{
    return ::vkGetPhysicalDeviceToolProperties(physicalDevice, pToolCount, pToolProperties);
}

VkResult vkCreatePrivateDataSlot(const VkPrivateDataSlotCreateInfo* pCreateInfo,
                                 const VkAllocationCallbacks* pAllocator,
                                 VkPrivateDataSlot* pPrivateDataSlot)
{
    return ::vkCreatePrivateDataSlot(device_, pCreateInfo, pAllocator, pPrivateDataSlot);
}

void vkDestroyPrivateDataSlot(VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyPrivateDataSlot(device_, privateDataSlot, pAllocator);
}

VkResult vkSetPrivateData(VkObjectType objectType,
                          uint64_t objectHandle,
                          VkPrivateDataSlot privateDataSlot,
                          uint64_t data)
{
    return ::vkSetPrivateData(device_, objectType, objectHandle, privateDataSlot, data);
}

void vkGetPrivateData(VkObjectType objectType,
                      uint64_t objectHandle,
                      VkPrivateDataSlot privateDataSlot,
                      uint64_t* pData)
{
    return ::vkGetPrivateData(device_, objectType, objectHandle, privateDataSlot, pData);
}

void vkCmdSetEvent2(VkCommandBuffer commandBuffer, VkEvent event, const VkDependencyInfo* pDependencyInfo)
{
    return ::vkCmdSetEvent2(commandBuffer, event, pDependencyInfo);
}

void vkCmdResetEvent2(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags2 stageMask)
{
    return ::vkCmdResetEvent2(commandBuffer, event, stageMask);
}

void vkCmdWaitEvents2(VkCommandBuffer commandBuffer,
                      uint32_t eventCount,
                      const VkEvent* pEvents,
                      const VkDependencyInfo* pDependencyInfos)
{
    return ::vkCmdWaitEvents2(commandBuffer, eventCount, pEvents, pDependencyInfos);
}

void vkCmdPipelineBarrier2(VkCommandBuffer commandBuffer, const VkDependencyInfo* pDependencyInfo)
{
    return ::vkCmdPipelineBarrier2(commandBuffer, pDependencyInfo);
}

void vkCmdWriteTimestamp2(VkCommandBuffer commandBuffer,
                          VkPipelineStageFlags2 stage,
                          VkQueryPool queryPool,
                          uint32_t query)
{
    return ::vkCmdWriteTimestamp2(commandBuffer, stage, queryPool, query);
}

VkResult vkQueueSubmit2(VkQueue queue, uint32_t submitCount, const VkSubmitInfo2* pSubmits, VkFence fence)
{
    return ::vkQueueSubmit2(queue, submitCount, pSubmits, fence);
}

void vkCmdCopyBuffer2(VkCommandBuffer commandBuffer, const VkCopyBufferInfo2* pCopyBufferInfo)
{
    return ::vkCmdCopyBuffer2(commandBuffer, pCopyBufferInfo);
}

void vkCmdCopyImage2(VkCommandBuffer commandBuffer, const VkCopyImageInfo2* pCopyImageInfo)
{
    return ::vkCmdCopyImage2(commandBuffer, pCopyImageInfo);
}

void vkCmdCopyBufferToImage2(VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo)
{
    return ::vkCmdCopyBufferToImage2(commandBuffer, pCopyBufferToImageInfo);
}

void vkCmdCopyImageToBuffer2(VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo)
{
    return ::vkCmdCopyImageToBuffer2(commandBuffer, pCopyImageToBufferInfo);
}

void vkCmdBlitImage2(VkCommandBuffer commandBuffer, const VkBlitImageInfo2* pBlitImageInfo)
{
    return ::vkCmdBlitImage2(commandBuffer, pBlitImageInfo);
}

void vkCmdResolveImage2(VkCommandBuffer commandBuffer, const VkResolveImageInfo2* pResolveImageInfo)
{
    return ::vkCmdResolveImage2(commandBuffer, pResolveImageInfo);
}

void vkCmdBeginRendering(VkCommandBuffer commandBuffer, const VkRenderingInfo* pRenderingInfo)
{
    return ::vkCmdBeginRendering(commandBuffer, pRenderingInfo);
}

void vkCmdEndRendering(VkCommandBuffer commandBuffer)
{
    return ::vkCmdEndRendering(commandBuffer);
}

void vkCmdSetCullMode(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode)
{
    return ::vkCmdSetCullMode(commandBuffer, cullMode);
}

void vkCmdSetFrontFace(VkCommandBuffer commandBuffer, VkFrontFace frontFace)
{
    return ::vkCmdSetFrontFace(commandBuffer, frontFace);
}

void vkCmdSetPrimitiveTopology(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology)
{
    return ::vkCmdSetPrimitiveTopology(commandBuffer, primitiveTopology);
}

void vkCmdSetViewportWithCount(VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports)
{
    return ::vkCmdSetViewportWithCount(commandBuffer, viewportCount, pViewports);
}

void vkCmdSetScissorWithCount(VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors)
{
    return ::vkCmdSetScissorWithCount(commandBuffer, scissorCount, pScissors);
}

void vkCmdBindVertexBuffers2(VkCommandBuffer commandBuffer,
                             uint32_t firstBinding,
                             uint32_t bindingCount,
                             const VkBuffer* pBuffers,
                             const VkDeviceSize* pOffsets,
                             const VkDeviceSize* pSizes,
                             const VkDeviceSize* pStrides)
{
    return ::vkCmdBindVertexBuffers2(commandBuffer,
                                     firstBinding,
                                     bindingCount,
                                     pBuffers,
                                     pOffsets,
                                     pSizes,
                                     pStrides);
}

void vkCmdSetDepthTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable)
{
    return ::vkCmdSetDepthTestEnable(commandBuffer, depthTestEnable);
}

void vkCmdSetDepthWriteEnable(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable)
{
    return ::vkCmdSetDepthWriteEnable(commandBuffer, depthWriteEnable);
}

void vkCmdSetDepthCompareOp(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp)
{
    return ::vkCmdSetDepthCompareOp(commandBuffer, depthCompareOp);
}

void vkCmdSetDepthBoundsTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable)
{
    return ::vkCmdSetDepthBoundsTestEnable(commandBuffer, depthBoundsTestEnable);
}

void vkCmdSetStencilTestEnable(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable)
{
    return ::vkCmdSetStencilTestEnable(commandBuffer, stencilTestEnable);
}

void vkCmdSetStencilOp(VkCommandBuffer commandBuffer,
                       VkStencilFaceFlags faceMask,
                       VkStencilOp failOp,
                       VkStencilOp passOp,
                       VkStencilOp depthFailOp,
                       VkCompareOp compareOp)
{
    return ::vkCmdSetStencilOp(commandBuffer, faceMask, failOp, passOp, depthFailOp, compareOp);
}

void vkCmdSetRasterizerDiscardEnable(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable)
{
    return ::vkCmdSetRasterizerDiscardEnable(commandBuffer, rasterizerDiscardEnable);
}

void vkCmdSetDepthBiasEnable(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable)
{
    return ::vkCmdSetDepthBiasEnable(commandBuffer, depthBiasEnable);
}

void vkCmdSetPrimitiveRestartEnable(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable)
{
    return ::vkCmdSetPrimitiveRestartEnable(commandBuffer, primitiveRestartEnable);
}

void vkGetDeviceBufferMemoryRequirements(const VkDeviceBufferMemoryRequirements* pInfo,
                                         VkMemoryRequirements2* pMemoryRequirements)
{
    return ::vkGetDeviceBufferMemoryRequirements(device_, pInfo, pMemoryRequirements);
}

void vkGetDeviceImageMemoryRequirements(const VkDeviceImageMemoryRequirements* pInfo,
                                        VkMemoryRequirements2* pMemoryRequirements)
{
    return ::vkGetDeviceImageMemoryRequirements(device_, pInfo, pMemoryRequirements);
}

void vkGetDeviceImageSparseMemoryRequirements(const VkDeviceImageMemoryRequirements* pInfo,
                                              uint32_t* pSparseMemoryRequirementCount,
                                              VkSparseImageMemoryRequirements2* pSparseMemoryRequirements)
{
    return ::vkGetDeviceImageSparseMemoryRequirements(device_,
                                                      pInfo,
                                                      pSparseMemoryRequirementCount,
                                                      pSparseMemoryRequirements);
}

void vkDestroySurfaceKHR(VkInstance instance, VkSurfaceKHR surface, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroySurfaceKHR(instance, surface, pAllocator);
}

VkResult vkGetPhysicalDeviceSurfaceSupportKHR(VkPhysicalDevice physicalDevice,
                                              uint32_t queueFamilyIndex,
                                              VkSurfaceKHR surface,
                                              VkBool32* pSupported)
{
    return ::vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, surface, pSupported);
}

VkResult vkGetPhysicalDeviceSurfaceCapabilitiesKHR(VkPhysicalDevice physicalDevice,
                                                   VkSurfaceKHR surface,
                                                   VkSurfaceCapabilitiesKHR* pSurfaceCapabilities)
{
    return ::vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, pSurfaceCapabilities);
}

VkResult vkGetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice physicalDevice,
                                              VkSurfaceKHR surface,
                                              uint32_t* pSurfaceFormatCount,
                                              VkSurfaceFormatKHR* pSurfaceFormats)
{
    return ::vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats);
}

VkResult vkGetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice physicalDevice,
                                                   VkSurfaceKHR surface,
                                                   uint32_t* pPresentModeCount,
                                                   VkPresentModeKHR* pPresentModes)
{
    return ::vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, pPresentModeCount, pPresentModes);
}

VkResult vkCreateSwapchainKHR(const VkSwapchainCreateInfoKHR* pCreateInfo,
                              const VkAllocationCallbacks* pAllocator,
                              VkSwapchainKHR* pSwapchain)
{
    return ::vkCreateSwapchainKHR(device_, pCreateInfo, pAllocator, pSwapchain);
}

void vkDestroySwapchainKHR(VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroySwapchainKHR(device_, swapchain, pAllocator);
}

VkResult vkGetSwapchainImagesKHR(VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages)
{
    return ::vkGetSwapchainImagesKHR(device_, swapchain, pSwapchainImageCount, pSwapchainImages);
}

VkResult vkAcquireNextImageKHR(VkSwapchainKHR swapchain,
                               uint64_t timeout,
                               VkSemaphore semaphore,
                               VkFence fence,
                               uint32_t* pImageIndex)
{
    return ::vkAcquireNextImageKHR(device_, swapchain, timeout, semaphore, fence, pImageIndex);
}

VkResult vkQueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo)
{
    return ::vkQueuePresentKHR(queue, pPresentInfo);
}

VkResult vkGetDeviceGroupPresentCapabilitiesKHR(VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities)
{
    return ::vkGetDeviceGroupPresentCapabilitiesKHR(device_, pDeviceGroupPresentCapabilities);
}

VkResult vkGetDeviceGroupSurfacePresentModesKHR(VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes)
{
    return ::vkGetDeviceGroupSurfacePresentModesKHR(device_, surface, pModes);
}

VkResult vkGetPhysicalDevicePresentRectanglesKHR(VkPhysicalDevice physicalDevice,
                                                 VkSurfaceKHR surface,
                                                 uint32_t* pRectCount,
                                                 VkRect2D* pRects)
{
    return ::vkGetPhysicalDevicePresentRectanglesKHR(physicalDevice, surface, pRectCount, pRects);
}

VkResult vkAcquireNextImage2KHR(const VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex)
{
    return ::vkAcquireNextImage2KHR(device_, pAcquireInfo, pImageIndex);
}

VkResult vkGetPhysicalDeviceDisplayPropertiesKHR(VkPhysicalDevice physicalDevice,
                                                 uint32_t* pPropertyCount,
                                                 VkDisplayPropertiesKHR* pProperties)
{
    return ::vkGetPhysicalDeviceDisplayPropertiesKHR(physicalDevice, pPropertyCount, pProperties);
}

VkResult vkGetPhysicalDeviceDisplayPlanePropertiesKHR(VkPhysicalDevice physicalDevice,
                                                      uint32_t* pPropertyCount,
                                                      VkDisplayPlanePropertiesKHR* pProperties)
{
    return ::vkGetPhysicalDeviceDisplayPlanePropertiesKHR(physicalDevice, pPropertyCount, pProperties);
}

VkResult vkGetDisplayPlaneSupportedDisplaysKHR(VkPhysicalDevice physicalDevice,
                                               uint32_t planeIndex,
                                               uint32_t* pDisplayCount,
                                               VkDisplayKHR* pDisplays)
{
    return ::vkGetDisplayPlaneSupportedDisplaysKHR(physicalDevice, planeIndex, pDisplayCount, pDisplays);
}

VkResult vkGetDisplayModePropertiesKHR(VkPhysicalDevice physicalDevice,
                                       VkDisplayKHR display,
                                       uint32_t* pPropertyCount,
                                       VkDisplayModePropertiesKHR* pProperties)
{
    return ::vkGetDisplayModePropertiesKHR(physicalDevice, display, pPropertyCount, pProperties);
}

VkResult vkCreateDisplayModeKHR(VkPhysicalDevice physicalDevice,
                                VkDisplayKHR display,
                                const VkDisplayModeCreateInfoKHR* pCreateInfo,
                                const VkAllocationCallbacks* pAllocator,
                                VkDisplayModeKHR* pMode)
{
    return ::vkCreateDisplayModeKHR(physicalDevice, display, pCreateInfo, pAllocator, pMode);
}

VkResult vkGetDisplayPlaneCapabilitiesKHR(VkPhysicalDevice physicalDevice,
                                          VkDisplayModeKHR mode,
                                          uint32_t planeIndex,
                                          VkDisplayPlaneCapabilitiesKHR* pCapabilities)
{
    return ::vkGetDisplayPlaneCapabilitiesKHR(physicalDevice, mode, planeIndex, pCapabilities);
}

VkResult vkCreateDisplayPlaneSurfaceKHR(VkInstance instance,
                                        const VkDisplaySurfaceCreateInfoKHR* pCreateInfo,
                                        const VkAllocationCallbacks* pAllocator,
                                        VkSurfaceKHR* pSurface)
{
    return ::vkCreateDisplayPlaneSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
}

VkResult vkCreateSharedSwapchainsKHR(uint32_t swapchainCount,
                                     const VkSwapchainCreateInfoKHR* pCreateInfos,
                                     const VkAllocationCallbacks* pAllocator,
                                     VkSwapchainKHR* pSwapchains)
{
    return ::vkCreateSharedSwapchainsKHR(device_, swapchainCount, pCreateInfos, pAllocator, pSwapchains);
}

void vkCmdBeginRenderingKHR(VkCommandBuffer commandBuffer, const VkRenderingInfo* pRenderingInfo)
{
    return ::vkCmdBeginRenderingKHR(commandBuffer, pRenderingInfo);
}

void vkCmdEndRenderingKHR(VkCommandBuffer commandBuffer)
{
    return ::vkCmdEndRenderingKHR(commandBuffer);
}

void vkGetPhysicalDeviceFeatures2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures)
{
    return ::vkGetPhysicalDeviceFeatures2KHR(physicalDevice, pFeatures);
}

void vkGetPhysicalDeviceProperties2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties)
{
    return ::vkGetPhysicalDeviceProperties2KHR(physicalDevice, pProperties);
}

void vkGetPhysicalDeviceFormatProperties2KHR(VkPhysicalDevice physicalDevice,
                                             VkFormat format,
                                             VkFormatProperties2* pFormatProperties)
{
    return ::vkGetPhysicalDeviceFormatProperties2KHR(physicalDevice, format, pFormatProperties);
}

VkResult vkGetPhysicalDeviceImageFormatProperties2KHR(VkPhysicalDevice physicalDevice,
                                                      const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo,
                                                      VkImageFormatProperties2* pImageFormatProperties)
{
    return ::vkGetPhysicalDeviceImageFormatProperties2KHR(physicalDevice, pImageFormatInfo, pImageFormatProperties);
}

void vkGetPhysicalDeviceQueueFamilyProperties2KHR(VkPhysicalDevice physicalDevice,
                                                  uint32_t* pQueueFamilyPropertyCount,
                                                  VkQueueFamilyProperties2* pQueueFamilyProperties)
{
    return ::vkGetPhysicalDeviceQueueFamilyProperties2KHR(physicalDevice,
                                                          pQueueFamilyPropertyCount,
                                                          pQueueFamilyProperties);
}

void vkGetPhysicalDeviceMemoryProperties2KHR(VkPhysicalDevice physicalDevice,
                                             VkPhysicalDeviceMemoryProperties2* pMemoryProperties)
{
    return ::vkGetPhysicalDeviceMemoryProperties2KHR(physicalDevice, pMemoryProperties);
}

void vkGetPhysicalDeviceSparseImageFormatProperties2KHR(VkPhysicalDevice physicalDevice,
                                                        const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo,
                                                        uint32_t* pPropertyCount,
                                                        VkSparseImageFormatProperties2* pProperties)
{
    return ::vkGetPhysicalDeviceSparseImageFormatProperties2KHR(physicalDevice,
                                                                pFormatInfo,
                                                                pPropertyCount,
                                                                pProperties);
}

void vkGetDeviceGroupPeerMemoryFeaturesKHR(uint32_t heapIndex,
                                           uint32_t localDeviceIndex,
                                           uint32_t remoteDeviceIndex,
                                           VkPeerMemoryFeatureFlags* pPeerMemoryFeatures)
{
    return ::vkGetDeviceGroupPeerMemoryFeaturesKHR(device_,
                                                   heapIndex,
                                                   localDeviceIndex,
                                                   remoteDeviceIndex,
                                                   pPeerMemoryFeatures);
}

void vkCmdSetDeviceMaskKHR(VkCommandBuffer commandBuffer, uint32_t deviceMask)
{
    return ::vkCmdSetDeviceMaskKHR(commandBuffer, deviceMask);
}

void vkCmdDispatchBaseKHR(VkCommandBuffer commandBuffer,
                          uint32_t baseGroupX,
                          uint32_t baseGroupY,
                          uint32_t baseGroupZ,
                          uint32_t groupCountX,
                          uint32_t groupCountY,
                          uint32_t groupCountZ)
{
    return ::vkCmdDispatchBaseKHR(commandBuffer,
                                  baseGroupX,
                                  baseGroupY,
                                  baseGroupZ,
                                  groupCountX,
                                  groupCountY,
                                  groupCountZ);
}

void vkTrimCommandPoolKHR(VkCommandPool commandPool, VkCommandPoolTrimFlags flags)
{
    return ::vkTrimCommandPoolKHR(device_, commandPool, flags);
}

VkResult vkEnumeratePhysicalDeviceGroupsKHR(VkInstance instance,
                                            uint32_t* pPhysicalDeviceGroupCount,
                                            VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties)
{
    return ::vkEnumeratePhysicalDeviceGroupsKHR(instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
}

void vkGetPhysicalDeviceExternalBufferPropertiesKHR(VkPhysicalDevice physicalDevice,
                                                    const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo,
                                                    VkExternalBufferProperties* pExternalBufferProperties)
{
    return ::vkGetPhysicalDeviceExternalBufferPropertiesKHR(physicalDevice,
                                                            pExternalBufferInfo,
                                                            pExternalBufferProperties);
}

VkResult vkGetMemoryFdKHR(const VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd)
{
    return ::vkGetMemoryFdKHR(device_, pGetFdInfo, pFd);
}

VkResult vkGetMemoryFdPropertiesKHR(VkExternalMemoryHandleTypeFlagBits handleType,
                                    int fd,
                                    VkMemoryFdPropertiesKHR* pMemoryFdProperties)
{
    return ::vkGetMemoryFdPropertiesKHR(device_, handleType, fd, pMemoryFdProperties);
}

void vkGetPhysicalDeviceExternalSemaphorePropertiesKHR(VkPhysicalDevice physicalDevice,
                                                       const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo,
                                                       VkExternalSemaphoreProperties* pExternalSemaphoreProperties)
{
    return ::vkGetPhysicalDeviceExternalSemaphorePropertiesKHR(physicalDevice,
                                                               pExternalSemaphoreInfo,
                                                               pExternalSemaphoreProperties);
}

VkResult vkImportSemaphoreFdKHR(const VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo)
{
    return ::vkImportSemaphoreFdKHR(device_, pImportSemaphoreFdInfo);
}

VkResult vkGetSemaphoreFdKHR(const VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd)
{
    return ::vkGetSemaphoreFdKHR(device_, pGetFdInfo, pFd);
}

void vkCmdPushDescriptorSetKHR(VkCommandBuffer commandBuffer,
                               VkPipelineBindPoint pipelineBindPoint,
                               VkPipelineLayout layout,
                               uint32_t set,
                               uint32_t descriptorWriteCount,
                               const VkWriteDescriptorSet* pDescriptorWrites)
{
    return ::vkCmdPushDescriptorSetKHR(commandBuffer,
                                       pipelineBindPoint,
                                       layout,
                                       set,
                                       descriptorWriteCount,
                                       pDescriptorWrites);
}

void vkCmdPushDescriptorSetWithTemplateKHR(VkCommandBuffer commandBuffer,
                                           VkDescriptorUpdateTemplate descriptorUpdateTemplate,
                                           VkPipelineLayout layout,
                                           uint32_t set,
                                           const void* pData)
{
    return ::vkCmdPushDescriptorSetWithTemplateKHR(commandBuffer, descriptorUpdateTemplate, layout, set, pData);
}

VkResult vkCreateDescriptorUpdateTemplateKHR(const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo,
                                             const VkAllocationCallbacks* pAllocator,
                                             VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate)
{
    return ::vkCreateDescriptorUpdateTemplateKHR(device_, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
}

void vkDestroyDescriptorUpdateTemplateKHR(VkDescriptorUpdateTemplate descriptorUpdateTemplate,
                                          const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyDescriptorUpdateTemplateKHR(device_, descriptorUpdateTemplate, pAllocator);
}

void vkUpdateDescriptorSetWithTemplateKHR(VkDescriptorSet descriptorSet,
                                          VkDescriptorUpdateTemplate descriptorUpdateTemplate,
                                          const void* pData)
{
    return ::vkUpdateDescriptorSetWithTemplateKHR(device_, descriptorSet, descriptorUpdateTemplate, pData);
}

VkResult vkCreateRenderPass2KHR(const VkRenderPassCreateInfo2* pCreateInfo,
                                const VkAllocationCallbacks* pAllocator,
                                VkRenderPass* pRenderPass)
{
    return ::vkCreateRenderPass2KHR(device_, pCreateInfo, pAllocator, pRenderPass);
}

void vkCmdBeginRenderPass2KHR(VkCommandBuffer commandBuffer,
                              const VkRenderPassBeginInfo* pRenderPassBegin,
                              const VkSubpassBeginInfo* pSubpassBeginInfo)
{
    return ::vkCmdBeginRenderPass2KHR(commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
}

void vkCmdNextSubpass2KHR(VkCommandBuffer commandBuffer,
                          const VkSubpassBeginInfo* pSubpassBeginInfo,
                          const VkSubpassEndInfo* pSubpassEndInfo)
{
    return ::vkCmdNextSubpass2KHR(commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);
}

void vkCmdEndRenderPass2KHR(VkCommandBuffer commandBuffer, const VkSubpassEndInfo* pSubpassEndInfo)
{
    return ::vkCmdEndRenderPass2KHR(commandBuffer, pSubpassEndInfo);
}

VkResult vkGetSwapchainStatusKHR(VkSwapchainKHR swapchain)
{
    return ::vkGetSwapchainStatusKHR(device_, swapchain);
}

void vkGetPhysicalDeviceExternalFencePropertiesKHR(VkPhysicalDevice physicalDevice,
                                                   const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo,
                                                   VkExternalFenceProperties* pExternalFenceProperties)
{
    return ::vkGetPhysicalDeviceExternalFencePropertiesKHR(physicalDevice,
                                                           pExternalFenceInfo,
                                                           pExternalFenceProperties);
}

VkResult vkImportFenceFdKHR(const VkImportFenceFdInfoKHR* pImportFenceFdInfo)
{
    return ::vkImportFenceFdKHR(device_, pImportFenceFdInfo);
}

VkResult vkGetFenceFdKHR(const VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd)
{
    return ::vkGetFenceFdKHR(device_, pGetFdInfo, pFd);
}

VkResult vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(VkPhysicalDevice physicalDevice,
                                                                         uint32_t queueFamilyIndex,
                                                                         uint32_t* pCounterCount,
                                                                         VkPerformanceCounterKHR* pCounters,
                                                                         VkPerformanceCounterDescriptionKHR* pCounterDescriptions)
{
    return ::vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(physicalDevice,
                                                                             queueFamilyIndex,
                                                                             pCounterCount,
                                                                             pCounters,
                                                                             pCounterDescriptions);
}

void vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(VkPhysicalDevice physicalDevice,
                                                             const VkQueryPoolPerformanceCreateInfoKHR* pPerformanceQueryCreateInfo,
                                                             uint32_t* pNumPasses)
{
    return ::vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(physicalDevice,
                                                                     pPerformanceQueryCreateInfo,
                                                                     pNumPasses);
}

VkResult vkAcquireProfilingLockKHR(const VkAcquireProfilingLockInfoKHR* pInfo)
{
    return ::vkAcquireProfilingLockKHR(device_, pInfo);
}

void vkReleaseProfilingLockKHR()
{
    return ::vkReleaseProfilingLockKHR(device_);
}

VkResult vkGetPhysicalDeviceSurfaceCapabilities2KHR(VkPhysicalDevice physicalDevice,
                                                    const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo,
                                                    VkSurfaceCapabilities2KHR* pSurfaceCapabilities)
{
    return ::vkGetPhysicalDeviceSurfaceCapabilities2KHR(physicalDevice, pSurfaceInfo, pSurfaceCapabilities);
}

VkResult vkGetPhysicalDeviceSurfaceFormats2KHR(VkPhysicalDevice physicalDevice,
                                               const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo,
                                               uint32_t* pSurfaceFormatCount,
                                               VkSurfaceFormat2KHR* pSurfaceFormats)
{
    return ::vkGetPhysicalDeviceSurfaceFormats2KHR(physicalDevice,
                                                   pSurfaceInfo,
                                                   pSurfaceFormatCount,
                                                   pSurfaceFormats);
}

VkResult vkGetPhysicalDeviceDisplayProperties2KHR(VkPhysicalDevice physicalDevice,
                                                  uint32_t* pPropertyCount,
                                                  VkDisplayProperties2KHR* pProperties)
{
    return ::vkGetPhysicalDeviceDisplayProperties2KHR(physicalDevice, pPropertyCount, pProperties);
}

VkResult vkGetPhysicalDeviceDisplayPlaneProperties2KHR(VkPhysicalDevice physicalDevice,
                                                       uint32_t* pPropertyCount,
                                                       VkDisplayPlaneProperties2KHR* pProperties)
{
    return ::vkGetPhysicalDeviceDisplayPlaneProperties2KHR(physicalDevice, pPropertyCount, pProperties);
}

VkResult vkGetDisplayModeProperties2KHR(VkPhysicalDevice physicalDevice,
                                        VkDisplayKHR display,
                                        uint32_t* pPropertyCount,
                                        VkDisplayModeProperties2KHR* pProperties)
{
    return ::vkGetDisplayModeProperties2KHR(physicalDevice, display, pPropertyCount, pProperties);
}

VkResult vkGetDisplayPlaneCapabilities2KHR(VkPhysicalDevice physicalDevice,
                                           const VkDisplayPlaneInfo2KHR* pDisplayPlaneInfo,
                                           VkDisplayPlaneCapabilities2KHR* pCapabilities)
{
    return ::vkGetDisplayPlaneCapabilities2KHR(physicalDevice, pDisplayPlaneInfo, pCapabilities);
}

void vkGetImageMemoryRequirements2KHR(const VkImageMemoryRequirementsInfo2* pInfo,
                                      VkMemoryRequirements2* pMemoryRequirements)
{
    return ::vkGetImageMemoryRequirements2KHR(device_, pInfo, pMemoryRequirements);
}

void vkGetBufferMemoryRequirements2KHR(const VkBufferMemoryRequirementsInfo2* pInfo,
                                       VkMemoryRequirements2* pMemoryRequirements)
{
    return ::vkGetBufferMemoryRequirements2KHR(device_, pInfo, pMemoryRequirements);
}

void vkGetImageSparseMemoryRequirements2KHR(const VkImageSparseMemoryRequirementsInfo2* pInfo,
                                            uint32_t* pSparseMemoryRequirementCount,
                                            VkSparseImageMemoryRequirements2* pSparseMemoryRequirements)
{
    return ::vkGetImageSparseMemoryRequirements2KHR(device_,
                                                    pInfo,
                                                    pSparseMemoryRequirementCount,
                                                    pSparseMemoryRequirements);
}

VkResult vkCreateSamplerYcbcrConversionKHR(const VkSamplerYcbcrConversionCreateInfo* pCreateInfo,
                                           const VkAllocationCallbacks* pAllocator,
                                           VkSamplerYcbcrConversion* pYcbcrConversion)
{
    return ::vkCreateSamplerYcbcrConversionKHR(device_, pCreateInfo, pAllocator, pYcbcrConversion);
}

void vkDestroySamplerYcbcrConversionKHR(VkSamplerYcbcrConversion ycbcrConversion,
                                        const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroySamplerYcbcrConversionKHR(device_, ycbcrConversion, pAllocator);
}

VkResult vkBindBufferMemory2KHR(uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos)
{
    return ::vkBindBufferMemory2KHR(device_, bindInfoCount, pBindInfos);
}

VkResult vkBindImageMemory2KHR(uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos)
{
    return ::vkBindImageMemory2KHR(device_, bindInfoCount, pBindInfos);
}

void vkGetDescriptorSetLayoutSupportKHR(const VkDescriptorSetLayoutCreateInfo* pCreateInfo,
                                        VkDescriptorSetLayoutSupport* pSupport)
{
    return ::vkGetDescriptorSetLayoutSupportKHR(device_, pCreateInfo, pSupport);
}

void vkCmdDrawIndirectCountKHR(VkCommandBuffer commandBuffer,
                               VkBuffer buffer,
                               VkDeviceSize offset,
                               VkBuffer countBuffer,
                               VkDeviceSize countBufferOffset,
                               uint32_t maxDrawCount,
                               uint32_t stride)
{
    return ::vkCmdDrawIndirectCountKHR(commandBuffer,
                                       buffer,
                                       offset,
                                       countBuffer,
                                       countBufferOffset,
                                       maxDrawCount,
                                       stride);
}

void vkCmdDrawIndexedIndirectCountKHR(VkCommandBuffer commandBuffer,
                                      VkBuffer buffer,
                                      VkDeviceSize offset,
                                      VkBuffer countBuffer,
                                      VkDeviceSize countBufferOffset,
                                      uint32_t maxDrawCount,
                                      uint32_t stride)
{
    return ::vkCmdDrawIndexedIndirectCountKHR(commandBuffer,
                                              buffer,
                                              offset,
                                              countBuffer,
                                              countBufferOffset,
                                              maxDrawCount,
                                              stride);
}

VkResult vkGetSemaphoreCounterValueKHR(VkSemaphore semaphore, uint64_t* pValue)
{
    return ::vkGetSemaphoreCounterValueKHR(device_, semaphore, pValue);
}

VkResult vkWaitSemaphoresKHR(const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout)
{
    return ::vkWaitSemaphoresKHR(device_, pWaitInfo, timeout);
}

VkResult vkSignalSemaphoreKHR(const VkSemaphoreSignalInfo* pSignalInfo)
{
    return ::vkSignalSemaphoreKHR(device_, pSignalInfo);
}

VkResult vkGetPhysicalDeviceFragmentShadingRatesKHR(VkPhysicalDevice physicalDevice,
                                                    uint32_t* pFragmentShadingRateCount,
                                                    VkPhysicalDeviceFragmentShadingRateKHR* pFragmentShadingRates)
{
    return ::vkGetPhysicalDeviceFragmentShadingRatesKHR(physicalDevice,
                                                        pFragmentShadingRateCount,
                                                        pFragmentShadingRates);
}

void vkCmdSetFragmentShadingRateKHR(VkCommandBuffer commandBuffer,
                                    const VkExtent2D* pFragmentSize,
                                    const VkFragmentShadingRateCombinerOpKHR combinerOps[2])
{
    return ::vkCmdSetFragmentShadingRateKHR(commandBuffer, pFragmentSize, combinerOps);
}

VkResult vkWaitForPresentKHR(VkSwapchainKHR swapchain, uint64_t presentId, uint64_t timeout)
{
    return ::vkWaitForPresentKHR(device_, swapchain, presentId, timeout);
}

VkDeviceAddress vkGetBufferDeviceAddressKHR(const VkBufferDeviceAddressInfo* pInfo)
{
    return ::vkGetBufferDeviceAddressKHR(device_, pInfo);
}

uint64_t vkGetBufferOpaqueCaptureAddressKHR(const VkBufferDeviceAddressInfo* pInfo)
{
    return ::vkGetBufferOpaqueCaptureAddressKHR(device_, pInfo);
}

uint64_t vkGetDeviceMemoryOpaqueCaptureAddressKHR(const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo)
{
    return ::vkGetDeviceMemoryOpaqueCaptureAddressKHR(device_, pInfo);
}

VkResult vkCreateDeferredOperationKHR(const VkAllocationCallbacks* pAllocator,
                                      VkDeferredOperationKHR* pDeferredOperation)
{
    return ::vkCreateDeferredOperationKHR(device_, pAllocator, pDeferredOperation);
}

void vkDestroyDeferredOperationKHR(VkDeferredOperationKHR operation, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyDeferredOperationKHR(device_, operation, pAllocator);
}

uint32_t vkGetDeferredOperationMaxConcurrencyKHR(VkDeferredOperationKHR operation)
{
    return ::vkGetDeferredOperationMaxConcurrencyKHR(device_, operation);
}

VkResult vkGetDeferredOperationResultKHR(VkDeferredOperationKHR operation)
{
    return ::vkGetDeferredOperationResultKHR(device_, operation);
}

VkResult vkDeferredOperationJoinKHR(VkDeferredOperationKHR operation)
{
    return ::vkDeferredOperationJoinKHR(device_, operation);
}

VkResult vkGetPipelineExecutablePropertiesKHR(const VkPipelineInfoKHR* pPipelineInfo,
                                              uint32_t* pExecutableCount,
                                              VkPipelineExecutablePropertiesKHR* pProperties)
{
    return ::vkGetPipelineExecutablePropertiesKHR(device_, pPipelineInfo, pExecutableCount, pProperties);
}

VkResult vkGetPipelineExecutableStatisticsKHR(const VkPipelineExecutableInfoKHR* pExecutableInfo,
                                              uint32_t* pStatisticCount,
                                              VkPipelineExecutableStatisticKHR* pStatistics)
{
    return ::vkGetPipelineExecutableStatisticsKHR(device_, pExecutableInfo, pStatisticCount, pStatistics);
}

VkResult vkGetPipelineExecutableInternalRepresentationsKHR(const VkPipelineExecutableInfoKHR* pExecutableInfo,
                                                           uint32_t* pInternalRepresentationCount,
                                                           VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations)
{
    return ::vkGetPipelineExecutableInternalRepresentationsKHR(device_,
                                                               pExecutableInfo,
                                                               pInternalRepresentationCount,
                                                               pInternalRepresentations);
}

void vkCmdSetEvent2KHR(VkCommandBuffer commandBuffer, VkEvent event, const VkDependencyInfo* pDependencyInfo)
{
    return ::vkCmdSetEvent2KHR(commandBuffer, event, pDependencyInfo);
}

void vkCmdResetEvent2KHR(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags2 stageMask)
{
    return ::vkCmdResetEvent2KHR(commandBuffer, event, stageMask);
}

void vkCmdWaitEvents2KHR(VkCommandBuffer commandBuffer,
                         uint32_t eventCount,
                         const VkEvent* pEvents,
                         const VkDependencyInfo* pDependencyInfos)
{
    return ::vkCmdWaitEvents2KHR(commandBuffer, eventCount, pEvents, pDependencyInfos);
}

void vkCmdPipelineBarrier2KHR(VkCommandBuffer commandBuffer, const VkDependencyInfo* pDependencyInfo)
{
    return ::vkCmdPipelineBarrier2KHR(commandBuffer, pDependencyInfo);
}

void vkCmdWriteTimestamp2KHR(VkCommandBuffer commandBuffer,
                             VkPipelineStageFlags2 stage,
                             VkQueryPool queryPool,
                             uint32_t query)
{
    return ::vkCmdWriteTimestamp2KHR(commandBuffer, stage, queryPool, query);
}

VkResult vkQueueSubmit2KHR(VkQueue queue, uint32_t submitCount, const VkSubmitInfo2* pSubmits, VkFence fence)
{
    return ::vkQueueSubmit2KHR(queue, submitCount, pSubmits, fence);
}

void vkCmdWriteBufferMarker2AMD(VkCommandBuffer commandBuffer,
                                VkPipelineStageFlags2 stage,
                                VkBuffer dstBuffer,
                                VkDeviceSize dstOffset,
                                uint32_t marker)
{
    return ::vkCmdWriteBufferMarker2AMD(commandBuffer, stage, dstBuffer, dstOffset, marker);
}

void vkGetQueueCheckpointData2NV(VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointData2NV* pCheckpointData)
{
    return ::vkGetQueueCheckpointData2NV(queue, pCheckpointDataCount, pCheckpointData);
}

void vkCmdCopyBuffer2KHR(VkCommandBuffer commandBuffer, const VkCopyBufferInfo2* pCopyBufferInfo)
{
    return ::vkCmdCopyBuffer2KHR(commandBuffer, pCopyBufferInfo);
}

void vkCmdCopyImage2KHR(VkCommandBuffer commandBuffer, const VkCopyImageInfo2* pCopyImageInfo)
{
    return ::vkCmdCopyImage2KHR(commandBuffer, pCopyImageInfo);
}

void vkCmdCopyBufferToImage2KHR(VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo)
{
    return ::vkCmdCopyBufferToImage2KHR(commandBuffer, pCopyBufferToImageInfo);
}

void vkCmdCopyImageToBuffer2KHR(VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo)
{
    return ::vkCmdCopyImageToBuffer2KHR(commandBuffer, pCopyImageToBufferInfo);
}

void vkCmdBlitImage2KHR(VkCommandBuffer commandBuffer, const VkBlitImageInfo2* pBlitImageInfo)
{
    return ::vkCmdBlitImage2KHR(commandBuffer, pBlitImageInfo);
}

void vkCmdResolveImage2KHR(VkCommandBuffer commandBuffer, const VkResolveImageInfo2* pResolveImageInfo)
{
    return ::vkCmdResolveImage2KHR(commandBuffer, pResolveImageInfo);
}

void vkGetDeviceBufferMemoryRequirementsKHR(const VkDeviceBufferMemoryRequirements* pInfo,
                                            VkMemoryRequirements2* pMemoryRequirements)
{
    return ::vkGetDeviceBufferMemoryRequirementsKHR(device_, pInfo, pMemoryRequirements);
}

void vkGetDeviceImageMemoryRequirementsKHR(const VkDeviceImageMemoryRequirements* pInfo,
                                           VkMemoryRequirements2* pMemoryRequirements)
{
    return ::vkGetDeviceImageMemoryRequirementsKHR(device_, pInfo, pMemoryRequirements);
}

void vkGetDeviceImageSparseMemoryRequirementsKHR(const VkDeviceImageMemoryRequirements* pInfo,
                                                 uint32_t* pSparseMemoryRequirementCount,
                                                 VkSparseImageMemoryRequirements2* pSparseMemoryRequirements)
{
    return ::vkGetDeviceImageSparseMemoryRequirementsKHR(device_,
                                                         pInfo,
                                                         pSparseMemoryRequirementCount,
                                                         pSparseMemoryRequirements);
}

VkResult vkCreateDebugReportCallbackEXT(VkInstance instance,
                                        const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
                                        const VkAllocationCallbacks* pAllocator,
                                        VkDebugReportCallbackEXT* pCallback)
{
    return ::vkCreateDebugReportCallbackEXT(instance, pCreateInfo, pAllocator, pCallback);
}

void vkDestroyDebugReportCallbackEXT(VkInstance instance,
                                     VkDebugReportCallbackEXT callback,
                                     const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyDebugReportCallbackEXT(instance, callback, pAllocator);
}

void vkDebugReportMessageEXT(VkInstance instance,
                             VkDebugReportFlagsEXT flags,
                             VkDebugReportObjectTypeEXT objectType,
                             uint64_t object,
                             size_t location,
                             int32_t messageCode,
                             const char* pLayerPrefix,
                             const char* pMessage)
{
    return ::vkDebugReportMessageEXT(instance,
                                     flags,
                                     objectType,
                                     object,
                                     location,
                                     messageCode,
                                     pLayerPrefix,
                                     pMessage);
}

VkResult vkDebugMarkerSetObjectTagEXT(const VkDebugMarkerObjectTagInfoEXT* pTagInfo)
{
    return ::vkDebugMarkerSetObjectTagEXT(device_, pTagInfo);
}

VkResult vkDebugMarkerSetObjectNameEXT(const VkDebugMarkerObjectNameInfoEXT* pNameInfo)
{
    return ::vkDebugMarkerSetObjectNameEXT(device_, pNameInfo);
}

void vkCmdDebugMarkerBeginEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo)
{
    return ::vkCmdDebugMarkerBeginEXT(commandBuffer, pMarkerInfo);
}

void vkCmdDebugMarkerEndEXT(VkCommandBuffer commandBuffer)
{
    return ::vkCmdDebugMarkerEndEXT(commandBuffer);
}

void vkCmdDebugMarkerInsertEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo)
{
    return ::vkCmdDebugMarkerInsertEXT(commandBuffer, pMarkerInfo);
}

void vkCmdBindTransformFeedbackBuffersEXT(VkCommandBuffer commandBuffer,
                                          uint32_t firstBinding,
                                          uint32_t bindingCount,
                                          const VkBuffer* pBuffers,
                                          const VkDeviceSize* pOffsets,
                                          const VkDeviceSize* pSizes)
{
    return ::vkCmdBindTransformFeedbackBuffersEXT(commandBuffer,
                                                  firstBinding,
                                                  bindingCount,
                                                  pBuffers,
                                                  pOffsets,
                                                  pSizes);
}

void vkCmdBeginTransformFeedbackEXT(VkCommandBuffer commandBuffer,
                                    uint32_t firstCounterBuffer,
                                    uint32_t counterBufferCount,
                                    const VkBuffer* pCounterBuffers,
                                    const VkDeviceSize* pCounterBufferOffsets)
{
    return ::vkCmdBeginTransformFeedbackEXT(commandBuffer,
                                            firstCounterBuffer,
                                            counterBufferCount,
                                            pCounterBuffers,
                                            pCounterBufferOffsets);
}

void vkCmdEndTransformFeedbackEXT(VkCommandBuffer commandBuffer,
                                  uint32_t firstCounterBuffer,
                                  uint32_t counterBufferCount,
                                  const VkBuffer* pCounterBuffers,
                                  const VkDeviceSize* pCounterBufferOffsets)
{
    return ::vkCmdEndTransformFeedbackEXT(commandBuffer,
                                          firstCounterBuffer,
                                          counterBufferCount,
                                          pCounterBuffers,
                                          pCounterBufferOffsets);
}

void vkCmdBeginQueryIndexedEXT(VkCommandBuffer commandBuffer,
                               VkQueryPool queryPool,
                               uint32_t query,
                               VkQueryControlFlags flags,
                               uint32_t index)
{
    return ::vkCmdBeginQueryIndexedEXT(commandBuffer, queryPool, query, flags, index);
}

void vkCmdEndQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index)
{
    return ::vkCmdEndQueryIndexedEXT(commandBuffer, queryPool, query, index);
}

void vkCmdDrawIndirectByteCountEXT(VkCommandBuffer commandBuffer,
                                   uint32_t instanceCount,
                                   uint32_t firstInstance,
                                   VkBuffer counterBuffer,
                                   VkDeviceSize counterBufferOffset,
                                   uint32_t counterOffset,
                                   uint32_t vertexStride)
{
    return ::vkCmdDrawIndirectByteCountEXT(commandBuffer,
                                           instanceCount,
                                           firstInstance,
                                           counterBuffer,
                                           counterBufferOffset,
                                           counterOffset,
                                           vertexStride);
}

VkResult vkCreateCuModuleNVX(const VkCuModuleCreateInfoNVX* pCreateInfo,
                             const VkAllocationCallbacks* pAllocator,
                             VkCuModuleNVX* pModule)
{
    return ::vkCreateCuModuleNVX(device_, pCreateInfo, pAllocator, pModule);
}

VkResult vkCreateCuFunctionNVX(const VkCuFunctionCreateInfoNVX* pCreateInfo,
                               const VkAllocationCallbacks* pAllocator,
                               VkCuFunctionNVX* pFunction)
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

void vkCmdCuLaunchKernelNVX(VkCommandBuffer commandBuffer, const VkCuLaunchInfoNVX* pLaunchInfo)
{
    return ::vkCmdCuLaunchKernelNVX(commandBuffer, pLaunchInfo);
}

uint32_t vkGetImageViewHandleNVX(const VkImageViewHandleInfoNVX* pInfo)
{
    return ::vkGetImageViewHandleNVX(device_, pInfo);
}

VkResult vkGetImageViewAddressNVX(VkImageView imageView, VkImageViewAddressPropertiesNVX* pProperties)
{
    return ::vkGetImageViewAddressNVX(device_, imageView, pProperties);
}

void vkCmdDrawIndirectCountAMD(VkCommandBuffer commandBuffer,
                               VkBuffer buffer,
                               VkDeviceSize offset,
                               VkBuffer countBuffer,
                               VkDeviceSize countBufferOffset,
                               uint32_t maxDrawCount,
                               uint32_t stride)
{
    return ::vkCmdDrawIndirectCountAMD(commandBuffer,
                                       buffer,
                                       offset,
                                       countBuffer,
                                       countBufferOffset,
                                       maxDrawCount,
                                       stride);
}

void vkCmdDrawIndexedIndirectCountAMD(VkCommandBuffer commandBuffer,
                                      VkBuffer buffer,
                                      VkDeviceSize offset,
                                      VkBuffer countBuffer,
                                      VkDeviceSize countBufferOffset,
                                      uint32_t maxDrawCount,
                                      uint32_t stride)
{
    return ::vkCmdDrawIndexedIndirectCountAMD(commandBuffer,
                                              buffer,
                                              offset,
                                              countBuffer,
                                              countBufferOffset,
                                              maxDrawCount,
                                              stride);
}

VkResult vkGetShaderInfoAMD(VkPipeline pipeline,
                            VkShaderStageFlagBits shaderStage,
                            VkShaderInfoTypeAMD infoType,
                            size_t* pInfoSize,
                            void* pInfo)
{
    return ::vkGetShaderInfoAMD(device_, pipeline, shaderStage, infoType, pInfoSize, pInfo);
}

VkResult vkGetPhysicalDeviceExternalImageFormatPropertiesNV(VkPhysicalDevice physicalDevice,
                                                            VkFormat format,
                                                            VkImageType type,
                                                            VkImageTiling tiling,
                                                            VkImageUsageFlags usage,
                                                            VkImageCreateFlags flags,
                                                            VkExternalMemoryHandleTypeFlagsNV externalHandleType,
                                                            VkExternalImageFormatPropertiesNV* pExternalImageFormatProperties)
{
    return ::vkGetPhysicalDeviceExternalImageFormatPropertiesNV(physicalDevice,
                                                                format,
                                                                type,
                                                                tiling,
                                                                usage,
                                                                flags,
                                                                externalHandleType,
                                                                pExternalImageFormatProperties);
}

void vkCmdBeginConditionalRenderingEXT(VkCommandBuffer commandBuffer,
                                       const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin)
{
    return ::vkCmdBeginConditionalRenderingEXT(commandBuffer, pConditionalRenderingBegin);
}

void vkCmdEndConditionalRenderingEXT(VkCommandBuffer commandBuffer)
{
    return ::vkCmdEndConditionalRenderingEXT(commandBuffer);
}

void vkCmdSetViewportWScalingNV(VkCommandBuffer commandBuffer,
                                uint32_t firstViewport,
                                uint32_t viewportCount,
                                const VkViewportWScalingNV* pViewportWScalings)
{
    return ::vkCmdSetViewportWScalingNV(commandBuffer, firstViewport, viewportCount, pViewportWScalings);
}

VkResult vkReleaseDisplayEXT(VkPhysicalDevice physicalDevice, VkDisplayKHR display)
{
    return ::vkReleaseDisplayEXT(physicalDevice, display);
}

VkResult vkGetPhysicalDeviceSurfaceCapabilities2EXT(VkPhysicalDevice physicalDevice,
                                                    VkSurfaceKHR surface,
                                                    VkSurfaceCapabilities2EXT* pSurfaceCapabilities)
{
    return ::vkGetPhysicalDeviceSurfaceCapabilities2EXT(physicalDevice, surface, pSurfaceCapabilities);
}

VkResult vkDisplayPowerControlEXT(VkDisplayKHR display, const VkDisplayPowerInfoEXT* pDisplayPowerInfo)
{
    return ::vkDisplayPowerControlEXT(device_, display, pDisplayPowerInfo);
}

VkResult vkRegisterDeviceEventEXT(const VkDeviceEventInfoEXT* pDeviceEventInfo,
                                  const VkAllocationCallbacks* pAllocator,
                                  VkFence* pFence)
{
    return ::vkRegisterDeviceEventEXT(device_, pDeviceEventInfo, pAllocator, pFence);
}

VkResult vkRegisterDisplayEventEXT(VkDisplayKHR display,
                                   const VkDisplayEventInfoEXT* pDisplayEventInfo,
                                   const VkAllocationCallbacks* pAllocator,
                                   VkFence* pFence)
{
    return ::vkRegisterDisplayEventEXT(device_, display, pDisplayEventInfo, pAllocator, pFence);
}

VkResult vkGetSwapchainCounterEXT(VkSwapchainKHR swapchain,
                                  VkSurfaceCounterFlagBitsEXT counter,
                                  uint64_t* pCounterValue)
{
    return ::vkGetSwapchainCounterEXT(device_, swapchain, counter, pCounterValue);
}

VkResult vkGetRefreshCycleDurationGOOGLE(VkSwapchainKHR swapchain,
                                         VkRefreshCycleDurationGOOGLE* pDisplayTimingProperties)
{
    return ::vkGetRefreshCycleDurationGOOGLE(device_, swapchain, pDisplayTimingProperties);
}

VkResult vkGetPastPresentationTimingGOOGLE(VkSwapchainKHR swapchain,
                                           uint32_t* pPresentationTimingCount,
                                           VkPastPresentationTimingGOOGLE* pPresentationTimings)
{
    return ::vkGetPastPresentationTimingGOOGLE(device_, swapchain, pPresentationTimingCount, pPresentationTimings);
}

void vkCmdSetDiscardRectangleEXT(VkCommandBuffer commandBuffer,
                                 uint32_t firstDiscardRectangle,
                                 uint32_t discardRectangleCount,
                                 const VkRect2D* pDiscardRectangles)
{
    return ::vkCmdSetDiscardRectangleEXT(commandBuffer,
                                         firstDiscardRectangle,
                                         discardRectangleCount,
                                         pDiscardRectangles);
}

void vkSetHdrMetadataEXT(uint32_t swapchainCount, const VkSwapchainKHR* pSwapchains, const VkHdrMetadataEXT* pMetadata)
{
    return ::vkSetHdrMetadataEXT(device_, swapchainCount, pSwapchains, pMetadata);
}

VkResult vkSetDebugUtilsObjectNameEXT(const VkDebugUtilsObjectNameInfoEXT* pNameInfo)
{
    return ::vkSetDebugUtilsObjectNameEXT(device_, pNameInfo);
}

VkResult vkSetDebugUtilsObjectTagEXT(const VkDebugUtilsObjectTagInfoEXT* pTagInfo)
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

void vkCmdBeginDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo)
{
    return ::vkCmdBeginDebugUtilsLabelEXT(commandBuffer, pLabelInfo);
}

void vkCmdEndDebugUtilsLabelEXT(VkCommandBuffer commandBuffer)
{
    return ::vkCmdEndDebugUtilsLabelEXT(commandBuffer);
}

void vkCmdInsertDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo)
{
    return ::vkCmdInsertDebugUtilsLabelEXT(commandBuffer, pLabelInfo);
}

VkResult vkCreateDebugUtilsMessengerEXT(VkInstance instance,
                                        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                        const VkAllocationCallbacks* pAllocator,
                                        VkDebugUtilsMessengerEXT* pMessenger)
{
    return ::vkCreateDebugUtilsMessengerEXT(instance, pCreateInfo, pAllocator, pMessenger);
}

void vkDestroyDebugUtilsMessengerEXT(VkInstance instance,
                                     VkDebugUtilsMessengerEXT messenger,
                                     const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyDebugUtilsMessengerEXT(instance, messenger, pAllocator);
}

void vkSubmitDebugUtilsMessageEXT(VkInstance instance,
                                  VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                  VkDebugUtilsMessageTypeFlagsEXT messageTypes,
                                  const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData)
{
    return ::vkSubmitDebugUtilsMessageEXT(instance, messageSeverity, messageTypes, pCallbackData);
}

void vkCmdSetSampleLocationsEXT(VkCommandBuffer commandBuffer, const VkSampleLocationsInfoEXT* pSampleLocationsInfo)
{
    return ::vkCmdSetSampleLocationsEXT(commandBuffer, pSampleLocationsInfo);
}

void vkGetPhysicalDeviceMultisamplePropertiesEXT(VkPhysicalDevice physicalDevice,
                                                 VkSampleCountFlagBits samples,
                                                 VkMultisamplePropertiesEXT* pMultisampleProperties)
{
    return ::vkGetPhysicalDeviceMultisamplePropertiesEXT(physicalDevice, samples, pMultisampleProperties);
}

VkResult vkGetImageDrmFormatModifierPropertiesEXT(VkImage image, VkImageDrmFormatModifierPropertiesEXT* pProperties)
{
    return ::vkGetImageDrmFormatModifierPropertiesEXT(device_, image, pProperties);
}

VkResult vkCreateValidationCacheEXT(const VkValidationCacheCreateInfoEXT* pCreateInfo,
                                    const VkAllocationCallbacks* pAllocator,
                                    VkValidationCacheEXT* pValidationCache)
{
    return ::vkCreateValidationCacheEXT(device_, pCreateInfo, pAllocator, pValidationCache);
}

void vkDestroyValidationCacheEXT(VkValidationCacheEXT validationCache, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyValidationCacheEXT(device_, validationCache, pAllocator);
}

VkResult vkMergeValidationCachesEXT(VkValidationCacheEXT dstCache,
                                    uint32_t srcCacheCount,
                                    const VkValidationCacheEXT* pSrcCaches)
{
    return ::vkMergeValidationCachesEXT(device_, dstCache, srcCacheCount, pSrcCaches);
}

VkResult vkGetValidationCacheDataEXT(VkValidationCacheEXT validationCache, size_t* pDataSize, void* pData)
{
    return ::vkGetValidationCacheDataEXT(device_, validationCache, pDataSize, pData);
}

void vkCmdBindShadingRateImageNV(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout)
{
    return ::vkCmdBindShadingRateImageNV(commandBuffer, imageView, imageLayout);
}

void vkCmdSetViewportShadingRatePaletteNV(VkCommandBuffer commandBuffer,
                                          uint32_t firstViewport,
                                          uint32_t viewportCount,
                                          const VkShadingRatePaletteNV* pShadingRatePalettes)
{
    return ::vkCmdSetViewportShadingRatePaletteNV(commandBuffer, firstViewport, viewportCount, pShadingRatePalettes);
}

void vkCmdSetCoarseSampleOrderNV(VkCommandBuffer commandBuffer,
                                 VkCoarseSampleOrderTypeNV sampleOrderType,
                                 uint32_t customSampleOrderCount,
                                 const VkCoarseSampleOrderCustomNV* pCustomSampleOrders)
{
    return ::vkCmdSetCoarseSampleOrderNV(commandBuffer,
                                         sampleOrderType,
                                         customSampleOrderCount,
                                         pCustomSampleOrders);
}

VkResult vkCreateAccelerationStructureNV(const VkAccelerationStructureCreateInfoNV* pCreateInfo,
                                         const VkAllocationCallbacks* pAllocator,
                                         VkAccelerationStructureNV* pAccelerationStructure)
{
    return ::vkCreateAccelerationStructureNV(device_, pCreateInfo, pAllocator, pAccelerationStructure);
}

void vkDestroyAccelerationStructureNV(VkAccelerationStructureNV accelerationStructure,
                                      const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyAccelerationStructureNV(device_, accelerationStructure, pAllocator);
}

void vkGetAccelerationStructureMemoryRequirementsNV(const VkAccelerationStructureMemoryRequirementsInfoNV* pInfo,
                                                    VkMemoryRequirements2KHR* pMemoryRequirements)
{
    return ::vkGetAccelerationStructureMemoryRequirementsNV(device_, pInfo, pMemoryRequirements);
}

VkResult vkBindAccelerationStructureMemoryNV(uint32_t bindInfoCount,
                                             const VkBindAccelerationStructureMemoryInfoNV* pBindInfos)
{
    return ::vkBindAccelerationStructureMemoryNV(device_, bindInfoCount, pBindInfos);
}

void vkCmdBuildAccelerationStructureNV(VkCommandBuffer commandBuffer,
                                       const VkAccelerationStructureInfoNV* pInfo,
                                       VkBuffer instanceData,
                                       VkDeviceSize instanceOffset,
                                       VkBool32 update,
                                       VkAccelerationStructureNV dst,
                                       VkAccelerationStructureNV src,
                                       VkBuffer scratch,
                                       VkDeviceSize scratchOffset)
{
    return ::vkCmdBuildAccelerationStructureNV(commandBuffer,
                                               pInfo,
                                               instanceData,
                                               instanceOffset,
                                               update,
                                               dst,
                                               src,
                                               scratch,
                                               scratchOffset);
}

void vkCmdCopyAccelerationStructureNV(VkCommandBuffer commandBuffer,
                                      VkAccelerationStructureNV dst,
                                      VkAccelerationStructureNV src,
                                      VkCopyAccelerationStructureModeKHR mode)
{
    return ::vkCmdCopyAccelerationStructureNV(commandBuffer, dst, src, mode);
}

void vkCmdTraceRaysNV(VkCommandBuffer commandBuffer,
                      VkBuffer raygenShaderBindingTableBuffer,
                      VkDeviceSize raygenShaderBindingOffset,
                      VkBuffer missShaderBindingTableBuffer,
                      VkDeviceSize missShaderBindingOffset,
                      VkDeviceSize missShaderBindingStride,
                      VkBuffer hitShaderBindingTableBuffer,
                      VkDeviceSize hitShaderBindingOffset,
                      VkDeviceSize hitShaderBindingStride,
                      VkBuffer callableShaderBindingTableBuffer,
                      VkDeviceSize callableShaderBindingOffset,
                      VkDeviceSize callableShaderBindingStride,
                      uint32_t width,
                      uint32_t height,
                      uint32_t depth)
{
    return ::vkCmdTraceRaysNV(commandBuffer,
                              raygenShaderBindingTableBuffer,
                              raygenShaderBindingOffset,
                              missShaderBindingTableBuffer,
                              missShaderBindingOffset,
                              missShaderBindingStride,
                              hitShaderBindingTableBuffer,
                              hitShaderBindingOffset,
                              hitShaderBindingStride,
                              callableShaderBindingTableBuffer,
                              callableShaderBindingOffset,
                              callableShaderBindingStride,
                              width,
                              height,
                              depth);
}

VkResult vkCreateRayTracingPipelinesNV(VkPipelineCache pipelineCache,
                                       uint32_t createInfoCount,
                                       const VkRayTracingPipelineCreateInfoNV* pCreateInfos,
                                       const VkAllocationCallbacks* pAllocator,
                                       VkPipeline* pPipelines)
{
    return ::vkCreateRayTracingPipelinesNV(device_,
                                           pipelineCache,
                                           createInfoCount,
                                           pCreateInfos,
                                           pAllocator,
                                           pPipelines);
}

VkResult vkGetRayTracingShaderGroupHandlesKHR(VkPipeline pipeline,
                                              uint32_t firstGroup,
                                              uint32_t groupCount,
                                              size_t dataSize,
                                              void* pData)
{
    return ::vkGetRayTracingShaderGroupHandlesKHR(device_, pipeline, firstGroup, groupCount, dataSize, pData);
}

VkResult vkGetRayTracingShaderGroupHandlesNV(VkPipeline pipeline,
                                             uint32_t firstGroup,
                                             uint32_t groupCount,
                                             size_t dataSize,
                                             void* pData)
{
    return ::vkGetRayTracingShaderGroupHandlesNV(device_, pipeline, firstGroup, groupCount, dataSize, pData);
}

VkResult vkGetAccelerationStructureHandleNV(VkAccelerationStructureNV accelerationStructure,
                                            size_t dataSize,
                                            void* pData)
{
    return ::vkGetAccelerationStructureHandleNV(device_, accelerationStructure, dataSize, pData);
}

void vkCmdWriteAccelerationStructuresPropertiesNV(VkCommandBuffer commandBuffer,
                                                  uint32_t accelerationStructureCount,
                                                  const VkAccelerationStructureNV* pAccelerationStructures,
                                                  VkQueryType queryType,
                                                  VkQueryPool queryPool,
                                                  uint32_t firstQuery)
{
    return ::vkCmdWriteAccelerationStructuresPropertiesNV(commandBuffer,
                                                          accelerationStructureCount,
                                                          pAccelerationStructures,
                                                          queryType,
                                                          queryPool,
                                                          firstQuery);
}

VkResult vkCompileDeferredNV(VkPipeline pipeline, uint32_t shader)
{
    return ::vkCompileDeferredNV(device_, pipeline, shader);
}

VkResult vkGetMemoryHostPointerPropertiesEXT(VkExternalMemoryHandleTypeFlagBits handleType,
                                             const void* pHostPointer,
                                             VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties)
{
    return ::vkGetMemoryHostPointerPropertiesEXT(device_, handleType, pHostPointer, pMemoryHostPointerProperties);
}

void vkCmdWriteBufferMarkerAMD(VkCommandBuffer commandBuffer,
                               VkPipelineStageFlagBits pipelineStage,
                               VkBuffer dstBuffer,
                               VkDeviceSize dstOffset,
                               uint32_t marker)
{
    return ::vkCmdWriteBufferMarkerAMD(commandBuffer, pipelineStage, dstBuffer, dstOffset, marker);
}

VkResult vkGetPhysicalDeviceCalibrateableTimeDomainsEXT(VkPhysicalDevice physicalDevice,
                                                        uint32_t* pTimeDomainCount,
                                                        VkTimeDomainEXT* pTimeDomains)
{
    return ::vkGetPhysicalDeviceCalibrateableTimeDomainsEXT(physicalDevice, pTimeDomainCount, pTimeDomains);
}

VkResult vkGetCalibratedTimestampsEXT(uint32_t timestampCount,
                                      const VkCalibratedTimestampInfoEXT* pTimestampInfos,
                                      uint64_t* pTimestamps,
                                      uint64_t* pMaxDeviation)
{
    return ::vkGetCalibratedTimestampsEXT(device_, timestampCount, pTimestampInfos, pTimestamps, pMaxDeviation);
}

void vkCmdDrawMeshTasksNV(VkCommandBuffer commandBuffer, uint32_t taskCount, uint32_t firstTask)
{
    return ::vkCmdDrawMeshTasksNV(commandBuffer, taskCount, firstTask);
}

void vkCmdDrawMeshTasksIndirectNV(VkCommandBuffer commandBuffer,
                                  VkBuffer buffer,
                                  VkDeviceSize offset,
                                  uint32_t drawCount,
                                  uint32_t stride)
{
    return ::vkCmdDrawMeshTasksIndirectNV(commandBuffer, buffer, offset, drawCount, stride);
}

void vkCmdDrawMeshTasksIndirectCountNV(VkCommandBuffer commandBuffer,
                                       VkBuffer buffer,
                                       VkDeviceSize offset,
                                       VkBuffer countBuffer,
                                       VkDeviceSize countBufferOffset,
                                       uint32_t maxDrawCount,
                                       uint32_t stride)
{
    return ::vkCmdDrawMeshTasksIndirectCountNV(commandBuffer,
                                               buffer,
                                               offset,
                                               countBuffer,
                                               countBufferOffset,
                                               maxDrawCount,
                                               stride);
}

void vkCmdSetExclusiveScissorNV(VkCommandBuffer commandBuffer,
                                uint32_t firstExclusiveScissor,
                                uint32_t exclusiveScissorCount,
                                const VkRect2D* pExclusiveScissors)
{
    return ::vkCmdSetExclusiveScissorNV(commandBuffer,
                                        firstExclusiveScissor,
                                        exclusiveScissorCount,
                                        pExclusiveScissors);
}

void vkCmdSetCheckpointNV(VkCommandBuffer commandBuffer, const void* pCheckpointMarker)
{
    return ::vkCmdSetCheckpointNV(commandBuffer, pCheckpointMarker);
}

void vkGetQueueCheckpointDataNV(VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointDataNV* pCheckpointData)
{
    return ::vkGetQueueCheckpointDataNV(queue, pCheckpointDataCount, pCheckpointData);
}

VkResult vkInitializePerformanceApiINTEL(const VkInitializePerformanceApiInfoINTEL* pInitializeInfo)
{
    return ::vkInitializePerformanceApiINTEL(device_, pInitializeInfo);
}

void vkUninitializePerformanceApiINTEL()
{
    return ::vkUninitializePerformanceApiINTEL(device_);
}

VkResult vkCmdSetPerformanceMarkerINTEL(VkCommandBuffer commandBuffer, const VkPerformanceMarkerInfoINTEL* pMarkerInfo)
{
    return ::vkCmdSetPerformanceMarkerINTEL(commandBuffer, pMarkerInfo);
}

VkResult vkCmdSetPerformanceStreamMarkerINTEL(VkCommandBuffer commandBuffer,
                                              const VkPerformanceStreamMarkerInfoINTEL* pMarkerInfo)
{
    return ::vkCmdSetPerformanceStreamMarkerINTEL(commandBuffer, pMarkerInfo);
}

VkResult vkCmdSetPerformanceOverrideINTEL(VkCommandBuffer commandBuffer,
                                          const VkPerformanceOverrideInfoINTEL* pOverrideInfo)
{
    return ::vkCmdSetPerformanceOverrideINTEL(commandBuffer, pOverrideInfo);
}

VkResult vkAcquirePerformanceConfigurationINTEL(const VkPerformanceConfigurationAcquireInfoINTEL* pAcquireInfo,
                                                VkPerformanceConfigurationINTEL* pConfiguration)
{
    return ::vkAcquirePerformanceConfigurationINTEL(device_, pAcquireInfo, pConfiguration);
}

VkResult vkReleasePerformanceConfigurationINTEL(VkPerformanceConfigurationINTEL configuration)
{
    return ::vkReleasePerformanceConfigurationINTEL(device_, configuration);
}

VkResult vkQueueSetPerformanceConfigurationINTEL(VkQueue queue, VkPerformanceConfigurationINTEL configuration)
{
    return ::vkQueueSetPerformanceConfigurationINTEL(queue, configuration);
}

VkResult vkGetPerformanceParameterINTEL(VkPerformanceParameterTypeINTEL parameter, VkPerformanceValueINTEL* pValue)
{
    return ::vkGetPerformanceParameterINTEL(device_, parameter, pValue);
}

void vkSetLocalDimmingAMD(VkSwapchainKHR swapChain, VkBool32 localDimmingEnable)
{
    return ::vkSetLocalDimmingAMD(device_, swapChain, localDimmingEnable);
}

VkDeviceAddress vkGetBufferDeviceAddressEXT(const VkBufferDeviceAddressInfo* pInfo)
{
    return ::vkGetBufferDeviceAddressEXT(device_, pInfo);
}

VkResult vkGetPhysicalDeviceToolPropertiesEXT(VkPhysicalDevice physicalDevice,
                                              uint32_t* pToolCount,
                                              VkPhysicalDeviceToolProperties* pToolProperties)
{
    return ::vkGetPhysicalDeviceToolPropertiesEXT(physicalDevice, pToolCount, pToolProperties);
}

VkResult vkGetPhysicalDeviceCooperativeMatrixPropertiesNV(VkPhysicalDevice physicalDevice,
                                                          uint32_t* pPropertyCount,
                                                          VkCooperativeMatrixPropertiesNV* pProperties)
{
    return ::vkGetPhysicalDeviceCooperativeMatrixPropertiesNV(physicalDevice, pPropertyCount, pProperties);
}

VkResult vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(VkPhysicalDevice physicalDevice,
                                                                           uint32_t* pCombinationCount,
                                                                           VkFramebufferMixedSamplesCombinationNV* pCombinations)
{
    return ::vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(physicalDevice,
                                                                               pCombinationCount,
                                                                               pCombinations);
}

VkResult vkCreateHeadlessSurfaceEXT(VkInstance instance,
                                    const VkHeadlessSurfaceCreateInfoEXT* pCreateInfo,
                                    const VkAllocationCallbacks* pAllocator,
                                    VkSurfaceKHR* pSurface)
{
    return ::vkCreateHeadlessSurfaceEXT(instance, pCreateInfo, pAllocator, pSurface);
}

void vkCmdSetLineStippleEXT(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern)
{
    return ::vkCmdSetLineStippleEXT(commandBuffer, lineStippleFactor, lineStipplePattern);
}

void vkResetQueryPoolEXT(VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount)
{
    return ::vkResetQueryPoolEXT(device_, queryPool, firstQuery, queryCount);
}

void vkCmdSetCullModeEXT(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode)
{
    return ::vkCmdSetCullModeEXT(commandBuffer, cullMode);
}

void vkCmdSetFrontFaceEXT(VkCommandBuffer commandBuffer, VkFrontFace frontFace)
{
    return ::vkCmdSetFrontFaceEXT(commandBuffer, frontFace);
}

void vkCmdSetPrimitiveTopologyEXT(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology)
{
    return ::vkCmdSetPrimitiveTopologyEXT(commandBuffer, primitiveTopology);
}

void vkCmdSetViewportWithCountEXT(VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports)
{
    return ::vkCmdSetViewportWithCountEXT(commandBuffer, viewportCount, pViewports);
}

void vkCmdSetScissorWithCountEXT(VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors)
{
    return ::vkCmdSetScissorWithCountEXT(commandBuffer, scissorCount, pScissors);
}

void vkCmdBindVertexBuffers2EXT(VkCommandBuffer commandBuffer,
                                uint32_t firstBinding,
                                uint32_t bindingCount,
                                const VkBuffer* pBuffers,
                                const VkDeviceSize* pOffsets,
                                const VkDeviceSize* pSizes,
                                const VkDeviceSize* pStrides)
{
    return ::vkCmdBindVertexBuffers2EXT(commandBuffer,
                                        firstBinding,
                                        bindingCount,
                                        pBuffers,
                                        pOffsets,
                                        pSizes,
                                        pStrides);
}

void vkCmdSetDepthTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable)
{
    return ::vkCmdSetDepthTestEnableEXT(commandBuffer, depthTestEnable);
}

void vkCmdSetDepthWriteEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable)
{
    return ::vkCmdSetDepthWriteEnableEXT(commandBuffer, depthWriteEnable);
}

void vkCmdSetDepthCompareOpEXT(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp)
{
    return ::vkCmdSetDepthCompareOpEXT(commandBuffer, depthCompareOp);
}

void vkCmdSetDepthBoundsTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable)
{
    return ::vkCmdSetDepthBoundsTestEnableEXT(commandBuffer, depthBoundsTestEnable);
}

void vkCmdSetStencilTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable)
{
    return ::vkCmdSetStencilTestEnableEXT(commandBuffer, stencilTestEnable);
}

void vkCmdSetStencilOpEXT(VkCommandBuffer commandBuffer,
                          VkStencilFaceFlags faceMask,
                          VkStencilOp failOp,
                          VkStencilOp passOp,
                          VkStencilOp depthFailOp,
                          VkCompareOp compareOp)
{
    return ::vkCmdSetStencilOpEXT(commandBuffer, faceMask, failOp, passOp, depthFailOp, compareOp);
}

void vkGetGeneratedCommandsMemoryRequirementsNV(const VkGeneratedCommandsMemoryRequirementsInfoNV* pInfo,
                                                VkMemoryRequirements2* pMemoryRequirements)
{
    return ::vkGetGeneratedCommandsMemoryRequirementsNV(device_, pInfo, pMemoryRequirements);
}

void vkCmdPreprocessGeneratedCommandsNV(VkCommandBuffer commandBuffer,
                                        const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo)
{
    return ::vkCmdPreprocessGeneratedCommandsNV(commandBuffer, pGeneratedCommandsInfo);
}

void vkCmdExecuteGeneratedCommandsNV(VkCommandBuffer commandBuffer,
                                     VkBool32 isPreprocessed,
                                     const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo)
{
    return ::vkCmdExecuteGeneratedCommandsNV(commandBuffer, isPreprocessed, pGeneratedCommandsInfo);
}

void vkCmdBindPipelineShaderGroupNV(VkCommandBuffer commandBuffer,
                                    VkPipelineBindPoint pipelineBindPoint,
                                    VkPipeline pipeline,
                                    uint32_t groupIndex)
{
    return ::vkCmdBindPipelineShaderGroupNV(commandBuffer, pipelineBindPoint, pipeline, groupIndex);
}

VkResult vkCreateIndirectCommandsLayoutNV(const VkIndirectCommandsLayoutCreateInfoNV* pCreateInfo,
                                          const VkAllocationCallbacks* pAllocator,
                                          VkIndirectCommandsLayoutNV* pIndirectCommandsLayout)
{
    return ::vkCreateIndirectCommandsLayoutNV(device_, pCreateInfo, pAllocator, pIndirectCommandsLayout);
}

void vkDestroyIndirectCommandsLayoutNV(VkIndirectCommandsLayoutNV indirectCommandsLayout,
                                       const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyIndirectCommandsLayoutNV(device_, indirectCommandsLayout, pAllocator);
}

VkResult vkAcquireDrmDisplayEXT(VkPhysicalDevice physicalDevice, int32_t drmFd, VkDisplayKHR display)
{
    return ::vkAcquireDrmDisplayEXT(physicalDevice, drmFd, display);
}

VkResult vkGetDrmDisplayEXT(VkPhysicalDevice physicalDevice,
                            int32_t drmFd,
                            uint32_t connectorId,
                            VkDisplayKHR* display)
{
    return ::vkGetDrmDisplayEXT(physicalDevice, drmFd, connectorId, display);
}

VkResult vkCreatePrivateDataSlotEXT(const VkPrivateDataSlotCreateInfo* pCreateInfo,
                                    const VkAllocationCallbacks* pAllocator,
                                    VkPrivateDataSlot* pPrivateDataSlot)
{
    return ::vkCreatePrivateDataSlotEXT(device_, pCreateInfo, pAllocator, pPrivateDataSlot);
}

void vkDestroyPrivateDataSlotEXT(VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyPrivateDataSlotEXT(device_, privateDataSlot, pAllocator);
}

VkResult vkSetPrivateDataEXT(VkObjectType objectType,
                             uint64_t objectHandle,
                             VkPrivateDataSlot privateDataSlot,
                             uint64_t data)
{
    return ::vkSetPrivateDataEXT(device_, objectType, objectHandle, privateDataSlot, data);
}

void vkGetPrivateDataEXT(VkObjectType objectType,
                         uint64_t objectHandle,
                         VkPrivateDataSlot privateDataSlot,
                         uint64_t* pData)
{
    return ::vkGetPrivateDataEXT(device_, objectType, objectHandle, privateDataSlot, pData);
}

void vkCmdSetFragmentShadingRateEnumNV(VkCommandBuffer commandBuffer,
                                       VkFragmentShadingRateNV shadingRate,
                                       const VkFragmentShadingRateCombinerOpKHR combinerOps[2])
{
    return ::vkCmdSetFragmentShadingRateEnumNV(commandBuffer, shadingRate, combinerOps);
}

VkResult vkAcquireWinrtDisplayNV(VkPhysicalDevice physicalDevice, VkDisplayKHR display)
{
    return ::vkAcquireWinrtDisplayNV(physicalDevice, display);
}

VkResult vkGetWinrtDisplayNV(VkPhysicalDevice physicalDevice, uint32_t deviceRelativeId, VkDisplayKHR* pDisplay)
{
    return ::vkGetWinrtDisplayNV(physicalDevice, deviceRelativeId, pDisplay);
}

void vkCmdSetVertexInputEXT(VkCommandBuffer commandBuffer,
                            uint32_t vertexBindingDescriptionCount,
                            const VkVertexInputBindingDescription2EXT* pVertexBindingDescriptions,
                            uint32_t vertexAttributeDescriptionCount,
                            const VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions)
{
    return ::vkCmdSetVertexInputEXT(commandBuffer,
                                    vertexBindingDescriptionCount,
                                    pVertexBindingDescriptions,
                                    vertexAttributeDescriptionCount,
                                    pVertexAttributeDescriptions);
}

VkResult vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI(VkRenderPass renderpass, VkExtent2D* pMaxWorkgroupSize)
{
    return ::vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI(device_, renderpass, pMaxWorkgroupSize);
}

void vkCmdSubpassShadingHUAWEI(VkCommandBuffer commandBuffer)
{
    return ::vkCmdSubpassShadingHUAWEI(commandBuffer);
}

void vkCmdBindInvocationMaskHUAWEI(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout)
{
    return ::vkCmdBindInvocationMaskHUAWEI(commandBuffer, imageView, imageLayout);
}

VkResult vkGetMemoryRemoteAddressNV(const VkMemoryGetRemoteAddressInfoNV* pMemoryGetRemoteAddressInfo,
                                    VkRemoteAddressNV* pAddress)
{
    return ::vkGetMemoryRemoteAddressNV(device_, pMemoryGetRemoteAddressInfo, pAddress);
}

void vkCmdSetPatchControlPointsEXT(VkCommandBuffer commandBuffer, uint32_t patchControlPoints)
{
    return ::vkCmdSetPatchControlPointsEXT(commandBuffer, patchControlPoints);
}

void vkCmdSetRasterizerDiscardEnableEXT(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable)
{
    return ::vkCmdSetRasterizerDiscardEnableEXT(commandBuffer, rasterizerDiscardEnable);
}

void vkCmdSetDepthBiasEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable)
{
    return ::vkCmdSetDepthBiasEnableEXT(commandBuffer, depthBiasEnable);
}

void vkCmdSetLogicOpEXT(VkCommandBuffer commandBuffer, VkLogicOp logicOp)
{
    return ::vkCmdSetLogicOpEXT(commandBuffer, logicOp);
}

void vkCmdSetPrimitiveRestartEnableEXT(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable)
{
    return ::vkCmdSetPrimitiveRestartEnableEXT(commandBuffer, primitiveRestartEnable);
}

void vkCmdSetColorWriteEnableEXT(VkCommandBuffer commandBuffer,
                                 uint32_t attachmentCount,
                                 const VkBool32* pColorWriteEnables)
{
    return ::vkCmdSetColorWriteEnableEXT(commandBuffer, attachmentCount, pColorWriteEnables);
}

void vkCmdDrawMultiEXT(VkCommandBuffer commandBuffer,
                       uint32_t drawCount,
                       const VkMultiDrawInfoEXT* pVertexInfo,
                       uint32_t instanceCount,
                       uint32_t firstInstance,
                       uint32_t stride)
{
    return ::vkCmdDrawMultiEXT(commandBuffer, drawCount, pVertexInfo, instanceCount, firstInstance, stride);
}

void vkCmdDrawMultiIndexedEXT(VkCommandBuffer commandBuffer,
                              uint32_t drawCount,
                              const VkMultiDrawIndexedInfoEXT* pIndexInfo,
                              uint32_t instanceCount,
                              uint32_t firstInstance,
                              uint32_t stride,
                              const int32_t* pVertexOffset)
{
    return ::vkCmdDrawMultiIndexedEXT(commandBuffer,
                                      drawCount,
                                      pIndexInfo,
                                      instanceCount,
                                      firstInstance,
                                      stride,
                                      pVertexOffset);
}

void vkSetDeviceMemoryPriorityEXT(VkDeviceMemory memory, float priority)
{
    return ::vkSetDeviceMemoryPriorityEXT(device_, memory, priority);
}

void vkGetDescriptorSetLayoutHostMappingInfoVALVE(const VkDescriptorSetBindingReferenceVALVE* pBindingReference,
                                                  VkDescriptorSetLayoutHostMappingInfoVALVE* pHostMapping)
{
    return ::vkGetDescriptorSetLayoutHostMappingInfoVALVE(device_, pBindingReference, pHostMapping);
}

void vkGetDescriptorSetHostMappingVALVE(VkDescriptorSet descriptorSet, void** ppData)
{
    return ::vkGetDescriptorSetHostMappingVALVE(device_, descriptorSet, ppData);
}

VkResult vkCreateAccelerationStructureKHR(const VkAccelerationStructureCreateInfoKHR* pCreateInfo,
                                          const VkAllocationCallbacks* pAllocator,
                                          VkAccelerationStructureKHR* pAccelerationStructure)
{
    return ::vkCreateAccelerationStructureKHR(device_, pCreateInfo, pAllocator, pAccelerationStructure);
}

void vkDestroyAccelerationStructureKHR(VkAccelerationStructureKHR accelerationStructure,
                                       const VkAllocationCallbacks* pAllocator)
{
    return ::vkDestroyAccelerationStructureKHR(device_, accelerationStructure, pAllocator);
}

void vkCmdBuildAccelerationStructuresKHR(VkCommandBuffer commandBuffer,
                                         uint32_t infoCount,
                                         const VkAccelerationStructureBuildGeometryInfoKHR* pInfos,
                                         const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos)
{
    return ::vkCmdBuildAccelerationStructuresKHR(commandBuffer, infoCount, pInfos, ppBuildRangeInfos);
}

void vkCmdBuildAccelerationStructuresIndirectKHR(VkCommandBuffer commandBuffer,
                                                 uint32_t infoCount,
                                                 const VkAccelerationStructureBuildGeometryInfoKHR* pInfos,
                                                 const VkDeviceAddress* pIndirectDeviceAddresses,
                                                 const uint32_t* pIndirectStrides,
                                                 const uint32_t* const* ppMaxPrimitiveCounts)
{
    return ::vkCmdBuildAccelerationStructuresIndirectKHR(commandBuffer,
                                                         infoCount,
                                                         pInfos,
                                                         pIndirectDeviceAddresses,
                                                         pIndirectStrides,
                                                         ppMaxPrimitiveCounts);
}

VkResult vkBuildAccelerationStructuresKHR(VkDeferredOperationKHR deferredOperation,
                                          uint32_t infoCount,
                                          const VkAccelerationStructureBuildGeometryInfoKHR* pInfos,
                                          const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos)
{
    return ::vkBuildAccelerationStructuresKHR(device_, deferredOperation, infoCount, pInfos, ppBuildRangeInfos);
}

VkResult vkCopyAccelerationStructureKHR(VkDeferredOperationKHR deferredOperation,
                                        const VkCopyAccelerationStructureInfoKHR* pInfo)
{
    return ::vkCopyAccelerationStructureKHR(device_, deferredOperation, pInfo);
}

VkResult vkCopyAccelerationStructureToMemoryKHR(VkDeferredOperationKHR deferredOperation,
                                                const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo)
{
    return ::vkCopyAccelerationStructureToMemoryKHR(device_, deferredOperation, pInfo);
}

VkResult vkCopyMemoryToAccelerationStructureKHR(VkDeferredOperationKHR deferredOperation,
                                                const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo)
{
    return ::vkCopyMemoryToAccelerationStructureKHR(device_, deferredOperation, pInfo);
}

VkResult vkWriteAccelerationStructuresPropertiesKHR(uint32_t accelerationStructureCount,
                                                    const VkAccelerationStructureKHR* pAccelerationStructures,
                                                    VkQueryType queryType,
                                                    size_t dataSize,
                                                    void* pData,
                                                    size_t stride)
{
    return ::vkWriteAccelerationStructuresPropertiesKHR(device_,
                                                        accelerationStructureCount,
                                                        pAccelerationStructures,
                                                        queryType,
                                                        dataSize,
                                                        pData,
                                                        stride);
}

void vkCmdCopyAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureInfoKHR* pInfo)
{
    return ::vkCmdCopyAccelerationStructureKHR(commandBuffer, pInfo);
}

void vkCmdCopyAccelerationStructureToMemoryKHR(VkCommandBuffer commandBuffer,
                                               const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo)
{
    return ::vkCmdCopyAccelerationStructureToMemoryKHR(commandBuffer, pInfo);
}

void vkCmdCopyMemoryToAccelerationStructureKHR(VkCommandBuffer commandBuffer,
                                               const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo)
{
    return ::vkCmdCopyMemoryToAccelerationStructureKHR(commandBuffer, pInfo);
}

VkDeviceAddress vkGetAccelerationStructureDeviceAddressKHR(const VkAccelerationStructureDeviceAddressInfoKHR* pInfo)
{
    return ::vkGetAccelerationStructureDeviceAddressKHR(device_, pInfo);
}

void vkCmdWriteAccelerationStructuresPropertiesKHR(VkCommandBuffer commandBuffer,
                                                   uint32_t accelerationStructureCount,
                                                   const VkAccelerationStructureKHR* pAccelerationStructures,
                                                   VkQueryType queryType,
                                                   VkQueryPool queryPool,
                                                   uint32_t firstQuery)
{
    return ::vkCmdWriteAccelerationStructuresPropertiesKHR(commandBuffer,
                                                           accelerationStructureCount,
                                                           pAccelerationStructures,
                                                           queryType,
                                                           queryPool,
                                                           firstQuery);
}

void vkGetDeviceAccelerationStructureCompatibilityKHR(const VkAccelerationStructureVersionInfoKHR* pVersionInfo,
                                                      VkAccelerationStructureCompatibilityKHR* pCompatibility)
{
    return ::vkGetDeviceAccelerationStructureCompatibilityKHR(device_, pVersionInfo, pCompatibility);
}

void vkGetAccelerationStructureBuildSizesKHR(VkAccelerationStructureBuildTypeKHR buildType,
                                             const VkAccelerationStructureBuildGeometryInfoKHR* pBuildInfo,
                                             const uint32_t* pMaxPrimitiveCounts,
                                             VkAccelerationStructureBuildSizesInfoKHR* pSizeInfo)
{
    return ::vkGetAccelerationStructureBuildSizesKHR(device_, buildType, pBuildInfo, pMaxPrimitiveCounts, pSizeInfo);
}

void vkCmdTraceRaysKHR(VkCommandBuffer commandBuffer,
                       const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable,
                       const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable,
                       const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable,
                       const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable,
                       uint32_t width,
                       uint32_t height,
                       uint32_t depth)
{
    return ::vkCmdTraceRaysKHR(commandBuffer,
                               pRaygenShaderBindingTable,
                               pMissShaderBindingTable,
                               pHitShaderBindingTable,
                               pCallableShaderBindingTable,
                               width,
                               height,
                               depth);
}

VkResult vkCreateRayTracingPipelinesKHR(VkDeferredOperationKHR deferredOperation,
                                        VkPipelineCache pipelineCache,
                                        uint32_t createInfoCount,
                                        const VkRayTracingPipelineCreateInfoKHR* pCreateInfos,
                                        const VkAllocationCallbacks* pAllocator,
                                        VkPipeline* pPipelines)
{
    return ::vkCreateRayTracingPipelinesKHR(device_,
                                            deferredOperation,
                                            pipelineCache,
                                            createInfoCount,
                                            pCreateInfos,
                                            pAllocator,
                                            pPipelines);
}

VkResult vkGetRayTracingCaptureReplayShaderGroupHandlesKHR(VkPipeline pipeline,
                                                           uint32_t firstGroup,
                                                           uint32_t groupCount,
                                                           size_t dataSize,
                                                           void* pData)
{
    return ::vkGetRayTracingCaptureReplayShaderGroupHandlesKHR(device_,
                                                               pipeline,
                                                               firstGroup,
                                                               groupCount,
                                                               dataSize,
                                                               pData);
}

void vkCmdTraceRaysIndirectKHR(VkCommandBuffer commandBuffer,
                               const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable,
                               const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable,
                               const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable,
                               const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable,
                               VkDeviceAddress indirectDeviceAddress)
{
    return ::vkCmdTraceRaysIndirectKHR(commandBuffer,
                                       pRaygenShaderBindingTable,
                                       pMissShaderBindingTable,
                                       pHitShaderBindingTable,
                                       pCallableShaderBindingTable,
                                       indirectDeviceAddress);
}

VkDeviceSize vkGetRayTracingShaderGroupStackSizeKHR(VkPipeline pipeline,
                                                    uint32_t group,
                                                    VkShaderGroupShaderKHR groupShader)
{
    return ::vkGetRayTracingShaderGroupStackSizeKHR(device_, pipeline, group, groupShader);
}

void vkCmdSetRayTracingPipelineStackSizeKHR(VkCommandBuffer commandBuffer, uint32_t pipelineStackSize)
{
    return ::vkCmdSetRayTracingPipelineStackSizeKHR(commandBuffer, pipelineStackSize);
}


// clang-format on

} // namespace plex::vkapi
