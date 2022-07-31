#ifndef PLEX_GRAPHICS_VULKAN_API_H
#define PLEX_GRAPHICS_VULKAN_API_H

#include <vulkan/vulkan_core.h>

namespace plex::vkapi
{

// clang-format off

void InitVulkanApi(VkDevice device);

VkResult vkCreateInstance(const VkInstanceCreateInfo* pCreateInfo,
                          const VkAllocationCallbacks* pAllocator,
                          VkInstance* pInstance);

void vkDestroyInstance(VkInstance instance, const VkAllocationCallbacks* pAllocator);

VkResult vkEnumeratePhysicalDevices(VkInstance instance,
                                    uint32_t* pPhysicalDeviceCount,
                                    VkPhysicalDevice* pPhysicalDevices);

void vkGetPhysicalDeviceFeatures(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures);

void vkGetPhysicalDeviceFormatProperties(VkPhysicalDevice physicalDevice,
                                         VkFormat format,
                                         VkFormatProperties* pFormatProperties);

VkResult vkGetPhysicalDeviceImageFormatProperties(VkPhysicalDevice physicalDevice,
                                                  VkFormat format,
                                                  VkImageType type,
                                                  VkImageTiling tiling,
                                                  VkImageUsageFlags usage,
                                                  VkImageCreateFlags flags,
                                                  VkImageFormatProperties* pImageFormatProperties);

void vkGetPhysicalDeviceProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties);

void vkGetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice physicalDevice,
                                              uint32_t* pQueueFamilyPropertyCount,
                                              VkQueueFamilyProperties* pQueueFamilyProperties);

void vkGetPhysicalDeviceMemoryProperties(VkPhysicalDevice physicalDevice,
                                         VkPhysicalDeviceMemoryProperties* pMemoryProperties);

PFN_vkVoidFunction vkGetInstanceProcAddr(VkInstance instance, const char* pName);

PFN_vkVoidFunction vkGetDeviceProcAddr(const char* pName);

VkResult vkCreateDevice(VkPhysicalDevice physicalDevice,
                        const VkDeviceCreateInfo* pCreateInfo,
                        const VkAllocationCallbacks* pAllocator,
                        VkDevice* pDevice);

void vkDestroyDevice(const VkAllocationCallbacks* pAllocator);

VkResult vkEnumerateInstanceExtensionProperties(const char* pLayerName,
                                                uint32_t* pPropertyCount,
                                                VkExtensionProperties* pProperties);

VkResult vkEnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice,
                                              const char* pLayerName,
                                              uint32_t* pPropertyCount,
                                              VkExtensionProperties* pProperties);

VkResult vkEnumerateInstanceLayerProperties(uint32_t* pPropertyCount, VkLayerProperties* pProperties);

VkResult vkEnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice,
                                          uint32_t* pPropertyCount,
                                          VkLayerProperties* pProperties);

void vkGetDeviceQueue(uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue);

VkResult vkQueueSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence);

VkResult vkQueueWaitIdle(VkQueue queue);

VkResult vkDeviceWaitIdle();

VkResult vkAllocateMemory(const VkMemoryAllocateInfo* pAllocateInfo,
                          const VkAllocationCallbacks* pAllocator,
                          VkDeviceMemory* pMemory);

void vkFreeMemory(VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator);

VkResult vkMapMemory(VkDeviceMemory memory,
                     VkDeviceSize offset,
                     VkDeviceSize size,
                     VkMemoryMapFlags flags,
                     void** ppData);

void vkUnmapMemory(VkDeviceMemory memory);

VkResult vkFlushMappedMemoryRanges(uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges);

VkResult vkInvalidateMappedMemoryRanges(uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges);

void vkGetDeviceMemoryCommitment(VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes);

VkResult vkBindBufferMemory(VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset);

VkResult vkBindImageMemory(VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset);

void vkGetBufferMemoryRequirements(VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements);

void vkGetImageMemoryRequirements(VkImage image, VkMemoryRequirements* pMemoryRequirements);

void vkGetImageSparseMemoryRequirements(VkImage image,
                                        uint32_t* pSparseMemoryRequirementCount,
                                        VkSparseImageMemoryRequirements* pSparseMemoryRequirements);

void vkGetPhysicalDeviceSparseImageFormatProperties(VkPhysicalDevice physicalDevice,
                                                    VkFormat format,
                                                    VkImageType type,
                                                    VkSampleCountFlagBits samples,
                                                    VkImageUsageFlags usage,
                                                    VkImageTiling tiling,
                                                    uint32_t* pPropertyCount,
                                                    VkSparseImageFormatProperties* pProperties);

VkResult vkQueueBindSparse(VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence);

VkResult vkCreateFence(const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence);

void vkDestroyFence(VkFence fence, const VkAllocationCallbacks* pAllocator);

VkResult vkResetFences(uint32_t fenceCount, const VkFence* pFences);

VkResult vkGetFenceStatus(VkFence fence);

VkResult vkWaitForFences(uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout);

VkResult vkCreateSemaphore(const VkSemaphoreCreateInfo* pCreateInfo,
                           const VkAllocationCallbacks* pAllocator,
                           VkSemaphore* pSemaphore);

void vkDestroySemaphore(VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator);

VkResult vkCreateEvent(const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent);

void vkDestroyEvent(VkEvent event, const VkAllocationCallbacks* pAllocator);

VkResult vkGetEventStatus(VkEvent event);

VkResult vkSetEvent(VkEvent event);

VkResult vkResetEvent(VkEvent event);

VkResult vkCreateQueryPool(const VkQueryPoolCreateInfo* pCreateInfo,
                           const VkAllocationCallbacks* pAllocator,
                           VkQueryPool* pQueryPool);

void vkDestroyQueryPool(VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator);

VkResult vkGetQueryPoolResults(VkQueryPool queryPool,
                               uint32_t firstQuery,
                               uint32_t queryCount,
                               size_t dataSize,
                               void* pData,
                               VkDeviceSize stride,
                               VkQueryResultFlags flags);

VkResult vkCreateBuffer(const VkBufferCreateInfo* pCreateInfo,
                        const VkAllocationCallbacks* pAllocator,
                        VkBuffer* pBuffer);

void vkDestroyBuffer(VkBuffer buffer, const VkAllocationCallbacks* pAllocator);

VkResult vkCreateBufferView(const VkBufferViewCreateInfo* pCreateInfo,
                            const VkAllocationCallbacks* pAllocator,
                            VkBufferView* pView);

void vkDestroyBufferView(VkBufferView bufferView, const VkAllocationCallbacks* pAllocator);

VkResult vkCreateImage(const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage);

void vkDestroyImage(VkImage image, const VkAllocationCallbacks* pAllocator);

void vkGetImageSubresourceLayout(VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout);

VkResult vkCreateImageView(const VkImageViewCreateInfo* pCreateInfo,
                           const VkAllocationCallbacks* pAllocator,
                           VkImageView* pView);

void vkDestroyImageView(VkImageView imageView, const VkAllocationCallbacks* pAllocator);

VkResult vkCreateShaderModule(const VkShaderModuleCreateInfo* pCreateInfo,
                              const VkAllocationCallbacks* pAllocator,
                              VkShaderModule* pShaderModule);

void vkDestroyShaderModule(VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator);

VkResult vkCreatePipelineCache(const VkPipelineCacheCreateInfo* pCreateInfo,
                               const VkAllocationCallbacks* pAllocator,
                               VkPipelineCache* pPipelineCache);

void vkDestroyPipelineCache(VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator);

VkResult vkGetPipelineCacheData(VkPipelineCache pipelineCache, size_t* pDataSize, void* pData);

VkResult vkMergePipelineCaches(VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches);

VkResult vkCreateGraphicsPipelines(VkPipelineCache pipelineCache,
                                   uint32_t createInfoCount,
                                   const VkGraphicsPipelineCreateInfo* pCreateInfos,
                                   const VkAllocationCallbacks* pAllocator,
                                   VkPipeline* pPipelines);

VkResult vkCreateComputePipelines(VkPipelineCache pipelineCache,
                                  uint32_t createInfoCount,
                                  const VkComputePipelineCreateInfo* pCreateInfos,
                                  const VkAllocationCallbacks* pAllocator,
                                  VkPipeline* pPipelines);

void vkDestroyPipeline(VkPipeline pipeline, const VkAllocationCallbacks* pAllocator);

VkResult vkCreatePipelineLayout(const VkPipelineLayoutCreateInfo* pCreateInfo,
                                const VkAllocationCallbacks* pAllocator,
                                VkPipelineLayout* pPipelineLayout);

void vkDestroyPipelineLayout(VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator);

VkResult vkCreateSampler(const VkSamplerCreateInfo* pCreateInfo,
                         const VkAllocationCallbacks* pAllocator,
                         VkSampler* pSampler);

void vkDestroySampler(VkSampler sampler, const VkAllocationCallbacks* pAllocator);

VkResult vkCreateDescriptorSetLayout(const VkDescriptorSetLayoutCreateInfo* pCreateInfo,
                                     const VkAllocationCallbacks* pAllocator,
                                     VkDescriptorSetLayout* pSetLayout);

void vkDestroyDescriptorSetLayout(VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator);

VkResult vkCreateDescriptorPool(const VkDescriptorPoolCreateInfo* pCreateInfo,
                                const VkAllocationCallbacks* pAllocator,
                                VkDescriptorPool* pDescriptorPool);

void vkDestroyDescriptorPool(VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator);

VkResult vkResetDescriptorPool(VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags);

VkResult vkAllocateDescriptorSets(const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets);

VkResult vkFreeDescriptorSets(VkDescriptorPool descriptorPool,
                              uint32_t descriptorSetCount,
                              const VkDescriptorSet* pDescriptorSets);

void vkUpdateDescriptorSets(uint32_t descriptorWriteCount,
                            const VkWriteDescriptorSet* pDescriptorWrites,
                            uint32_t descriptorCopyCount,
                            const VkCopyDescriptorSet* pDescriptorCopies);

VkResult vkCreateFramebuffer(const VkFramebufferCreateInfo* pCreateInfo,
                             const VkAllocationCallbacks* pAllocator,
                             VkFramebuffer* pFramebuffer);

void vkDestroyFramebuffer(VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator);

VkResult vkCreateRenderPass(const VkRenderPassCreateInfo* pCreateInfo,
                            const VkAllocationCallbacks* pAllocator,
                            VkRenderPass* pRenderPass);

void vkDestroyRenderPass(VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator);

void vkGetRenderAreaGranularity(VkRenderPass renderPass, VkExtent2D* pGranularity);

VkResult vkCreateCommandPool(const VkCommandPoolCreateInfo* pCreateInfo,
                             const VkAllocationCallbacks* pAllocator,
                             VkCommandPool* pCommandPool);

void vkDestroyCommandPool(VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator);

VkResult vkResetCommandPool(VkCommandPool commandPool, VkCommandPoolResetFlags flags);

VkResult vkAllocateCommandBuffers(const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers);

void vkFreeCommandBuffers(VkCommandPool commandPool,
                          uint32_t commandBufferCount,
                          const VkCommandBuffer* pCommandBuffers);

VkResult vkBeginCommandBuffer(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo);

VkResult vkEndCommandBuffer(VkCommandBuffer commandBuffer);

VkResult vkResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags);

void vkCmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline);

void vkCmdSetViewport(VkCommandBuffer commandBuffer,
                      uint32_t firstViewport,
                      uint32_t viewportCount,
                      const VkViewport* pViewports);

void vkCmdSetScissor(VkCommandBuffer commandBuffer,
                     uint32_t firstScissor,
                     uint32_t scissorCount,
                     const VkRect2D* pScissors);

void vkCmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth);

void vkCmdSetDepthBias(VkCommandBuffer commandBuffer,
                       float depthBiasConstantFactor,
                       float depthBiasClamp,
                       float depthBiasSlopeFactor);

void vkCmdSetBlendConstants(VkCommandBuffer commandBuffer, const float blendConstants[4]);

void vkCmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds);

void vkCmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask);

void vkCmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask);

void vkCmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference);

void vkCmdBindDescriptorSets(VkCommandBuffer commandBuffer,
                             VkPipelineBindPoint pipelineBindPoint,
                             VkPipelineLayout layout,
                             uint32_t firstSet,
                             uint32_t descriptorSetCount,
                             const VkDescriptorSet* pDescriptorSets,
                             uint32_t dynamicOffsetCount,
                             const uint32_t* pDynamicOffsets);

void vkCmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType);

void vkCmdBindVertexBuffers(VkCommandBuffer commandBuffer,
                            uint32_t firstBinding,
                            uint32_t bindingCount,
                            const VkBuffer* pBuffers,
                            const VkDeviceSize* pOffsets);

void vkCmdDraw(VkCommandBuffer commandBuffer,
               uint32_t vertexCount,
               uint32_t instanceCount,
               uint32_t firstVertex,
               uint32_t firstInstance);

void vkCmdDrawIndexed(VkCommandBuffer commandBuffer,
                      uint32_t indexCount,
                      uint32_t instanceCount,
                      uint32_t firstIndex,
                      int32_t vertexOffset,
                      uint32_t firstInstance);

void vkCmdDrawIndirect(VkCommandBuffer commandBuffer,
                       VkBuffer buffer,
                       VkDeviceSize offset,
                       uint32_t drawCount,
                       uint32_t stride);

void vkCmdDrawIndexedIndirect(VkCommandBuffer commandBuffer,
                              VkBuffer buffer,
                              VkDeviceSize offset,
                              uint32_t drawCount,
                              uint32_t stride);

void vkCmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);

void vkCmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset);

void vkCmdCopyBuffer(VkCommandBuffer commandBuffer,
                     VkBuffer srcBuffer,
                     VkBuffer dstBuffer,
                     uint32_t regionCount,
                     const VkBufferCopy* pRegions);

void vkCmdCopyImage(VkCommandBuffer commandBuffer,
                    VkImage srcImage,
                    VkImageLayout srcImageLayout,
                    VkImage dstImage,
                    VkImageLayout dstImageLayout,
                    uint32_t regionCount,
                    const VkImageCopy* pRegions);

void vkCmdBlitImage(VkCommandBuffer commandBuffer,
                    VkImage srcImage,
                    VkImageLayout srcImageLayout,
                    VkImage dstImage,
                    VkImageLayout dstImageLayout,
                    uint32_t regionCount,
                    const VkImageBlit* pRegions,
                    VkFilter filter);

void vkCmdCopyBufferToImage(VkCommandBuffer commandBuffer,
                            VkBuffer srcBuffer,
                            VkImage dstImage,
                            VkImageLayout dstImageLayout,
                            uint32_t regionCount,
                            const VkBufferImageCopy* pRegions);

void vkCmdCopyImageToBuffer(VkCommandBuffer commandBuffer,
                            VkImage srcImage,
                            VkImageLayout srcImageLayout,
                            VkBuffer dstBuffer,
                            uint32_t regionCount,
                            const VkBufferImageCopy* pRegions);

void vkCmdUpdateBuffer(VkCommandBuffer commandBuffer,
                       VkBuffer dstBuffer,
                       VkDeviceSize dstOffset,
                       VkDeviceSize dataSize,
                       const void* pData);

void vkCmdFillBuffer(VkCommandBuffer commandBuffer,
                     VkBuffer dstBuffer,
                     VkDeviceSize dstOffset,
                     VkDeviceSize size,
                     uint32_t data);

void vkCmdClearColorImage(VkCommandBuffer commandBuffer,
                          VkImage image,
                          VkImageLayout imageLayout,
                          const VkClearColorValue* pColor,
                          uint32_t rangeCount,
                          const VkImageSubresourceRange* pRanges);

void vkCmdClearDepthStencilImage(VkCommandBuffer commandBuffer,
                                 VkImage image,
                                 VkImageLayout imageLayout,
                                 const VkClearDepthStencilValue* pDepthStencil,
                                 uint32_t rangeCount,
                                 const VkImageSubresourceRange* pRanges);

void vkCmdClearAttachments(VkCommandBuffer commandBuffer,
                           uint32_t attachmentCount,
                           const VkClearAttachment* pAttachments,
                           uint32_t rectCount,
                           const VkClearRect* pRects);

void vkCmdResolveImage(VkCommandBuffer commandBuffer,
                       VkImage srcImage,
                       VkImageLayout srcImageLayout,
                       VkImage dstImage,
                       VkImageLayout dstImageLayout,
                       uint32_t regionCount,
                       const VkImageResolve* pRegions);

void vkCmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask);

void vkCmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask);

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
                     const VkImageMemoryBarrier* pImageMemoryBarriers);

void vkCmdPipelineBarrier(VkCommandBuffer commandBuffer,
                          VkPipelineStageFlags srcStageMask,
                          VkPipelineStageFlags dstStageMask,
                          VkDependencyFlags dependencyFlags,
                          uint32_t memoryBarrierCount,
                          const VkMemoryBarrier* pMemoryBarriers,
                          uint32_t bufferMemoryBarrierCount,
                          const VkBufferMemoryBarrier* pBufferMemoryBarriers,
                          uint32_t imageMemoryBarrierCount,
                          const VkImageMemoryBarrier* pImageMemoryBarriers);

void vkCmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags);

void vkCmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query);

void vkCmdResetQueryPool(VkCommandBuffer commandBuffer,
                         VkQueryPool queryPool,
                         uint32_t firstQuery,
                         uint32_t queryCount);

void vkCmdWriteTimestamp(VkCommandBuffer commandBuffer,
                         VkPipelineStageFlagBits pipelineStage,
                         VkQueryPool queryPool,
                         uint32_t query);

void vkCmdCopyQueryPoolResults(VkCommandBuffer commandBuffer,
                               VkQueryPool queryPool,
                               uint32_t firstQuery,
                               uint32_t queryCount,
                               VkBuffer dstBuffer,
                               VkDeviceSize dstOffset,
                               VkDeviceSize stride,
                               VkQueryResultFlags flags);

void vkCmdPushConstants(VkCommandBuffer commandBuffer,
                        VkPipelineLayout layout,
                        VkShaderStageFlags stageFlags,
                        uint32_t offset,
                        uint32_t size,
                        const void* pValues);

void vkCmdBeginRenderPass(VkCommandBuffer commandBuffer,
                          const VkRenderPassBeginInfo* pRenderPassBegin,
                          VkSubpassContents contents);

void vkCmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents);

void vkCmdEndRenderPass(VkCommandBuffer commandBuffer);

void vkCmdExecuteCommands(VkCommandBuffer commandBuffer,
                          uint32_t commandBufferCount,
                          const VkCommandBuffer* pCommandBuffers);

VkResult vkEnumerateInstanceVersion(uint32_t* pApiVersion);

VkResult vkBindBufferMemory2(uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos);

VkResult vkBindImageMemory2(uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos);

void vkGetDeviceGroupPeerMemoryFeatures(uint32_t heapIndex,
                                        uint32_t localDeviceIndex,
                                        uint32_t remoteDeviceIndex,
                                        VkPeerMemoryFeatureFlags* pPeerMemoryFeatures);

void vkCmdSetDeviceMask(VkCommandBuffer commandBuffer, uint32_t deviceMask);

void vkCmdDispatchBase(VkCommandBuffer commandBuffer,
                       uint32_t baseGroupX,
                       uint32_t baseGroupY,
                       uint32_t baseGroupZ,
                       uint32_t groupCountX,
                       uint32_t groupCountY,
                       uint32_t groupCountZ);

VkResult vkEnumeratePhysicalDeviceGroups(VkInstance instance,
                                         uint32_t* pPhysicalDeviceGroupCount,
                                         VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties);

void vkGetImageMemoryRequirements2(const VkImageMemoryRequirementsInfo2* pInfo,
                                   VkMemoryRequirements2* pMemoryRequirements);

void vkGetBufferMemoryRequirements2(const VkBufferMemoryRequirementsInfo2* pInfo,
                                    VkMemoryRequirements2* pMemoryRequirements);

void vkGetImageSparseMemoryRequirements2(const VkImageSparseMemoryRequirementsInfo2* pInfo,
                                         uint32_t* pSparseMemoryRequirementCount,
                                         VkSparseImageMemoryRequirements2* pSparseMemoryRequirements);

void vkGetPhysicalDeviceFeatures2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures);

void vkGetPhysicalDeviceProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties);

void vkGetPhysicalDeviceFormatProperties2(VkPhysicalDevice physicalDevice,
                                          VkFormat format,
                                          VkFormatProperties2* pFormatProperties);

VkResult vkGetPhysicalDeviceImageFormatProperties2(VkPhysicalDevice physicalDevice,
                                                   const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo,
                                                   VkImageFormatProperties2* pImageFormatProperties);

void vkGetPhysicalDeviceQueueFamilyProperties2(VkPhysicalDevice physicalDevice,
                                               uint32_t* pQueueFamilyPropertyCount,
                                               VkQueueFamilyProperties2* pQueueFamilyProperties);

void vkGetPhysicalDeviceMemoryProperties2(VkPhysicalDevice physicalDevice,
                                          VkPhysicalDeviceMemoryProperties2* pMemoryProperties);

void vkGetPhysicalDeviceSparseImageFormatProperties2(VkPhysicalDevice physicalDevice,
                                                     const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo,
                                                     uint32_t* pPropertyCount,
                                                     VkSparseImageFormatProperties2* pProperties);

void vkTrimCommandPool(VkCommandPool commandPool, VkCommandPoolTrimFlags flags);

void vkGetDeviceQueue2(const VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue);

VkResult vkCreateSamplerYcbcrConversion(const VkSamplerYcbcrConversionCreateInfo* pCreateInfo,
                                        const VkAllocationCallbacks* pAllocator,
                                        VkSamplerYcbcrConversion* pYcbcrConversion);

void vkDestroySamplerYcbcrConversion(VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator);

VkResult vkCreateDescriptorUpdateTemplate(const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo,
                                          const VkAllocationCallbacks* pAllocator,
                                          VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate);

void vkDestroyDescriptorUpdateTemplate(VkDescriptorUpdateTemplate descriptorUpdateTemplate,
                                       const VkAllocationCallbacks* pAllocator);

void vkUpdateDescriptorSetWithTemplate(VkDescriptorSet descriptorSet,
                                       VkDescriptorUpdateTemplate descriptorUpdateTemplate,
                                       const void* pData);

void vkGetPhysicalDeviceExternalBufferProperties(VkPhysicalDevice physicalDevice,
                                                 const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo,
                                                 VkExternalBufferProperties* pExternalBufferProperties);

void vkGetPhysicalDeviceExternalFenceProperties(VkPhysicalDevice physicalDevice,
                                                const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo,
                                                VkExternalFenceProperties* pExternalFenceProperties);

void vkGetPhysicalDeviceExternalSemaphoreProperties(VkPhysicalDevice physicalDevice,
                                                    const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo,
                                                    VkExternalSemaphoreProperties* pExternalSemaphoreProperties);

void vkGetDescriptorSetLayoutSupport(const VkDescriptorSetLayoutCreateInfo* pCreateInfo,
                                     VkDescriptorSetLayoutSupport* pSupport);

void vkCmdDrawIndirectCount(VkCommandBuffer commandBuffer,
                            VkBuffer buffer,
                            VkDeviceSize offset,
                            VkBuffer countBuffer,
                            VkDeviceSize countBufferOffset,
                            uint32_t maxDrawCount,
                            uint32_t stride);

void vkCmdDrawIndexedIndirectCount(VkCommandBuffer commandBuffer,
                                   VkBuffer buffer,
                                   VkDeviceSize offset,
                                   VkBuffer countBuffer,
                                   VkDeviceSize countBufferOffset,
                                   uint32_t maxDrawCount,
                                   uint32_t stride);

VkResult vkCreateRenderPass2(const VkRenderPassCreateInfo2* pCreateInfo,
                             const VkAllocationCallbacks* pAllocator,
                             VkRenderPass* pRenderPass);

void vkCmdBeginRenderPass2(VkCommandBuffer commandBuffer,
                           const VkRenderPassBeginInfo* pRenderPassBegin,
                           const VkSubpassBeginInfo* pSubpassBeginInfo);

void vkCmdNextSubpass2(VkCommandBuffer commandBuffer,
                       const VkSubpassBeginInfo* pSubpassBeginInfo,
                       const VkSubpassEndInfo* pSubpassEndInfo);

void vkCmdEndRenderPass2(VkCommandBuffer commandBuffer, const VkSubpassEndInfo* pSubpassEndInfo);

void vkResetQueryPool(VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount);

VkResult vkGetSemaphoreCounterValue(VkSemaphore semaphore, uint64_t* pValue);

VkResult vkWaitSemaphores(const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout);

VkResult vkSignalSemaphore(const VkSemaphoreSignalInfo* pSignalInfo);

VkDeviceAddress vkGetBufferDeviceAddress(const VkBufferDeviceAddressInfo* pInfo);

uint64_t vkGetBufferOpaqueCaptureAddress(const VkBufferDeviceAddressInfo* pInfo);

uint64_t vkGetDeviceMemoryOpaqueCaptureAddress(const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo);

VkResult vkGetPhysicalDeviceToolProperties(VkPhysicalDevice physicalDevice,
                                           uint32_t* pToolCount,
                                           VkPhysicalDeviceToolProperties* pToolProperties);

VkResult vkCreatePrivateDataSlot(const VkPrivateDataSlotCreateInfo* pCreateInfo,
                                 const VkAllocationCallbacks* pAllocator,
                                 VkPrivateDataSlot* pPrivateDataSlot);

void vkDestroyPrivateDataSlot(VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator);

VkResult vkSetPrivateData(VkObjectType objectType,
                          uint64_t objectHandle,
                          VkPrivateDataSlot privateDataSlot,
                          uint64_t data);

void vkGetPrivateData(VkObjectType objectType,
                      uint64_t objectHandle,
                      VkPrivateDataSlot privateDataSlot,
                      uint64_t* pData);

void vkCmdSetEvent2(VkCommandBuffer commandBuffer, VkEvent event, const VkDependencyInfo* pDependencyInfo);

void vkCmdResetEvent2(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags2 stageMask);

void vkCmdWaitEvents2(VkCommandBuffer commandBuffer,
                      uint32_t eventCount,
                      const VkEvent* pEvents,
                      const VkDependencyInfo* pDependencyInfos);

void vkCmdPipelineBarrier2(VkCommandBuffer commandBuffer, const VkDependencyInfo* pDependencyInfo);

void vkCmdWriteTimestamp2(VkCommandBuffer commandBuffer,
                          VkPipelineStageFlags2 stage,
                          VkQueryPool queryPool,
                          uint32_t query);

VkResult vkQueueSubmit2(VkQueue queue, uint32_t submitCount, const VkSubmitInfo2* pSubmits, VkFence fence);

void vkCmdCopyBuffer2(VkCommandBuffer commandBuffer, const VkCopyBufferInfo2* pCopyBufferInfo);

void vkCmdCopyImage2(VkCommandBuffer commandBuffer, const VkCopyImageInfo2* pCopyImageInfo);

void vkCmdCopyBufferToImage2(VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo);

void vkCmdCopyImageToBuffer2(VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo);

void vkCmdBlitImage2(VkCommandBuffer commandBuffer, const VkBlitImageInfo2* pBlitImageInfo);

void vkCmdResolveImage2(VkCommandBuffer commandBuffer, const VkResolveImageInfo2* pResolveImageInfo);

void vkCmdBeginRendering(VkCommandBuffer commandBuffer, const VkRenderingInfo* pRenderingInfo);

void vkCmdEndRendering(VkCommandBuffer commandBuffer);

void vkCmdSetCullMode(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode);

void vkCmdSetFrontFace(VkCommandBuffer commandBuffer, VkFrontFace frontFace);

void vkCmdSetPrimitiveTopology(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology);

void vkCmdSetViewportWithCount(VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports);

void vkCmdSetScissorWithCount(VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors);

void vkCmdBindVertexBuffers2(VkCommandBuffer commandBuffer,
                             uint32_t firstBinding,
                             uint32_t bindingCount,
                             const VkBuffer* pBuffers,
                             const VkDeviceSize* pOffsets,
                             const VkDeviceSize* pSizes,
                             const VkDeviceSize* pStrides);

void vkCmdSetDepthTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable);

void vkCmdSetDepthWriteEnable(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable);

void vkCmdSetDepthCompareOp(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp);

void vkCmdSetDepthBoundsTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable);

void vkCmdSetStencilTestEnable(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable);

void vkCmdSetStencilOp(VkCommandBuffer commandBuffer,
                       VkStencilFaceFlags faceMask,
                       VkStencilOp failOp,
                       VkStencilOp passOp,
                       VkStencilOp depthFailOp,
                       VkCompareOp compareOp);

void vkCmdSetRasterizerDiscardEnable(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable);

void vkCmdSetDepthBiasEnable(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable);

void vkCmdSetPrimitiveRestartEnable(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable);

void vkGetDeviceBufferMemoryRequirements(const VkDeviceBufferMemoryRequirements* pInfo,
                                         VkMemoryRequirements2* pMemoryRequirements);

void vkGetDeviceImageMemoryRequirements(const VkDeviceImageMemoryRequirements* pInfo,
                                        VkMemoryRequirements2* pMemoryRequirements);

void vkGetDeviceImageSparseMemoryRequirements(const VkDeviceImageMemoryRequirements* pInfo,
                                              uint32_t* pSparseMemoryRequirementCount,
                                              VkSparseImageMemoryRequirements2* pSparseMemoryRequirements);

void vkDestroySurfaceKHR(VkInstance instance, VkSurfaceKHR surface, const VkAllocationCallbacks* pAllocator);

VkResult vkGetPhysicalDeviceSurfaceSupportKHR(VkPhysicalDevice physicalDevice,
                                              uint32_t queueFamilyIndex,
                                              VkSurfaceKHR surface,
                                              VkBool32* pSupported);

VkResult vkGetPhysicalDeviceSurfaceCapabilitiesKHR(VkPhysicalDevice physicalDevice,
                                                   VkSurfaceKHR surface,
                                                   VkSurfaceCapabilitiesKHR* pSurfaceCapabilities);

VkResult vkGetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice physicalDevice,
                                              VkSurfaceKHR surface,
                                              uint32_t* pSurfaceFormatCount,
                                              VkSurfaceFormatKHR* pSurfaceFormats);

VkResult vkGetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice physicalDevice,
                                                   VkSurfaceKHR surface,
                                                   uint32_t* pPresentModeCount,
                                                   VkPresentModeKHR* pPresentModes);

VkResult vkCreateSwapchainKHR(const VkSwapchainCreateInfoKHR* pCreateInfo,
                              const VkAllocationCallbacks* pAllocator,
                              VkSwapchainKHR* pSwapchain);

void vkDestroySwapchainKHR(VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator);

VkResult vkGetSwapchainImagesKHR(VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages);

VkResult vkAcquireNextImageKHR(VkSwapchainKHR swapchain,
                               uint64_t timeout,
                               VkSemaphore semaphore,
                               VkFence fence,
                               uint32_t* pImageIndex);

VkResult vkQueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo);

VkResult vkGetDeviceGroupPresentCapabilitiesKHR(VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities);

VkResult vkGetDeviceGroupSurfacePresentModesKHR(VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes);

VkResult vkGetPhysicalDevicePresentRectanglesKHR(VkPhysicalDevice physicalDevice,
                                                 VkSurfaceKHR surface,
                                                 uint32_t* pRectCount,
                                                 VkRect2D* pRects);

VkResult vkAcquireNextImage2KHR(const VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex);

VkResult vkGetPhysicalDeviceDisplayPropertiesKHR(VkPhysicalDevice physicalDevice,
                                                 uint32_t* pPropertyCount,
                                                 VkDisplayPropertiesKHR* pProperties);

VkResult vkGetPhysicalDeviceDisplayPlanePropertiesKHR(VkPhysicalDevice physicalDevice,
                                                      uint32_t* pPropertyCount,
                                                      VkDisplayPlanePropertiesKHR* pProperties);

VkResult vkGetDisplayPlaneSupportedDisplaysKHR(VkPhysicalDevice physicalDevice,
                                               uint32_t planeIndex,
                                               uint32_t* pDisplayCount,
                                               VkDisplayKHR* pDisplays);

VkResult vkGetDisplayModePropertiesKHR(VkPhysicalDevice physicalDevice,
                                       VkDisplayKHR display,
                                       uint32_t* pPropertyCount,
                                       VkDisplayModePropertiesKHR* pProperties);

VkResult vkCreateDisplayModeKHR(VkPhysicalDevice physicalDevice,
                                VkDisplayKHR display,
                                const VkDisplayModeCreateInfoKHR* pCreateInfo,
                                const VkAllocationCallbacks* pAllocator,
                                VkDisplayModeKHR* pMode);

VkResult vkGetDisplayPlaneCapabilitiesKHR(VkPhysicalDevice physicalDevice,
                                          VkDisplayModeKHR mode,
                                          uint32_t planeIndex,
                                          VkDisplayPlaneCapabilitiesKHR* pCapabilities);

VkResult vkCreateDisplayPlaneSurfaceKHR(VkInstance instance,
                                        const VkDisplaySurfaceCreateInfoKHR* pCreateInfo,
                                        const VkAllocationCallbacks* pAllocator,
                                        VkSurfaceKHR* pSurface);

VkResult vkCreateSharedSwapchainsKHR(uint32_t swapchainCount,
                                     const VkSwapchainCreateInfoKHR* pCreateInfos,
                                     const VkAllocationCallbacks* pAllocator,
                                     VkSwapchainKHR* pSwapchains);

void vkCmdBeginRenderingKHR(VkCommandBuffer commandBuffer, const VkRenderingInfo* pRenderingInfo);

void vkCmdEndRenderingKHR(VkCommandBuffer commandBuffer);

void vkGetPhysicalDeviceFeatures2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures);

void vkGetPhysicalDeviceProperties2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties);

void vkGetPhysicalDeviceFormatProperties2KHR(VkPhysicalDevice physicalDevice,
                                             VkFormat format,
                                             VkFormatProperties2* pFormatProperties);

VkResult vkGetPhysicalDeviceImageFormatProperties2KHR(VkPhysicalDevice physicalDevice,
                                                      const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo,
                                                      VkImageFormatProperties2* pImageFormatProperties);

void vkGetPhysicalDeviceQueueFamilyProperties2KHR(VkPhysicalDevice physicalDevice,
                                                  uint32_t* pQueueFamilyPropertyCount,
                                                  VkQueueFamilyProperties2* pQueueFamilyProperties);

void vkGetPhysicalDeviceMemoryProperties2KHR(VkPhysicalDevice physicalDevice,
                                             VkPhysicalDeviceMemoryProperties2* pMemoryProperties);

void vkGetPhysicalDeviceSparseImageFormatProperties2KHR(VkPhysicalDevice physicalDevice,
                                                        const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo,
                                                        uint32_t* pPropertyCount,
                                                        VkSparseImageFormatProperties2* pProperties);

void vkGetDeviceGroupPeerMemoryFeaturesKHR(uint32_t heapIndex,
                                           uint32_t localDeviceIndex,
                                           uint32_t remoteDeviceIndex,
                                           VkPeerMemoryFeatureFlags* pPeerMemoryFeatures);

void vkCmdSetDeviceMaskKHR(VkCommandBuffer commandBuffer, uint32_t deviceMask);

void vkCmdDispatchBaseKHR(VkCommandBuffer commandBuffer,
                          uint32_t baseGroupX,
                          uint32_t baseGroupY,
                          uint32_t baseGroupZ,
                          uint32_t groupCountX,
                          uint32_t groupCountY,
                          uint32_t groupCountZ);

void vkTrimCommandPoolKHR(VkCommandPool commandPool, VkCommandPoolTrimFlags flags);

VkResult vkEnumeratePhysicalDeviceGroupsKHR(VkInstance instance,
                                            uint32_t* pPhysicalDeviceGroupCount,
                                            VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties);

void vkGetPhysicalDeviceExternalBufferPropertiesKHR(VkPhysicalDevice physicalDevice,
                                                    const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo,
                                                    VkExternalBufferProperties* pExternalBufferProperties);

VkResult vkGetMemoryFdKHR(const VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd);

VkResult vkGetMemoryFdPropertiesKHR(VkExternalMemoryHandleTypeFlagBits handleType,
                                    int fd,
                                    VkMemoryFdPropertiesKHR* pMemoryFdProperties);

void vkGetPhysicalDeviceExternalSemaphorePropertiesKHR(VkPhysicalDevice physicalDevice,
                                                       const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo,
                                                       VkExternalSemaphoreProperties* pExternalSemaphoreProperties);

VkResult vkImportSemaphoreFdKHR(const VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo);

VkResult vkGetSemaphoreFdKHR(const VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd);

void vkCmdPushDescriptorSetKHR(VkCommandBuffer commandBuffer,
                               VkPipelineBindPoint pipelineBindPoint,
                               VkPipelineLayout layout,
                               uint32_t set,
                               uint32_t descriptorWriteCount,
                               const VkWriteDescriptorSet* pDescriptorWrites);

void vkCmdPushDescriptorSetWithTemplateKHR(VkCommandBuffer commandBuffer,
                                           VkDescriptorUpdateTemplate descriptorUpdateTemplate,
                                           VkPipelineLayout layout,
                                           uint32_t set,
                                           const void* pData);

VkResult vkCreateDescriptorUpdateTemplateKHR(const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo,
                                             const VkAllocationCallbacks* pAllocator,
                                             VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate);

void vkDestroyDescriptorUpdateTemplateKHR(VkDescriptorUpdateTemplate descriptorUpdateTemplate,
                                          const VkAllocationCallbacks* pAllocator);

void vkUpdateDescriptorSetWithTemplateKHR(VkDescriptorSet descriptorSet,
                                          VkDescriptorUpdateTemplate descriptorUpdateTemplate,
                                          const void* pData);

VkResult vkCreateRenderPass2KHR(const VkRenderPassCreateInfo2* pCreateInfo,
                                const VkAllocationCallbacks* pAllocator,
                                VkRenderPass* pRenderPass);

void vkCmdBeginRenderPass2KHR(VkCommandBuffer commandBuffer,
                              const VkRenderPassBeginInfo* pRenderPassBegin,
                              const VkSubpassBeginInfo* pSubpassBeginInfo);

void vkCmdNextSubpass2KHR(VkCommandBuffer commandBuffer,
                          const VkSubpassBeginInfo* pSubpassBeginInfo,
                          const VkSubpassEndInfo* pSubpassEndInfo);

void vkCmdEndRenderPass2KHR(VkCommandBuffer commandBuffer, const VkSubpassEndInfo* pSubpassEndInfo);

VkResult vkGetSwapchainStatusKHR(VkSwapchainKHR swapchain);

void vkGetPhysicalDeviceExternalFencePropertiesKHR(VkPhysicalDevice physicalDevice,
                                                   const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo,
                                                   VkExternalFenceProperties* pExternalFenceProperties);

VkResult vkImportFenceFdKHR(const VkImportFenceFdInfoKHR* pImportFenceFdInfo);

VkResult vkGetFenceFdKHR(const VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd);

VkResult vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(VkPhysicalDevice physicalDevice,
                                                                         uint32_t queueFamilyIndex,
                                                                         uint32_t* pCounterCount,
                                                                         VkPerformanceCounterKHR* pCounters,
                                                                         VkPerformanceCounterDescriptionKHR* pCounterDescriptions);

void vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(VkPhysicalDevice physicalDevice,
                                                             const VkQueryPoolPerformanceCreateInfoKHR* pPerformanceQueryCreateInfo,
                                                             uint32_t* pNumPasses);

VkResult vkAcquireProfilingLockKHR(const VkAcquireProfilingLockInfoKHR* pInfo);

void vkReleaseProfilingLockKHR();

VkResult vkGetPhysicalDeviceSurfaceCapabilities2KHR(VkPhysicalDevice physicalDevice,
                                                    const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo,
                                                    VkSurfaceCapabilities2KHR* pSurfaceCapabilities);

VkResult vkGetPhysicalDeviceSurfaceFormats2KHR(VkPhysicalDevice physicalDevice,
                                               const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo,
                                               uint32_t* pSurfaceFormatCount,
                                               VkSurfaceFormat2KHR* pSurfaceFormats);

VkResult vkGetPhysicalDeviceDisplayProperties2KHR(VkPhysicalDevice physicalDevice,
                                                  uint32_t* pPropertyCount,
                                                  VkDisplayProperties2KHR* pProperties);

VkResult vkGetPhysicalDeviceDisplayPlaneProperties2KHR(VkPhysicalDevice physicalDevice,
                                                       uint32_t* pPropertyCount,
                                                       VkDisplayPlaneProperties2KHR* pProperties);

VkResult vkGetDisplayModeProperties2KHR(VkPhysicalDevice physicalDevice,
                                        VkDisplayKHR display,
                                        uint32_t* pPropertyCount,
                                        VkDisplayModeProperties2KHR* pProperties);

VkResult vkGetDisplayPlaneCapabilities2KHR(VkPhysicalDevice physicalDevice,
                                           const VkDisplayPlaneInfo2KHR* pDisplayPlaneInfo,
                                           VkDisplayPlaneCapabilities2KHR* pCapabilities);

void vkGetImageMemoryRequirements2KHR(const VkImageMemoryRequirementsInfo2* pInfo,
                                      VkMemoryRequirements2* pMemoryRequirements);

void vkGetBufferMemoryRequirements2KHR(const VkBufferMemoryRequirementsInfo2* pInfo,
                                       VkMemoryRequirements2* pMemoryRequirements);

void vkGetImageSparseMemoryRequirements2KHR(const VkImageSparseMemoryRequirementsInfo2* pInfo,
                                            uint32_t* pSparseMemoryRequirementCount,
                                            VkSparseImageMemoryRequirements2* pSparseMemoryRequirements);

VkResult vkCreateSamplerYcbcrConversionKHR(const VkSamplerYcbcrConversionCreateInfo* pCreateInfo,
                                           const VkAllocationCallbacks* pAllocator,
                                           VkSamplerYcbcrConversion* pYcbcrConversion);

void vkDestroySamplerYcbcrConversionKHR(VkSamplerYcbcrConversion ycbcrConversion,
                                        const VkAllocationCallbacks* pAllocator);

VkResult vkBindBufferMemory2KHR(uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos);

VkResult vkBindImageMemory2KHR(uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos);

void vkGetDescriptorSetLayoutSupportKHR(const VkDescriptorSetLayoutCreateInfo* pCreateInfo,
                                        VkDescriptorSetLayoutSupport* pSupport);

void vkCmdDrawIndirectCountKHR(VkCommandBuffer commandBuffer,
                               VkBuffer buffer,
                               VkDeviceSize offset,
                               VkBuffer countBuffer,
                               VkDeviceSize countBufferOffset,
                               uint32_t maxDrawCount,
                               uint32_t stride);

void vkCmdDrawIndexedIndirectCountKHR(VkCommandBuffer commandBuffer,
                                      VkBuffer buffer,
                                      VkDeviceSize offset,
                                      VkBuffer countBuffer,
                                      VkDeviceSize countBufferOffset,
                                      uint32_t maxDrawCount,
                                      uint32_t stride);

VkResult vkGetSemaphoreCounterValueKHR(VkSemaphore semaphore, uint64_t* pValue);

VkResult vkWaitSemaphoresKHR(const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout);

VkResult vkSignalSemaphoreKHR(const VkSemaphoreSignalInfo* pSignalInfo);

VkResult vkGetPhysicalDeviceFragmentShadingRatesKHR(VkPhysicalDevice physicalDevice,
                                                    uint32_t* pFragmentShadingRateCount,
                                                    VkPhysicalDeviceFragmentShadingRateKHR* pFragmentShadingRates);

void vkCmdSetFragmentShadingRateKHR(VkCommandBuffer commandBuffer,
                                    const VkExtent2D* pFragmentSize,
                                    const VkFragmentShadingRateCombinerOpKHR combinerOps[2]);

VkResult vkWaitForPresentKHR(VkSwapchainKHR swapchain, uint64_t presentId, uint64_t timeout);

VkDeviceAddress vkGetBufferDeviceAddressKHR(const VkBufferDeviceAddressInfo* pInfo);

uint64_t vkGetBufferOpaqueCaptureAddressKHR(const VkBufferDeviceAddressInfo* pInfo);

uint64_t vkGetDeviceMemoryOpaqueCaptureAddressKHR(const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo);

VkResult vkCreateDeferredOperationKHR(const VkAllocationCallbacks* pAllocator,
                                      VkDeferredOperationKHR* pDeferredOperation);

void vkDestroyDeferredOperationKHR(VkDeferredOperationKHR operation, const VkAllocationCallbacks* pAllocator);

uint32_t vkGetDeferredOperationMaxConcurrencyKHR(VkDeferredOperationKHR operation);

VkResult vkGetDeferredOperationResultKHR(VkDeferredOperationKHR operation);

VkResult vkDeferredOperationJoinKHR(VkDeferredOperationKHR operation);

VkResult vkGetPipelineExecutablePropertiesKHR(const VkPipelineInfoKHR* pPipelineInfo,
                                              uint32_t* pExecutableCount,
                                              VkPipelineExecutablePropertiesKHR* pProperties);

VkResult vkGetPipelineExecutableStatisticsKHR(const VkPipelineExecutableInfoKHR* pExecutableInfo,
                                              uint32_t* pStatisticCount,
                                              VkPipelineExecutableStatisticKHR* pStatistics);

VkResult vkGetPipelineExecutableInternalRepresentationsKHR(const VkPipelineExecutableInfoKHR* pExecutableInfo,
                                                           uint32_t* pInternalRepresentationCount,
                                                           VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations);

void vkCmdSetEvent2KHR(VkCommandBuffer commandBuffer, VkEvent event, const VkDependencyInfo* pDependencyInfo);

void vkCmdResetEvent2KHR(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags2 stageMask);

void vkCmdWaitEvents2KHR(VkCommandBuffer commandBuffer,
                         uint32_t eventCount,
                         const VkEvent* pEvents,
                         const VkDependencyInfo* pDependencyInfos);

void vkCmdPipelineBarrier2KHR(VkCommandBuffer commandBuffer, const VkDependencyInfo* pDependencyInfo);

void vkCmdWriteTimestamp2KHR(VkCommandBuffer commandBuffer,
                             VkPipelineStageFlags2 stage,
                             VkQueryPool queryPool,
                             uint32_t query);

VkResult vkQueueSubmit2KHR(VkQueue queue, uint32_t submitCount, const VkSubmitInfo2* pSubmits, VkFence fence);

void vkCmdWriteBufferMarker2AMD(VkCommandBuffer commandBuffer,
                                VkPipelineStageFlags2 stage,
                                VkBuffer dstBuffer,
                                VkDeviceSize dstOffset,
                                uint32_t marker);

void vkGetQueueCheckpointData2NV(VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointData2NV* pCheckpointData);

void vkCmdCopyBuffer2KHR(VkCommandBuffer commandBuffer, const VkCopyBufferInfo2* pCopyBufferInfo);

void vkCmdCopyImage2KHR(VkCommandBuffer commandBuffer, const VkCopyImageInfo2* pCopyImageInfo);

void vkCmdCopyBufferToImage2KHR(VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo);

void vkCmdCopyImageToBuffer2KHR(VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo);

void vkCmdBlitImage2KHR(VkCommandBuffer commandBuffer, const VkBlitImageInfo2* pBlitImageInfo);

void vkCmdResolveImage2KHR(VkCommandBuffer commandBuffer, const VkResolveImageInfo2* pResolveImageInfo);

void vkGetDeviceBufferMemoryRequirementsKHR(const VkDeviceBufferMemoryRequirements* pInfo,
                                            VkMemoryRequirements2* pMemoryRequirements);

void vkGetDeviceImageMemoryRequirementsKHR(const VkDeviceImageMemoryRequirements* pInfo,
                                           VkMemoryRequirements2* pMemoryRequirements);

void vkGetDeviceImageSparseMemoryRequirementsKHR(const VkDeviceImageMemoryRequirements* pInfo,
                                                 uint32_t* pSparseMemoryRequirementCount,
                                                 VkSparseImageMemoryRequirements2* pSparseMemoryRequirements);

VkResult vkCreateDebugReportCallbackEXT(VkInstance instance,
                                        const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
                                        const VkAllocationCallbacks* pAllocator,
                                        VkDebugReportCallbackEXT* pCallback);

void vkDestroyDebugReportCallbackEXT(VkInstance instance,
                                     VkDebugReportCallbackEXT callback,
                                     const VkAllocationCallbacks* pAllocator);

void vkDebugReportMessageEXT(VkInstance instance,
                             VkDebugReportFlagsEXT flags,
                             VkDebugReportObjectTypeEXT objectType,
                             uint64_t object,
                             size_t location,
                             int32_t messageCode,
                             const char* pLayerPrefix,
                             const char* pMessage);

VkResult vkDebugMarkerSetObjectTagEXT(const VkDebugMarkerObjectTagInfoEXT* pTagInfo);

VkResult vkDebugMarkerSetObjectNameEXT(const VkDebugMarkerObjectNameInfoEXT* pNameInfo);

void vkCmdDebugMarkerBeginEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo);

void vkCmdDebugMarkerEndEXT(VkCommandBuffer commandBuffer);

void vkCmdDebugMarkerInsertEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo);

void vkCmdBindTransformFeedbackBuffersEXT(VkCommandBuffer commandBuffer,
                                          uint32_t firstBinding,
                                          uint32_t bindingCount,
                                          const VkBuffer* pBuffers,
                                          const VkDeviceSize* pOffsets,
                                          const VkDeviceSize* pSizes);

void vkCmdBeginTransformFeedbackEXT(VkCommandBuffer commandBuffer,
                                    uint32_t firstCounterBuffer,
                                    uint32_t counterBufferCount,
                                    const VkBuffer* pCounterBuffers,
                                    const VkDeviceSize* pCounterBufferOffsets);

void vkCmdEndTransformFeedbackEXT(VkCommandBuffer commandBuffer,
                                  uint32_t firstCounterBuffer,
                                  uint32_t counterBufferCount,
                                  const VkBuffer* pCounterBuffers,
                                  const VkDeviceSize* pCounterBufferOffsets);

void vkCmdBeginQueryIndexedEXT(VkCommandBuffer commandBuffer,
                               VkQueryPool queryPool,
                               uint32_t query,
                               VkQueryControlFlags flags,
                               uint32_t index);

void vkCmdEndQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index);

void vkCmdDrawIndirectByteCountEXT(VkCommandBuffer commandBuffer,
                                   uint32_t instanceCount,
                                   uint32_t firstInstance,
                                   VkBuffer counterBuffer,
                                   VkDeviceSize counterBufferOffset,
                                   uint32_t counterOffset,
                                   uint32_t vertexStride);

VkResult vkCreateCuModuleNVX(const VkCuModuleCreateInfoNVX* pCreateInfo,
                             const VkAllocationCallbacks* pAllocator,
                             VkCuModuleNVX* pModule);

VkResult vkCreateCuFunctionNVX(const VkCuFunctionCreateInfoNVX* pCreateInfo,
                               const VkAllocationCallbacks* pAllocator,
                               VkCuFunctionNVX* pFunction);

void vkDestroyCuModuleNVX(VkCuModuleNVX module, const VkAllocationCallbacks* pAllocator);

void vkDestroyCuFunctionNVX(VkCuFunctionNVX function, const VkAllocationCallbacks* pAllocator);

void vkCmdCuLaunchKernelNVX(VkCommandBuffer commandBuffer, const VkCuLaunchInfoNVX* pLaunchInfo);

uint32_t vkGetImageViewHandleNVX(const VkImageViewHandleInfoNVX* pInfo);

VkResult vkGetImageViewAddressNVX(VkImageView imageView, VkImageViewAddressPropertiesNVX* pProperties);

void vkCmdDrawIndirectCountAMD(VkCommandBuffer commandBuffer,
                               VkBuffer buffer,
                               VkDeviceSize offset,
                               VkBuffer countBuffer,
                               VkDeviceSize countBufferOffset,
                               uint32_t maxDrawCount,
                               uint32_t stride);

void vkCmdDrawIndexedIndirectCountAMD(VkCommandBuffer commandBuffer,
                                      VkBuffer buffer,
                                      VkDeviceSize offset,
                                      VkBuffer countBuffer,
                                      VkDeviceSize countBufferOffset,
                                      uint32_t maxDrawCount,
                                      uint32_t stride);

VkResult vkGetShaderInfoAMD(VkPipeline pipeline,
                            VkShaderStageFlagBits shaderStage,
                            VkShaderInfoTypeAMD infoType,
                            size_t* pInfoSize,
                            void* pInfo);

VkResult vkGetPhysicalDeviceExternalImageFormatPropertiesNV(VkPhysicalDevice physicalDevice,
                                                            VkFormat format,
                                                            VkImageType type,
                                                            VkImageTiling tiling,
                                                            VkImageUsageFlags usage,
                                                            VkImageCreateFlags flags,
                                                            VkExternalMemoryHandleTypeFlagsNV externalHandleType,
                                                            VkExternalImageFormatPropertiesNV* pExternalImageFormatProperties);

void vkCmdBeginConditionalRenderingEXT(VkCommandBuffer commandBuffer,
                                       const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin);

void vkCmdEndConditionalRenderingEXT(VkCommandBuffer commandBuffer);

void vkCmdSetViewportWScalingNV(VkCommandBuffer commandBuffer,
                                uint32_t firstViewport,
                                uint32_t viewportCount,
                                const VkViewportWScalingNV* pViewportWScalings);

VkResult vkReleaseDisplayEXT(VkPhysicalDevice physicalDevice, VkDisplayKHR display);

VkResult vkGetPhysicalDeviceSurfaceCapabilities2EXT(VkPhysicalDevice physicalDevice,
                                                    VkSurfaceKHR surface,
                                                    VkSurfaceCapabilities2EXT* pSurfaceCapabilities);

VkResult vkDisplayPowerControlEXT(VkDisplayKHR display, const VkDisplayPowerInfoEXT* pDisplayPowerInfo);

VkResult vkRegisterDeviceEventEXT(const VkDeviceEventInfoEXT* pDeviceEventInfo,
                                  const VkAllocationCallbacks* pAllocator,
                                  VkFence* pFence);

VkResult vkRegisterDisplayEventEXT(VkDisplayKHR display,
                                   const VkDisplayEventInfoEXT* pDisplayEventInfo,
                                   const VkAllocationCallbacks* pAllocator,
                                   VkFence* pFence);

VkResult vkGetSwapchainCounterEXT(VkSwapchainKHR swapchain,
                                  VkSurfaceCounterFlagBitsEXT counter,
                                  uint64_t* pCounterValue);

VkResult vkGetRefreshCycleDurationGOOGLE(VkSwapchainKHR swapchain,
                                         VkRefreshCycleDurationGOOGLE* pDisplayTimingProperties);

VkResult vkGetPastPresentationTimingGOOGLE(VkSwapchainKHR swapchain,
                                           uint32_t* pPresentationTimingCount,
                                           VkPastPresentationTimingGOOGLE* pPresentationTimings);

void vkCmdSetDiscardRectangleEXT(VkCommandBuffer commandBuffer,
                                 uint32_t firstDiscardRectangle,
                                 uint32_t discardRectangleCount,
                                 const VkRect2D* pDiscardRectangles);

void vkSetHdrMetadataEXT(uint32_t swapchainCount, const VkSwapchainKHR* pSwapchains, const VkHdrMetadataEXT* pMetadata);

VkResult vkSetDebugUtilsObjectNameEXT(const VkDebugUtilsObjectNameInfoEXT* pNameInfo);

VkResult vkSetDebugUtilsObjectTagEXT(const VkDebugUtilsObjectTagInfoEXT* pTagInfo);

void vkQueueBeginDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo);

void vkQueueEndDebugUtilsLabelEXT(VkQueue queue);

void vkQueueInsertDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo);

void vkCmdBeginDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo);

void vkCmdEndDebugUtilsLabelEXT(VkCommandBuffer commandBuffer);

void vkCmdInsertDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo);

VkResult vkCreateDebugUtilsMessengerEXT(VkInstance instance,
                                        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                        const VkAllocationCallbacks* pAllocator,
                                        VkDebugUtilsMessengerEXT* pMessenger);

void vkDestroyDebugUtilsMessengerEXT(VkInstance instance,
                                     VkDebugUtilsMessengerEXT messenger,
                                     const VkAllocationCallbacks* pAllocator);

void vkSubmitDebugUtilsMessageEXT(VkInstance instance,
                                  VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                  VkDebugUtilsMessageTypeFlagsEXT messageTypes,
                                  const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData);

void vkCmdSetSampleLocationsEXT(VkCommandBuffer commandBuffer, const VkSampleLocationsInfoEXT* pSampleLocationsInfo);

void vkGetPhysicalDeviceMultisamplePropertiesEXT(VkPhysicalDevice physicalDevice,
                                                 VkSampleCountFlagBits samples,
                                                 VkMultisamplePropertiesEXT* pMultisampleProperties);

VkResult vkGetImageDrmFormatModifierPropertiesEXT(VkImage image, VkImageDrmFormatModifierPropertiesEXT* pProperties);

VkResult vkCreateValidationCacheEXT(const VkValidationCacheCreateInfoEXT* pCreateInfo,
                                    const VkAllocationCallbacks* pAllocator,
                                    VkValidationCacheEXT* pValidationCache);

void vkDestroyValidationCacheEXT(VkValidationCacheEXT validationCache, const VkAllocationCallbacks* pAllocator);

VkResult vkMergeValidationCachesEXT(VkValidationCacheEXT dstCache,
                                    uint32_t srcCacheCount,
                                    const VkValidationCacheEXT* pSrcCaches);

VkResult vkGetValidationCacheDataEXT(VkValidationCacheEXT validationCache, size_t* pDataSize, void* pData);

void vkCmdBindShadingRateImageNV(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout);

void vkCmdSetViewportShadingRatePaletteNV(VkCommandBuffer commandBuffer,
                                          uint32_t firstViewport,
                                          uint32_t viewportCount,
                                          const VkShadingRatePaletteNV* pShadingRatePalettes);

void vkCmdSetCoarseSampleOrderNV(VkCommandBuffer commandBuffer,
                                 VkCoarseSampleOrderTypeNV sampleOrderType,
                                 uint32_t customSampleOrderCount,
                                 const VkCoarseSampleOrderCustomNV* pCustomSampleOrders);

VkResult vkCreateAccelerationStructureNV(const VkAccelerationStructureCreateInfoNV* pCreateInfo,
                                         const VkAllocationCallbacks* pAllocator,
                                         VkAccelerationStructureNV* pAccelerationStructure);

void vkDestroyAccelerationStructureNV(VkAccelerationStructureNV accelerationStructure,
                                      const VkAllocationCallbacks* pAllocator);

void vkGetAccelerationStructureMemoryRequirementsNV(const VkAccelerationStructureMemoryRequirementsInfoNV* pInfo,
                                                    VkMemoryRequirements2KHR* pMemoryRequirements);

VkResult vkBindAccelerationStructureMemoryNV(uint32_t bindInfoCount,
                                             const VkBindAccelerationStructureMemoryInfoNV* pBindInfos);

void vkCmdBuildAccelerationStructureNV(VkCommandBuffer commandBuffer,
                                       const VkAccelerationStructureInfoNV* pInfo,
                                       VkBuffer instanceData,
                                       VkDeviceSize instanceOffset,
                                       VkBool32 update,
                                       VkAccelerationStructureNV dst,
                                       VkAccelerationStructureNV src,
                                       VkBuffer scratch,
                                       VkDeviceSize scratchOffset);

void vkCmdCopyAccelerationStructureNV(VkCommandBuffer commandBuffer,
                                      VkAccelerationStructureNV dst,
                                      VkAccelerationStructureNV src,
                                      VkCopyAccelerationStructureModeKHR mode);

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
                      uint32_t depth);

VkResult vkCreateRayTracingPipelinesNV(VkPipelineCache pipelineCache,
                                       uint32_t createInfoCount,
                                       const VkRayTracingPipelineCreateInfoNV* pCreateInfos,
                                       const VkAllocationCallbacks* pAllocator,
                                       VkPipeline* pPipelines);

VkResult vkGetRayTracingShaderGroupHandlesKHR(VkPipeline pipeline,
                                              uint32_t firstGroup,
                                              uint32_t groupCount,
                                              size_t dataSize,
                                              void* pData);

VkResult vkGetRayTracingShaderGroupHandlesNV(VkPipeline pipeline,
                                             uint32_t firstGroup,
                                             uint32_t groupCount,
                                             size_t dataSize,
                                             void* pData);

VkResult vkGetAccelerationStructureHandleNV(VkAccelerationStructureNV accelerationStructure,
                                            size_t dataSize,
                                            void* pData);

void vkCmdWriteAccelerationStructuresPropertiesNV(VkCommandBuffer commandBuffer,
                                                  uint32_t accelerationStructureCount,
                                                  const VkAccelerationStructureNV* pAccelerationStructures,
                                                  VkQueryType queryType,
                                                  VkQueryPool queryPool,
                                                  uint32_t firstQuery);

VkResult vkCompileDeferredNV(VkPipeline pipeline, uint32_t shader);

VkResult vkGetMemoryHostPointerPropertiesEXT(VkExternalMemoryHandleTypeFlagBits handleType,
                                             const void* pHostPointer,
                                             VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties);

void vkCmdWriteBufferMarkerAMD(VkCommandBuffer commandBuffer,
                               VkPipelineStageFlagBits pipelineStage,
                               VkBuffer dstBuffer,
                               VkDeviceSize dstOffset,
                               uint32_t marker);

VkResult vkGetPhysicalDeviceCalibrateableTimeDomainsEXT(VkPhysicalDevice physicalDevice,
                                                        uint32_t* pTimeDomainCount,
                                                        VkTimeDomainEXT* pTimeDomains);

VkResult vkGetCalibratedTimestampsEXT(uint32_t timestampCount,
                                      const VkCalibratedTimestampInfoEXT* pTimestampInfos,
                                      uint64_t* pTimestamps,
                                      uint64_t* pMaxDeviation);

void vkCmdDrawMeshTasksNV(VkCommandBuffer commandBuffer, uint32_t taskCount, uint32_t firstTask);

void vkCmdDrawMeshTasksIndirectNV(VkCommandBuffer commandBuffer,
                                  VkBuffer buffer,
                                  VkDeviceSize offset,
                                  uint32_t drawCount,
                                  uint32_t stride);

void vkCmdDrawMeshTasksIndirectCountNV(VkCommandBuffer commandBuffer,
                                       VkBuffer buffer,
                                       VkDeviceSize offset,
                                       VkBuffer countBuffer,
                                       VkDeviceSize countBufferOffset,
                                       uint32_t maxDrawCount,
                                       uint32_t stride);

void vkCmdSetExclusiveScissorNV(VkCommandBuffer commandBuffer,
                                uint32_t firstExclusiveScissor,
                                uint32_t exclusiveScissorCount,
                                const VkRect2D* pExclusiveScissors);

void vkCmdSetCheckpointNV(VkCommandBuffer commandBuffer, const void* pCheckpointMarker);

void vkGetQueueCheckpointDataNV(VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointDataNV* pCheckpointData);

VkResult vkInitializePerformanceApiINTEL(const VkInitializePerformanceApiInfoINTEL* pInitializeInfo);

void vkUninitializePerformanceApiINTEL();

VkResult vkCmdSetPerformanceMarkerINTEL(VkCommandBuffer commandBuffer, const VkPerformanceMarkerInfoINTEL* pMarkerInfo);

VkResult vkCmdSetPerformanceStreamMarkerINTEL(VkCommandBuffer commandBuffer,
                                              const VkPerformanceStreamMarkerInfoINTEL* pMarkerInfo);

VkResult vkCmdSetPerformanceOverrideINTEL(VkCommandBuffer commandBuffer,
                                          const VkPerformanceOverrideInfoINTEL* pOverrideInfo);

VkResult vkAcquirePerformanceConfigurationINTEL(const VkPerformanceConfigurationAcquireInfoINTEL* pAcquireInfo,
                                                VkPerformanceConfigurationINTEL* pConfiguration);

VkResult vkReleasePerformanceConfigurationINTEL(VkPerformanceConfigurationINTEL configuration);

VkResult vkQueueSetPerformanceConfigurationINTEL(VkQueue queue, VkPerformanceConfigurationINTEL configuration);

VkResult vkGetPerformanceParameterINTEL(VkPerformanceParameterTypeINTEL parameter, VkPerformanceValueINTEL* pValue);

void vkSetLocalDimmingAMD(VkSwapchainKHR swapChain, VkBool32 localDimmingEnable);

VkDeviceAddress vkGetBufferDeviceAddressEXT(const VkBufferDeviceAddressInfo* pInfo);

VkResult vkGetPhysicalDeviceToolPropertiesEXT(VkPhysicalDevice physicalDevice,
                                              uint32_t* pToolCount,
                                              VkPhysicalDeviceToolProperties* pToolProperties);

VkResult vkGetPhysicalDeviceCooperativeMatrixPropertiesNV(VkPhysicalDevice physicalDevice,
                                                          uint32_t* pPropertyCount,
                                                          VkCooperativeMatrixPropertiesNV* pProperties);

VkResult vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(VkPhysicalDevice physicalDevice,
                                                                           uint32_t* pCombinationCount,
                                                                           VkFramebufferMixedSamplesCombinationNV* pCombinations);

VkResult vkCreateHeadlessSurfaceEXT(VkInstance instance,
                                    const VkHeadlessSurfaceCreateInfoEXT* pCreateInfo,
                                    const VkAllocationCallbacks* pAllocator,
                                    VkSurfaceKHR* pSurface);

void vkCmdSetLineStippleEXT(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern);

void vkResetQueryPoolEXT(VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount);

void vkCmdSetCullModeEXT(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode);

void vkCmdSetFrontFaceEXT(VkCommandBuffer commandBuffer, VkFrontFace frontFace);

void vkCmdSetPrimitiveTopologyEXT(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology);

void vkCmdSetViewportWithCountEXT(VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports);

void vkCmdSetScissorWithCountEXT(VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors);

void vkCmdBindVertexBuffers2EXT(VkCommandBuffer commandBuffer,
                                uint32_t firstBinding,
                                uint32_t bindingCount,
                                const VkBuffer* pBuffers,
                                const VkDeviceSize* pOffsets,
                                const VkDeviceSize* pSizes,
                                const VkDeviceSize* pStrides);

void vkCmdSetDepthTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable);

void vkCmdSetDepthWriteEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable);

void vkCmdSetDepthCompareOpEXT(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp);

void vkCmdSetDepthBoundsTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable);

void vkCmdSetStencilTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable);

void vkCmdSetStencilOpEXT(VkCommandBuffer commandBuffer,
                          VkStencilFaceFlags faceMask,
                          VkStencilOp failOp,
                          VkStencilOp passOp,
                          VkStencilOp depthFailOp,
                          VkCompareOp compareOp);

void vkGetGeneratedCommandsMemoryRequirementsNV(const VkGeneratedCommandsMemoryRequirementsInfoNV* pInfo,
                                                VkMemoryRequirements2* pMemoryRequirements);

void vkCmdPreprocessGeneratedCommandsNV(VkCommandBuffer commandBuffer,
                                        const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo);

void vkCmdExecuteGeneratedCommandsNV(VkCommandBuffer commandBuffer,
                                     VkBool32 isPreprocessed,
                                     const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo);

void vkCmdBindPipelineShaderGroupNV(VkCommandBuffer commandBuffer,
                                    VkPipelineBindPoint pipelineBindPoint,
                                    VkPipeline pipeline,
                                    uint32_t groupIndex);

VkResult vkCreateIndirectCommandsLayoutNV(const VkIndirectCommandsLayoutCreateInfoNV* pCreateInfo,
                                          const VkAllocationCallbacks* pAllocator,
                                          VkIndirectCommandsLayoutNV* pIndirectCommandsLayout);

void vkDestroyIndirectCommandsLayoutNV(VkIndirectCommandsLayoutNV indirectCommandsLayout,
                                       const VkAllocationCallbacks* pAllocator);

VkResult vkAcquireDrmDisplayEXT(VkPhysicalDevice physicalDevice, int32_t drmFd, VkDisplayKHR display);

VkResult vkGetDrmDisplayEXT(VkPhysicalDevice physicalDevice,
                            int32_t drmFd,
                            uint32_t connectorId,
                            VkDisplayKHR* display);

VkResult vkCreatePrivateDataSlotEXT(const VkPrivateDataSlotCreateInfo* pCreateInfo,
                                    const VkAllocationCallbacks* pAllocator,
                                    VkPrivateDataSlot* pPrivateDataSlot);

void vkDestroyPrivateDataSlotEXT(VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator);

VkResult vkSetPrivateDataEXT(VkObjectType objectType,
                             uint64_t objectHandle,
                             VkPrivateDataSlot privateDataSlot,
                             uint64_t data);

void vkGetPrivateDataEXT(VkObjectType objectType,
                         uint64_t objectHandle,
                         VkPrivateDataSlot privateDataSlot,
                         uint64_t* pData);

void vkCmdSetFragmentShadingRateEnumNV(VkCommandBuffer commandBuffer,
                                       VkFragmentShadingRateNV shadingRate,
                                       const VkFragmentShadingRateCombinerOpKHR combinerOps[2]);

VkResult vkAcquireWinrtDisplayNV(VkPhysicalDevice physicalDevice, VkDisplayKHR display);

VkResult vkGetWinrtDisplayNV(VkPhysicalDevice physicalDevice, uint32_t deviceRelativeId, VkDisplayKHR* pDisplay);

void vkCmdSetVertexInputEXT(VkCommandBuffer commandBuffer,
                            uint32_t vertexBindingDescriptionCount,
                            const VkVertexInputBindingDescription2EXT* pVertexBindingDescriptions,
                            uint32_t vertexAttributeDescriptionCount,
                            const VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions);

VkResult vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI(VkRenderPass renderpass, VkExtent2D* pMaxWorkgroupSize);

void vkCmdSubpassShadingHUAWEI(VkCommandBuffer commandBuffer);

void vkCmdBindInvocationMaskHUAWEI(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout);

VkResult vkGetMemoryRemoteAddressNV(const VkMemoryGetRemoteAddressInfoNV* pMemoryGetRemoteAddressInfo,
                                    VkRemoteAddressNV* pAddress);

void vkCmdSetPatchControlPointsEXT(VkCommandBuffer commandBuffer, uint32_t patchControlPoints);

void vkCmdSetRasterizerDiscardEnableEXT(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable);

void vkCmdSetDepthBiasEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable);

void vkCmdSetLogicOpEXT(VkCommandBuffer commandBuffer, VkLogicOp logicOp);

void vkCmdSetPrimitiveRestartEnableEXT(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable);

void vkCmdSetColorWriteEnableEXT(VkCommandBuffer commandBuffer,
                                 uint32_t attachmentCount,
                                 const VkBool32* pColorWriteEnables);

void vkCmdDrawMultiEXT(VkCommandBuffer commandBuffer,
                       uint32_t drawCount,
                       const VkMultiDrawInfoEXT* pVertexInfo,
                       uint32_t instanceCount,
                       uint32_t firstInstance,
                       uint32_t stride);

void vkCmdDrawMultiIndexedEXT(VkCommandBuffer commandBuffer,
                              uint32_t drawCount,
                              const VkMultiDrawIndexedInfoEXT* pIndexInfo,
                              uint32_t instanceCount,
                              uint32_t firstInstance,
                              uint32_t stride,
                              const int32_t* pVertexOffset);

void vkSetDeviceMemoryPriorityEXT(VkDeviceMemory memory, float priority);

void vkGetDescriptorSetLayoutHostMappingInfoVALVE(const VkDescriptorSetBindingReferenceVALVE* pBindingReference,
                                                  VkDescriptorSetLayoutHostMappingInfoVALVE* pHostMapping);

void vkGetDescriptorSetHostMappingVALVE(VkDescriptorSet descriptorSet, void** ppData);

VkResult vkCreateAccelerationStructureKHR(const VkAccelerationStructureCreateInfoKHR* pCreateInfo,
                                          const VkAllocationCallbacks* pAllocator,
                                          VkAccelerationStructureKHR* pAccelerationStructure);

void vkDestroyAccelerationStructureKHR(VkAccelerationStructureKHR accelerationStructure,
                                       const VkAllocationCallbacks* pAllocator);

void vkCmdBuildAccelerationStructuresKHR(VkCommandBuffer commandBuffer,
                                         uint32_t infoCount,
                                         const VkAccelerationStructureBuildGeometryInfoKHR* pInfos,
                                         const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos);

void vkCmdBuildAccelerationStructuresIndirectKHR(VkCommandBuffer commandBuffer,
                                                 uint32_t infoCount,
                                                 const VkAccelerationStructureBuildGeometryInfoKHR* pInfos,
                                                 const VkDeviceAddress* pIndirectDeviceAddresses,
                                                 const uint32_t* pIndirectStrides,
                                                 const uint32_t* const* ppMaxPrimitiveCounts);

VkResult vkBuildAccelerationStructuresKHR(VkDeferredOperationKHR deferredOperation,
                                          uint32_t infoCount,
                                          const VkAccelerationStructureBuildGeometryInfoKHR* pInfos,
                                          const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos);

VkResult vkCopyAccelerationStructureKHR(VkDeferredOperationKHR deferredOperation,
                                        const VkCopyAccelerationStructureInfoKHR* pInfo);

VkResult vkCopyAccelerationStructureToMemoryKHR(VkDeferredOperationKHR deferredOperation,
                                                const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo);

VkResult vkCopyMemoryToAccelerationStructureKHR(VkDeferredOperationKHR deferredOperation,
                                                const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo);

VkResult vkWriteAccelerationStructuresPropertiesKHR(uint32_t accelerationStructureCount,
                                                    const VkAccelerationStructureKHR* pAccelerationStructures,
                                                    VkQueryType queryType,
                                                    size_t dataSize,
                                                    void* pData,
                                                    size_t stride);

void vkCmdCopyAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureInfoKHR* pInfo);

void vkCmdCopyAccelerationStructureToMemoryKHR(VkCommandBuffer commandBuffer,
                                               const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo);

void vkCmdCopyMemoryToAccelerationStructureKHR(VkCommandBuffer commandBuffer,
                                               const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo);

VkDeviceAddress vkGetAccelerationStructureDeviceAddressKHR(const VkAccelerationStructureDeviceAddressInfoKHR* pInfo);

void vkCmdWriteAccelerationStructuresPropertiesKHR(VkCommandBuffer commandBuffer,
                                                   uint32_t accelerationStructureCount,
                                                   const VkAccelerationStructureKHR* pAccelerationStructures,
                                                   VkQueryType queryType,
                                                   VkQueryPool queryPool,
                                                   uint32_t firstQuery);

void vkGetDeviceAccelerationStructureCompatibilityKHR(const VkAccelerationStructureVersionInfoKHR* pVersionInfo,
                                                      VkAccelerationStructureCompatibilityKHR* pCompatibility);

void vkGetAccelerationStructureBuildSizesKHR(VkAccelerationStructureBuildTypeKHR buildType,
                                             const VkAccelerationStructureBuildGeometryInfoKHR* pBuildInfo,
                                             const uint32_t* pMaxPrimitiveCounts,
                                             VkAccelerationStructureBuildSizesInfoKHR* pSizeInfo);

void vkCmdTraceRaysKHR(VkCommandBuffer commandBuffer,
                       const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable,
                       const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable,
                       const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable,
                       const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable,
                       uint32_t width,
                       uint32_t height,
                       uint32_t depth);

VkResult vkCreateRayTracingPipelinesKHR(VkDeferredOperationKHR deferredOperation,
                                        VkPipelineCache pipelineCache,
                                        uint32_t createInfoCount,
                                        const VkRayTracingPipelineCreateInfoKHR* pCreateInfos,
                                        const VkAllocationCallbacks* pAllocator,
                                        VkPipeline* pPipelines);

VkResult vkGetRayTracingCaptureReplayShaderGroupHandlesKHR(VkPipeline pipeline,
                                                           uint32_t firstGroup,
                                                           uint32_t groupCount,
                                                           size_t dataSize,
                                                           void* pData);

void vkCmdTraceRaysIndirectKHR(VkCommandBuffer commandBuffer,
                               const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable,
                               const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable,
                               const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable,
                               const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable,
                               VkDeviceAddress indirectDeviceAddress);

VkDeviceSize vkGetRayTracingShaderGroupStackSizeKHR(VkPipeline pipeline,
                                                    uint32_t group,
                                                    VkShaderGroupShaderKHR groupShader);

void vkCmdSetRayTracingPipelineStackSizeKHR(VkCommandBuffer commandBuffer, uint32_t pipelineStackSize);

// clang-format on

} // namespace plex::vkapi

#endif // PLEX_GRAPHICS_VULKAN_API_H
