#ifndef PLEX_GRAPHICS_VULKAN_API_H
#define PLEX_GRAPHICS_VULKAN_API_H

#include <vulkan/vulkan_core.h>

#include <vector>

namespace plex::vkapi
{

// clang-format off

template<typename T>
struct VulkanResultWithValue
{
    VkResult result { VK_SUCCESS };
    T value {};

    VulkanResultWithValue(T&& value = {}) : value(std::move(value)) {}

    operator bool() const
    {
        return result == VK_SUCCESS;
    }
};

struct VulkanResult
{
    VkResult result;

    VulkanResult(VkResult result) : result(result) {}

    operator bool() const
    {
        return result == VK_SUCCESS;
    }
};

struct VulkanFunctionPointer
{
    void (*callback)() {};

    VulkanFunctionPointer(void (*callback)()) : callback(callback) {}

    operator bool() const
    {
        return callback != nullptr;
    }
};

void InitVulkanApi(VkDevice device);

VulkanResult vkCreateInstance(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance);

void vkDestroyInstance(VkInstance instance, const VkAllocationCallbacks* pAllocator);

VulkanResultWithValue<std::vector<VkPhysicalDevice>> vkEnumeratePhysicalDevices(VkInstance instance);

void vkGetPhysicalDeviceFeatures(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures);

void vkGetPhysicalDeviceFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties* pFormatProperties);

VulkanResult vkGetPhysicalDeviceImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties);

void vkGetPhysicalDeviceProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties);

VulkanResultWithValue<std::vector<VkQueueFamilyProperties>> vkGetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice physicalDevice);

void vkGetPhysicalDeviceMemoryProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties);

VulkanFunctionPointer vkGetInstanceProcAddr(VkInstance instance, const char* pName);

VulkanFunctionPointer vkGetDeviceProcAddr(const char* pName);

VulkanResult vkCreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice);

void vkDestroyDevice(const VkAllocationCallbacks* pAllocator);

VulkanResultWithValue<std::vector<VkExtensionProperties>> vkEnumerateInstanceExtensionProperties(const char* pLayerName);

VulkanResultWithValue<std::vector<VkExtensionProperties>> vkEnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice, const char* pLayerName);

VulkanResult vkEnumerateInstanceLayerProperties(uint32_t* pPropertyCount, VkLayerProperties* pProperties);

VulkanResultWithValue<std::vector<VkLayerProperties>> vkEnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice);

void vkGetDeviceQueue(uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue);

VulkanResult vkQueueSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence);

VulkanResult vkQueueWaitIdle(VkQueue queue);

VulkanResult vkDeviceWaitIdle();

VulkanResult vkAllocateMemory(const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory);

void vkFreeMemory(VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator);

VulkanResult vkMapMemory(VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData);

void vkUnmapMemory(VkDeviceMemory memory);

VulkanResult vkFlushMappedMemoryRanges(uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges);

VulkanResult vkInvalidateMappedMemoryRanges(uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges);

void vkGetDeviceMemoryCommitment(VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes);

VulkanResult vkBindBufferMemory(VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset);

VulkanResult vkBindImageMemory(VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset);

void vkGetBufferMemoryRequirements(VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements);

void vkGetImageMemoryRequirements(VkImage image, VkMemoryRequirements* pMemoryRequirements);

void vkGetImageSparseMemoryRequirements(VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements);

VulkanResultWithValue<std::vector<VkSparseImageFormatProperties>> vkGetPhysicalDeviceSparseImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling);

VulkanResult vkQueueBindSparse(VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence);

VulkanResult vkCreateFence(const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence);

void vkDestroyFence(VkFence fence, const VkAllocationCallbacks* pAllocator);

VulkanResult vkResetFences(uint32_t fenceCount, const VkFence* pFences);

VulkanResult vkGetFenceStatus(VkFence fence);

VulkanResult vkWaitForFences(uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout);

VulkanResult vkCreateSemaphore(const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore);

void vkDestroySemaphore(VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator);

VulkanResult vkCreateEvent(const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent);

void vkDestroyEvent(VkEvent event, const VkAllocationCallbacks* pAllocator);

VulkanResult vkGetEventStatus(VkEvent event);

VulkanResult vkSetEvent(VkEvent event);

VulkanResult vkResetEvent(VkEvent event);

VulkanResult vkCreateQueryPool(const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool);

void vkDestroyQueryPool(VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator);

VulkanResult vkGetQueryPoolResults(VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags);

VulkanResult vkCreateBuffer(const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer);

void vkDestroyBuffer(VkBuffer buffer, const VkAllocationCallbacks* pAllocator);

VulkanResult vkCreateBufferView(const VkBufferViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferView* pView);

void vkDestroyBufferView(VkBufferView bufferView, const VkAllocationCallbacks* pAllocator);

VulkanResult vkCreateImage(const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage);

void vkDestroyImage(VkImage image, const VkAllocationCallbacks* pAllocator);

void vkGetImageSubresourceLayout(VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout);

VulkanResult vkCreateImageView(const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView);

void vkDestroyImageView(VkImageView imageView, const VkAllocationCallbacks* pAllocator);

VulkanResult vkCreateShaderModule(const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule);

void vkDestroyShaderModule(VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator);

VulkanResult vkCreatePipelineCache(const VkPipelineCacheCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache);

void vkDestroyPipelineCache(VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator);

VulkanResult vkGetPipelineCacheData(VkPipelineCache pipelineCache, size_t* pDataSize, void* pData);

VulkanResult vkMergePipelineCaches(VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches);

VulkanResult vkCreateGraphicsPipelines(VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);

VulkanResult vkCreateComputePipelines(VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);

void vkDestroyPipeline(VkPipeline pipeline, const VkAllocationCallbacks* pAllocator);

VulkanResult vkCreatePipelineLayout(const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout);

void vkDestroyPipelineLayout(VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator);

VulkanResult vkCreateSampler(const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler);

void vkDestroySampler(VkSampler sampler, const VkAllocationCallbacks* pAllocator);

VulkanResult vkCreateDescriptorSetLayout(const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout);

void vkDestroyDescriptorSetLayout(VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator);

VulkanResult vkCreateDescriptorPool(const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool);

void vkDestroyDescriptorPool(VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator);

VulkanResult vkResetDescriptorPool(VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags);

VulkanResult vkAllocateDescriptorSets(const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets);

VulkanResult vkFreeDescriptorSets(VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets);

void vkUpdateDescriptorSets(uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies);

VulkanResult vkCreateFramebuffer(const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer);

void vkDestroyFramebuffer(VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator);

VulkanResult vkCreateRenderPass(const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass);

void vkDestroyRenderPass(VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator);

void vkGetRenderAreaGranularity(VkRenderPass renderPass, VkExtent2D* pGranularity);

VulkanResult vkCreateCommandPool(const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool);

void vkDestroyCommandPool(VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator);

VulkanResult vkResetCommandPool(VkCommandPool commandPool, VkCommandPoolResetFlags flags);

VulkanResult vkAllocateCommandBuffers(const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers);

void vkFreeCommandBuffers(VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers);

VulkanResult vkEnumerateInstanceVersion(uint32_t* pApiVersion);

VulkanResult vkBindBufferMemory2(uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos);

VulkanResult vkBindImageMemory2(uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos);

void vkGetDeviceGroupPeerMemoryFeatures(uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures);

VulkanResultWithValue<std::vector<VkPhysicalDeviceGroupProperties>> vkEnumeratePhysicalDeviceGroups(VkInstance instance);

void vkGetImageMemoryRequirements2(const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements);

void vkGetBufferMemoryRequirements2(const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements);

void vkGetImageSparseMemoryRequirements2(const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements);

void vkGetPhysicalDeviceFeatures2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures);

void vkGetPhysicalDeviceProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties);

void vkGetPhysicalDeviceFormatProperties2(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties);

VulkanResult vkGetPhysicalDeviceImageFormatProperties2(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties);

VulkanResultWithValue<std::vector<VkQueueFamilyProperties2>> vkGetPhysicalDeviceQueueFamilyProperties2(VkPhysicalDevice physicalDevice);

void vkGetPhysicalDeviceMemoryProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties);

VulkanResultWithValue<std::vector<VkSparseImageFormatProperties2>> vkGetPhysicalDeviceSparseImageFormatProperties2(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo);

void vkTrimCommandPool(VkCommandPool commandPool, VkCommandPoolTrimFlags flags);

void vkGetDeviceQueue2(const VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue);

VulkanResult vkCreateSamplerYcbcrConversion(const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion);

void vkDestroySamplerYcbcrConversion(VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator);

VulkanResult vkCreateDescriptorUpdateTemplate(const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate);

void vkDestroyDescriptorUpdateTemplate(VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator);

void vkUpdateDescriptorSetWithTemplate(VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData);

void vkGetPhysicalDeviceExternalBufferProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties);

void vkGetPhysicalDeviceExternalFenceProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties);

void vkGetPhysicalDeviceExternalSemaphoreProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties);

void vkGetDescriptorSetLayoutSupport(const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport);

VulkanResult vkCreateRenderPass2(const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass);

void vkResetQueryPool(VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount);

VulkanResult vkGetSemaphoreCounterValue(VkSemaphore semaphore, uint64_t* pValue);

VulkanResult vkWaitSemaphores(const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout);

VulkanResult vkSignalSemaphore(const VkSemaphoreSignalInfo* pSignalInfo);

VulkanResultWithValue<VkDeviceAddress> vkGetBufferDeviceAddress(const VkBufferDeviceAddressInfo* pInfo);

VulkanResultWithValue<uint64_t> vkGetBufferOpaqueCaptureAddress(const VkBufferDeviceAddressInfo* pInfo);

VulkanResultWithValue<uint64_t> vkGetDeviceMemoryOpaqueCaptureAddress(const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo);

VulkanResultWithValue<std::vector<VkPhysicalDeviceToolProperties>> vkGetPhysicalDeviceToolProperties(VkPhysicalDevice physicalDevice);

VulkanResult vkCreatePrivateDataSlot(const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot);

void vkDestroyPrivateDataSlot(VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator);

VulkanResult vkSetPrivateData(VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data);

void vkGetPrivateData(VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData);

VulkanResult vkQueueSubmit2(VkQueue queue, uint32_t submitCount, const VkSubmitInfo2* pSubmits, VkFence fence);

void vkGetDeviceBufferMemoryRequirements(const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements);

void vkGetDeviceImageMemoryRequirements(const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements);

void vkGetDeviceImageSparseMemoryRequirements(const VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements);

void vkDestroySurfaceKHR(VkInstance instance, VkSurfaceKHR surface, const VkAllocationCallbacks* pAllocator);

VulkanResult vkGetPhysicalDeviceSurfaceSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported);

VulkanResult vkGetPhysicalDeviceSurfaceCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities);

VulkanResultWithValue<std::vector<VkSurfaceFormatKHR>> vkGetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

VulkanResultWithValue<std::vector<VkPresentModeKHR>> vkGetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

VulkanResult vkCreateSwapchainKHR(const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain);

void vkDestroySwapchainKHR(VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator);

VulkanResult vkGetSwapchainImagesKHR(VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages);

VulkanResult vkAcquireNextImageKHR(VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex);

VulkanResult vkQueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo);

VulkanResult vkGetDeviceGroupPresentCapabilitiesKHR(VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities);

VulkanResult vkGetDeviceGroupSurfacePresentModesKHR(VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes);

VulkanResultWithValue<std::vector<VkRect2D>> vkGetPhysicalDevicePresentRectanglesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

VulkanResult vkAcquireNextImage2KHR(const VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex);

VulkanResultWithValue<std::vector<VkDisplayPropertiesKHR>> vkGetPhysicalDeviceDisplayPropertiesKHR(VkPhysicalDevice physicalDevice);

VulkanResultWithValue<std::vector<VkDisplayPlanePropertiesKHR>> vkGetPhysicalDeviceDisplayPlanePropertiesKHR(VkPhysicalDevice physicalDevice);

VulkanResultWithValue<std::vector<VkDisplayKHR>> vkGetDisplayPlaneSupportedDisplaysKHR(VkPhysicalDevice physicalDevice, uint32_t planeIndex);

VulkanResultWithValue<std::vector<VkDisplayModePropertiesKHR>> vkGetDisplayModePropertiesKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display);

VulkanResult vkCreateDisplayModeKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, const VkDisplayModeCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDisplayModeKHR* pMode);

VulkanResult vkGetDisplayPlaneCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkDisplayModeKHR mode, uint32_t planeIndex, VkDisplayPlaneCapabilitiesKHR* pCapabilities);

VulkanResult vkCreateDisplayPlaneSurfaceKHR(VkInstance instance, const VkDisplaySurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);

VulkanResult vkCreateSharedSwapchainsKHR(uint32_t swapchainCount, const VkSwapchainCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchains);

void vkGetPhysicalDeviceFeatures2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures);

void vkGetPhysicalDeviceProperties2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties);

void vkGetPhysicalDeviceFormatProperties2KHR(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties);

VulkanResult vkGetPhysicalDeviceImageFormatProperties2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties);

VulkanResultWithValue<std::vector<VkQueueFamilyProperties2>> vkGetPhysicalDeviceQueueFamilyProperties2KHR(VkPhysicalDevice physicalDevice);

void vkGetPhysicalDeviceMemoryProperties2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties);

VulkanResultWithValue<std::vector<VkSparseImageFormatProperties2>> vkGetPhysicalDeviceSparseImageFormatProperties2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo);

void vkGetDeviceGroupPeerMemoryFeaturesKHR(uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures);

void vkTrimCommandPoolKHR(VkCommandPool commandPool, VkCommandPoolTrimFlags flags);

VulkanResultWithValue<std::vector<VkPhysicalDeviceGroupProperties>> vkEnumeratePhysicalDeviceGroupsKHR(VkInstance instance);

void vkGetPhysicalDeviceExternalBufferPropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties);

VulkanResult vkGetMemoryFdKHR(const VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd);

VulkanResult vkGetMemoryFdPropertiesKHR(VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties);

void vkGetPhysicalDeviceExternalSemaphorePropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties);

VulkanResult vkImportSemaphoreFdKHR(const VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo);

VulkanResult vkGetSemaphoreFdKHR(const VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd);

VulkanResult vkCreateDescriptorUpdateTemplateKHR(const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate);

void vkDestroyDescriptorUpdateTemplateKHR(VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator);

void vkUpdateDescriptorSetWithTemplateKHR(VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData);

VulkanResult vkCreateRenderPass2KHR(const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass);

VulkanResult vkGetSwapchainStatusKHR(VkSwapchainKHR swapchain);

void vkGetPhysicalDeviceExternalFencePropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties);

VulkanResult vkImportFenceFdKHR(const VkImportFenceFdInfoKHR* pImportFenceFdInfo);

VulkanResult vkGetFenceFdKHR(const VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd);

VulkanResult vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, uint32_t* pCounterCount, VkPerformanceCounterKHR* pCounters, VkPerformanceCounterDescriptionKHR* pCounterDescriptions);

void vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(VkPhysicalDevice physicalDevice, const VkQueryPoolPerformanceCreateInfoKHR* pPerformanceQueryCreateInfo, uint32_t* pNumPasses);

VulkanResult vkAcquireProfilingLockKHR(const VkAcquireProfilingLockInfoKHR* pInfo);

void vkReleaseProfilingLockKHR();

VulkanResult vkGetPhysicalDeviceSurfaceCapabilities2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkSurfaceCapabilities2KHR* pSurfaceCapabilities);

VulkanResultWithValue<std::vector<VkSurfaceFormat2KHR>> vkGetPhysicalDeviceSurfaceFormats2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo);

VulkanResultWithValue<std::vector<VkDisplayProperties2KHR>> vkGetPhysicalDeviceDisplayProperties2KHR(VkPhysicalDevice physicalDevice);

VulkanResultWithValue<std::vector<VkDisplayPlaneProperties2KHR>> vkGetPhysicalDeviceDisplayPlaneProperties2KHR(VkPhysicalDevice physicalDevice);

VulkanResultWithValue<std::vector<VkDisplayModeProperties2KHR>> vkGetDisplayModeProperties2KHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display);

VulkanResult vkGetDisplayPlaneCapabilities2KHR(VkPhysicalDevice physicalDevice, const VkDisplayPlaneInfo2KHR* pDisplayPlaneInfo, VkDisplayPlaneCapabilities2KHR* pCapabilities);

void vkGetImageMemoryRequirements2KHR(const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements);

void vkGetBufferMemoryRequirements2KHR(const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements);

void vkGetImageSparseMemoryRequirements2KHR(const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements);

VulkanResult vkCreateSamplerYcbcrConversionKHR(const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion);

void vkDestroySamplerYcbcrConversionKHR(VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator);

VulkanResult vkBindBufferMemory2KHR(uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos);

VulkanResult vkBindImageMemory2KHR(uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos);

void vkGetDescriptorSetLayoutSupportKHR(const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport);

VulkanResult vkGetSemaphoreCounterValueKHR(VkSemaphore semaphore, uint64_t* pValue);

VulkanResult vkWaitSemaphoresKHR(const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout);

VulkanResult vkSignalSemaphoreKHR(const VkSemaphoreSignalInfo* pSignalInfo);

VulkanResultWithValue<std::vector<VkPhysicalDeviceFragmentShadingRateKHR>> vkGetPhysicalDeviceFragmentShadingRatesKHR(VkPhysicalDevice physicalDevice);

VulkanResult vkWaitForPresentKHR(VkSwapchainKHR swapchain, uint64_t presentId, uint64_t timeout);

VulkanResultWithValue<VkDeviceAddress> vkGetBufferDeviceAddressKHR(const VkBufferDeviceAddressInfo* pInfo);

VulkanResultWithValue<uint64_t> vkGetBufferOpaqueCaptureAddressKHR(const VkBufferDeviceAddressInfo* pInfo);

VulkanResultWithValue<uint64_t> vkGetDeviceMemoryOpaqueCaptureAddressKHR(const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo);

VulkanResult vkCreateDeferredOperationKHR(const VkAllocationCallbacks* pAllocator, VkDeferredOperationKHR* pDeferredOperation);

void vkDestroyDeferredOperationKHR(VkDeferredOperationKHR operation, const VkAllocationCallbacks* pAllocator);

VulkanResultWithValue<uint32_t> vkGetDeferredOperationMaxConcurrencyKHR(VkDeferredOperationKHR operation);

VulkanResult vkGetDeferredOperationResultKHR(VkDeferredOperationKHR operation);

VulkanResult vkDeferredOperationJoinKHR(VkDeferredOperationKHR operation);

VulkanResult vkGetPipelineExecutablePropertiesKHR(const VkPipelineInfoKHR* pPipelineInfo, uint32_t* pExecutableCount, VkPipelineExecutablePropertiesKHR* pProperties);

VulkanResult vkGetPipelineExecutableStatisticsKHR(const VkPipelineExecutableInfoKHR* pExecutableInfo, uint32_t* pStatisticCount, VkPipelineExecutableStatisticKHR* pStatistics);

VulkanResult vkGetPipelineExecutableInternalRepresentationsKHR(const VkPipelineExecutableInfoKHR* pExecutableInfo, uint32_t* pInternalRepresentationCount, VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations);

VulkanResult vkQueueSubmit2KHR(VkQueue queue, uint32_t submitCount, const VkSubmitInfo2* pSubmits, VkFence fence);

VulkanResultWithValue<std::vector<VkCheckpointData2NV>> vkGetQueueCheckpointData2NV(VkQueue queue);

void vkGetDeviceBufferMemoryRequirementsKHR(const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements);

void vkGetDeviceImageMemoryRequirementsKHR(const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements);

void vkGetDeviceImageSparseMemoryRequirementsKHR(const VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements);

VulkanResult vkCreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);

void vkDestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);

void vkDebugReportMessageEXT(VkInstance instance, VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage);

VulkanResult vkDebugMarkerSetObjectTagEXT(const VkDebugMarkerObjectTagInfoEXT* pTagInfo);

VulkanResult vkDebugMarkerSetObjectNameEXT(const VkDebugMarkerObjectNameInfoEXT* pNameInfo);

VulkanResult vkCreateCuModuleNVX(const VkCuModuleCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuModuleNVX* pModule);

VulkanResult vkCreateCuFunctionNVX(const VkCuFunctionCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuFunctionNVX* pFunction);

void vkDestroyCuModuleNVX(VkCuModuleNVX module, const VkAllocationCallbacks* pAllocator);

void vkDestroyCuFunctionNVX(VkCuFunctionNVX function, const VkAllocationCallbacks* pAllocator);

VulkanResultWithValue<uint32_t> vkGetImageViewHandleNVX(const VkImageViewHandleInfoNVX* pInfo);

VulkanResult vkGetImageViewAddressNVX(VkImageView imageView, VkImageViewAddressPropertiesNVX* pProperties);

VulkanResult vkGetShaderInfoAMD(VkPipeline pipeline, VkShaderStageFlagBits shaderStage, VkShaderInfoTypeAMD infoType, size_t* pInfoSize, void* pInfo);

VulkanResult vkGetPhysicalDeviceExternalImageFormatPropertiesNV(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkExternalMemoryHandleTypeFlagsNV externalHandleType, VkExternalImageFormatPropertiesNV* pExternalImageFormatProperties);

VulkanResult vkReleaseDisplayEXT(VkPhysicalDevice physicalDevice, VkDisplayKHR display);

VulkanResult vkGetPhysicalDeviceSurfaceCapabilities2EXT(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilities2EXT* pSurfaceCapabilities);

VulkanResult vkDisplayPowerControlEXT(VkDisplayKHR display, const VkDisplayPowerInfoEXT* pDisplayPowerInfo);

VulkanResult vkRegisterDeviceEventEXT(const VkDeviceEventInfoEXT* pDeviceEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence);

VulkanResult vkRegisterDisplayEventEXT(VkDisplayKHR display, const VkDisplayEventInfoEXT* pDisplayEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence);

VulkanResult vkGetSwapchainCounterEXT(VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue);

VulkanResult vkGetRefreshCycleDurationGOOGLE(VkSwapchainKHR swapchain, VkRefreshCycleDurationGOOGLE* pDisplayTimingProperties);

VulkanResult vkGetPastPresentationTimingGOOGLE(VkSwapchainKHR swapchain, uint32_t* pPresentationTimingCount, VkPastPresentationTimingGOOGLE* pPresentationTimings);

void vkSetHdrMetadataEXT(uint32_t swapchainCount, const VkSwapchainKHR* pSwapchains, const VkHdrMetadataEXT* pMetadata);

VulkanResult vkSetDebugUtilsObjectNameEXT(const VkDebugUtilsObjectNameInfoEXT* pNameInfo);

VulkanResult vkSetDebugUtilsObjectTagEXT(const VkDebugUtilsObjectTagInfoEXT* pTagInfo);

void vkQueueBeginDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo);

void vkQueueEndDebugUtilsLabelEXT(VkQueue queue);

void vkQueueInsertDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo);

VulkanResult vkCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger);

void vkDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks* pAllocator);

void vkSubmitDebugUtilsMessageEXT(VkInstance instance, VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData);

void vkGetPhysicalDeviceMultisamplePropertiesEXT(VkPhysicalDevice physicalDevice, VkSampleCountFlagBits samples, VkMultisamplePropertiesEXT* pMultisampleProperties);

VulkanResult vkGetImageDrmFormatModifierPropertiesEXT(VkImage image, VkImageDrmFormatModifierPropertiesEXT* pProperties);

VulkanResult vkCreateValidationCacheEXT(const VkValidationCacheCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkValidationCacheEXT* pValidationCache);

void vkDestroyValidationCacheEXT(VkValidationCacheEXT validationCache, const VkAllocationCallbacks* pAllocator);

VulkanResult vkMergeValidationCachesEXT(VkValidationCacheEXT dstCache, uint32_t srcCacheCount, const VkValidationCacheEXT* pSrcCaches);

VulkanResult vkGetValidationCacheDataEXT(VkValidationCacheEXT validationCache, size_t* pDataSize, void* pData);

VulkanResult vkCreateAccelerationStructureNV(const VkAccelerationStructureCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkAccelerationStructureNV* pAccelerationStructure);

void vkDestroyAccelerationStructureNV(VkAccelerationStructureNV accelerationStructure, const VkAllocationCallbacks* pAllocator);

void vkGetAccelerationStructureMemoryRequirementsNV(const VkAccelerationStructureMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2KHR* pMemoryRequirements);

VulkanResult vkBindAccelerationStructureMemoryNV(uint32_t bindInfoCount, const VkBindAccelerationStructureMemoryInfoNV* pBindInfos);

VulkanResult vkCreateRayTracingPipelinesNV(VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoNV* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);

VulkanResult vkGetRayTracingShaderGroupHandlesKHR(VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData);

VulkanResult vkGetRayTracingShaderGroupHandlesNV(VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData);

VulkanResult vkGetAccelerationStructureHandleNV(VkAccelerationStructureNV accelerationStructure, size_t dataSize, void* pData);

VulkanResult vkCompileDeferredNV(VkPipeline pipeline, uint32_t shader);

VulkanResult vkGetMemoryHostPointerPropertiesEXT(VkExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties);

VulkanResultWithValue<std::vector<VkTimeDomainEXT>> vkGetPhysicalDeviceCalibrateableTimeDomainsEXT(VkPhysicalDevice physicalDevice);

VulkanResult vkGetCalibratedTimestampsEXT(uint32_t timestampCount, const VkCalibratedTimestampInfoEXT* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation);

VulkanResultWithValue<std::vector<VkCheckpointDataNV>> vkGetQueueCheckpointDataNV(VkQueue queue);

VulkanResult vkInitializePerformanceApiINTEL(const VkInitializePerformanceApiInfoINTEL* pInitializeInfo);

void vkUninitializePerformanceApiINTEL();

VulkanResult vkAcquirePerformanceConfigurationINTEL(const VkPerformanceConfigurationAcquireInfoINTEL* pAcquireInfo, VkPerformanceConfigurationINTEL* pConfiguration);

VulkanResult vkReleasePerformanceConfigurationINTEL(VkPerformanceConfigurationINTEL configuration);

VulkanResult vkQueueSetPerformanceConfigurationINTEL(VkQueue queue, VkPerformanceConfigurationINTEL configuration);

VulkanResult vkGetPerformanceParameterINTEL(VkPerformanceParameterTypeINTEL parameter, VkPerformanceValueINTEL* pValue);

void vkSetLocalDimmingAMD(VkSwapchainKHR swapChain, VkBool32 localDimmingEnable);

VulkanResultWithValue<VkDeviceAddress> vkGetBufferDeviceAddressEXT(const VkBufferDeviceAddressInfo* pInfo);

VulkanResultWithValue<std::vector<VkPhysicalDeviceToolProperties>> vkGetPhysicalDeviceToolPropertiesEXT(VkPhysicalDevice physicalDevice);

VulkanResultWithValue<std::vector<VkCooperativeMatrixPropertiesNV>> vkGetPhysicalDeviceCooperativeMatrixPropertiesNV(VkPhysicalDevice physicalDevice);

VulkanResultWithValue<std::vector<VkFramebufferMixedSamplesCombinationNV>> vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(VkPhysicalDevice physicalDevice);

VulkanResult vkCreateHeadlessSurfaceEXT(VkInstance instance, const VkHeadlessSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);

void vkResetQueryPoolEXT(VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount);

void vkGetGeneratedCommandsMemoryRequirementsNV(const VkGeneratedCommandsMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2* pMemoryRequirements);

VulkanResult vkCreateIndirectCommandsLayoutNV(const VkIndirectCommandsLayoutCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectCommandsLayoutNV* pIndirectCommandsLayout);

void vkDestroyIndirectCommandsLayoutNV(VkIndirectCommandsLayoutNV indirectCommandsLayout, const VkAllocationCallbacks* pAllocator);

VulkanResult vkAcquireDrmDisplayEXT(VkPhysicalDevice physicalDevice, int32_t drmFd, VkDisplayKHR display);

VulkanResult vkGetDrmDisplayEXT(VkPhysicalDevice physicalDevice, int32_t drmFd, uint32_t connectorId, VkDisplayKHR* display);

VulkanResult vkCreatePrivateDataSlotEXT(const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot);

void vkDestroyPrivateDataSlotEXT(VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator);

VulkanResult vkSetPrivateDataEXT(VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data);

void vkGetPrivateDataEXT(VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData);

VulkanResult vkAcquireWinrtDisplayNV(VkPhysicalDevice physicalDevice, VkDisplayKHR display);

VulkanResult vkGetWinrtDisplayNV(VkPhysicalDevice physicalDevice, uint32_t deviceRelativeId, VkDisplayKHR* pDisplay);

VulkanResult vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI(VkRenderPass renderpass, VkExtent2D* pMaxWorkgroupSize);

VulkanResult vkGetMemoryRemoteAddressNV(const VkMemoryGetRemoteAddressInfoNV* pMemoryGetRemoteAddressInfo, VkRemoteAddressNV* pAddress);

void vkSetDeviceMemoryPriorityEXT(VkDeviceMemory memory, float priority);

void vkGetDescriptorSetLayoutHostMappingInfoVALVE(const VkDescriptorSetBindingReferenceVALVE* pBindingReference, VkDescriptorSetLayoutHostMappingInfoVALVE* pHostMapping);

void vkGetDescriptorSetHostMappingVALVE(VkDescriptorSet descriptorSet, void** ppData);

VulkanResult vkCreateAccelerationStructureKHR(const VkAccelerationStructureCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkAccelerationStructureKHR* pAccelerationStructure);

void vkDestroyAccelerationStructureKHR(VkAccelerationStructureKHR accelerationStructure, const VkAllocationCallbacks* pAllocator);

VulkanResult vkBuildAccelerationStructuresKHR(VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos);

VulkanResult vkCopyAccelerationStructureKHR(VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureInfoKHR* pInfo);

VulkanResult vkCopyAccelerationStructureToMemoryKHR(VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo);

VulkanResult vkCopyMemoryToAccelerationStructureKHR(VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo);

VulkanResult vkWriteAccelerationStructuresPropertiesKHR(uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, size_t dataSize, void* pData, size_t stride);

VulkanResultWithValue<VkDeviceAddress> vkGetAccelerationStructureDeviceAddressKHR(const VkAccelerationStructureDeviceAddressInfoKHR* pInfo);

void vkGetDeviceAccelerationStructureCompatibilityKHR(const VkAccelerationStructureVersionInfoKHR* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility);

void vkGetAccelerationStructureBuildSizesKHR(VkAccelerationStructureBuildTypeKHR buildType, const VkAccelerationStructureBuildGeometryInfoKHR* pBuildInfo, const uint32_t* pMaxPrimitiveCounts, VkAccelerationStructureBuildSizesInfoKHR* pSizeInfo);

VulkanResult vkCreateRayTracingPipelinesKHR(VkDeferredOperationKHR deferredOperation, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);

VulkanResult vkGetRayTracingCaptureReplayShaderGroupHandlesKHR(VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData);

VulkanResultWithValue<VkDeviceSize> vkGetRayTracingShaderGroupStackSizeKHR(VkPipeline pipeline, uint32_t group, VkShaderGroupShaderKHR groupShader);

class VulkanCommandRecorder {
private:
    VkCommandBuffer _command_buffer { VK_NULL_HANDLE };

public:
    VulkanCommandRecorder(VkCommandBuffer command_buffer);

    VulkanResult vkBeginCommandBuffer(const VkCommandBufferBeginInfo* pBeginInfo) const noexcept;

    VulkanResult vkEndCommandBuffer() const noexcept;

    VulkanResult vkResetCommandBuffer(VkCommandBufferResetFlags flags) const noexcept;

    void vkCmdBindPipeline(VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline) const noexcept;

    void vkCmdSetViewport(uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports) const noexcept;

    void vkCmdSetScissor(uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors) const noexcept;

    void vkCmdSetLineWidth(float lineWidth) const noexcept;

    void vkCmdSetDepthBias(float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) const noexcept;

    void vkCmdSetBlendConstants(const float blendConstants[4]) const noexcept;

    void vkCmdSetDepthBounds(float minDepthBounds, float maxDepthBounds) const noexcept;

    void vkCmdSetStencilCompareMask(VkStencilFaceFlags faceMask, uint32_t compareMask) const noexcept;

    void vkCmdSetStencilWriteMask(VkStencilFaceFlags faceMask, uint32_t writeMask) const noexcept;

    void vkCmdSetStencilReference(VkStencilFaceFlags faceMask, uint32_t reference) const noexcept;

    void vkCmdBindDescriptorSets(VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets) const noexcept;

    void vkCmdBindIndexBuffer(VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) const noexcept;

    void vkCmdBindVertexBuffers(uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets) const noexcept;

    void vkCmdDraw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) const noexcept;

    void vkCmdDrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) const noexcept;

    void vkCmdDrawIndirect(VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) const noexcept;

    void vkCmdDrawIndexedIndirect(VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) const noexcept;

    void vkCmdDispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const noexcept;

    void vkCmdDispatchIndirect(VkBuffer buffer, VkDeviceSize offset) const noexcept;

    void vkCmdCopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions) const noexcept;

    void vkCmdCopyImage(VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions) const noexcept;

    void vkCmdBlitImage(VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter) const noexcept;

    void vkCmdCopyBufferToImage(VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions) const noexcept;

    void vkCmdCopyImageToBuffer(VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions) const noexcept;

    void vkCmdUpdateBuffer(VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData) const noexcept;

    void vkCmdFillBuffer(VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data) const noexcept;

    void vkCmdClearColorImage(VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) const noexcept;

    void vkCmdClearDepthStencilImage(VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) const noexcept;

    void vkCmdClearAttachments(uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects) const noexcept;

    void vkCmdResolveImage(VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions) const noexcept;

    void vkCmdSetEvent(VkEvent event, VkPipelineStageFlags stageMask) const noexcept;

    void vkCmdResetEvent(VkEvent event, VkPipelineStageFlags stageMask) const noexcept;

    void vkCmdWaitEvents(uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) const noexcept;

    void vkCmdPipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) const noexcept;

    void vkCmdBeginQuery(VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags) const noexcept;

    void vkCmdEndQuery(VkQueryPool queryPool, uint32_t query) const noexcept;

    void vkCmdResetQueryPool(VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) const noexcept;

    void vkCmdWriteTimestamp(VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query) const noexcept;

    void vkCmdCopyQueryPoolResults(VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags) const noexcept;

    void vkCmdPushConstants(VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues) const noexcept;

    void vkCmdBeginRenderPass(const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents) const noexcept;

    void vkCmdNextSubpass(VkSubpassContents contents) const noexcept;

    void vkCmdEndRenderPass() const noexcept;

    void vkCmdExecuteCommands(uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) const noexcept;

    void vkCmdSetDeviceMask(uint32_t deviceMask) const noexcept;

    void vkCmdDispatchBase(uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const noexcept;

    void vkCmdDrawIndirectCount(VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const noexcept;

    void vkCmdDrawIndexedIndirectCount(VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const noexcept;

    void vkCmdBeginRenderPass2(const VkRenderPassBeginInfo* pRenderPassBegin, const VkSubpassBeginInfo* pSubpassBeginInfo) const noexcept;

    void vkCmdNextSubpass2(const VkSubpassBeginInfo* pSubpassBeginInfo, const VkSubpassEndInfo* pSubpassEndInfo) const noexcept;

    void vkCmdEndRenderPass2(const VkSubpassEndInfo* pSubpassEndInfo) const noexcept;

    void vkCmdSetEvent2(VkEvent event, const VkDependencyInfo* pDependencyInfo) const noexcept;

    void vkCmdResetEvent2(VkEvent event, VkPipelineStageFlags2 stageMask) const noexcept;

    void vkCmdWaitEvents2(uint32_t eventCount, const VkEvent* pEvents, const VkDependencyInfo* pDependencyInfos) const noexcept;

    void vkCmdPipelineBarrier2(const VkDependencyInfo* pDependencyInfo) const noexcept;

    void vkCmdWriteTimestamp2(VkPipelineStageFlags2 stage, VkQueryPool queryPool, uint32_t query) const noexcept;

    void vkCmdCopyBuffer2(const VkCopyBufferInfo2* pCopyBufferInfo) const noexcept;

    void vkCmdCopyImage2(const VkCopyImageInfo2* pCopyImageInfo) const noexcept;

    void vkCmdCopyBufferToImage2(const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo) const noexcept;

    void vkCmdCopyImageToBuffer2(const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo) const noexcept;

    void vkCmdBlitImage2(const VkBlitImageInfo2* pBlitImageInfo) const noexcept;

    void vkCmdResolveImage2(const VkResolveImageInfo2* pResolveImageInfo) const noexcept;

    void vkCmdBeginRendering(const VkRenderingInfo* pRenderingInfo) const noexcept;

    void vkCmdEndRendering() const noexcept;

    void vkCmdSetCullMode(VkCullModeFlags cullMode) const noexcept;

    void vkCmdSetFrontFace(VkFrontFace frontFace) const noexcept;

    void vkCmdSetPrimitiveTopology(VkPrimitiveTopology primitiveTopology) const noexcept;

    void vkCmdSetViewportWithCount(uint32_t viewportCount, const VkViewport* pViewports) const noexcept;

    void vkCmdSetScissorWithCount(uint32_t scissorCount, const VkRect2D* pScissors) const noexcept;

    void vkCmdBindVertexBuffers2(uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides) const noexcept;

    void vkCmdSetDepthTestEnable(VkBool32 depthTestEnable) const noexcept;

    void vkCmdSetDepthWriteEnable(VkBool32 depthWriteEnable) const noexcept;

    void vkCmdSetDepthCompareOp(VkCompareOp depthCompareOp) const noexcept;

    void vkCmdSetDepthBoundsTestEnable(VkBool32 depthBoundsTestEnable) const noexcept;

    void vkCmdSetStencilTestEnable(VkBool32 stencilTestEnable) const noexcept;

    void vkCmdSetStencilOp(VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp) const noexcept;

    void vkCmdSetRasterizerDiscardEnable(VkBool32 rasterizerDiscardEnable) const noexcept;

    void vkCmdSetDepthBiasEnable(VkBool32 depthBiasEnable) const noexcept;

    void vkCmdSetPrimitiveRestartEnable(VkBool32 primitiveRestartEnable) const noexcept;

    void vkCmdBeginRenderingKHR(const VkRenderingInfo* pRenderingInfo) const noexcept;

    void vkCmdEndRenderingKHR() const noexcept;

    void vkCmdSetDeviceMaskKHR(uint32_t deviceMask) const noexcept;

    void vkCmdDispatchBaseKHR(uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const noexcept;

    void vkCmdPushDescriptorSetKHR(VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites) const noexcept;

    void vkCmdPushDescriptorSetWithTemplateKHR(VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData) const noexcept;

    void vkCmdBeginRenderPass2KHR(const VkRenderPassBeginInfo* pRenderPassBegin, const VkSubpassBeginInfo* pSubpassBeginInfo) const noexcept;

    void vkCmdNextSubpass2KHR(const VkSubpassBeginInfo* pSubpassBeginInfo, const VkSubpassEndInfo* pSubpassEndInfo) const noexcept;

    void vkCmdEndRenderPass2KHR(const VkSubpassEndInfo* pSubpassEndInfo) const noexcept;

    void vkCmdDrawIndirectCountKHR(VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const noexcept;

    void vkCmdDrawIndexedIndirectCountKHR(VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const noexcept;

    void vkCmdSetFragmentShadingRateKHR(const VkExtent2D* pFragmentSize, const VkFragmentShadingRateCombinerOpKHR combinerOps[2]) const noexcept;

    void vkCmdSetEvent2KHR(VkEvent event, const VkDependencyInfo* pDependencyInfo) const noexcept;

    void vkCmdResetEvent2KHR(VkEvent event, VkPipelineStageFlags2 stageMask) const noexcept;

    void vkCmdWaitEvents2KHR(uint32_t eventCount, const VkEvent* pEvents, const VkDependencyInfo* pDependencyInfos) const noexcept;

    void vkCmdPipelineBarrier2KHR(const VkDependencyInfo* pDependencyInfo) const noexcept;

    void vkCmdWriteTimestamp2KHR(VkPipelineStageFlags2 stage, VkQueryPool queryPool, uint32_t query) const noexcept;

    void vkCmdWriteBufferMarker2AMD(VkPipelineStageFlags2 stage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker) const noexcept;

    void vkCmdCopyBuffer2KHR(const VkCopyBufferInfo2* pCopyBufferInfo) const noexcept;

    void vkCmdCopyImage2KHR(const VkCopyImageInfo2* pCopyImageInfo) const noexcept;

    void vkCmdCopyBufferToImage2KHR(const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo) const noexcept;

    void vkCmdCopyImageToBuffer2KHR(const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo) const noexcept;

    void vkCmdBlitImage2KHR(const VkBlitImageInfo2* pBlitImageInfo) const noexcept;

    void vkCmdResolveImage2KHR(const VkResolveImageInfo2* pResolveImageInfo) const noexcept;

    void vkCmdDebugMarkerBeginEXT(const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) const noexcept;

    void vkCmdDebugMarkerEndEXT() const noexcept;

    void vkCmdDebugMarkerInsertEXT(const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) const noexcept;

    void vkCmdBindTransformFeedbackBuffersEXT(uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes) const noexcept;

    void vkCmdBeginTransformFeedbackEXT(uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) const noexcept;

    void vkCmdEndTransformFeedbackEXT(uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) const noexcept;

    void vkCmdBeginQueryIndexedEXT(VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index) const noexcept;

    void vkCmdEndQueryIndexedEXT(VkQueryPool queryPool, uint32_t query, uint32_t index) const noexcept;

    void vkCmdDrawIndirectByteCountEXT(uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride) const noexcept;

    void vkCmdCuLaunchKernelNVX(const VkCuLaunchInfoNVX* pLaunchInfo) const noexcept;

    void vkCmdDrawIndirectCountAMD(VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const noexcept;

    void vkCmdDrawIndexedIndirectCountAMD(VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const noexcept;

    void vkCmdBeginConditionalRenderingEXT(const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin) const noexcept;

    void vkCmdEndConditionalRenderingEXT() const noexcept;

    void vkCmdSetViewportWScalingNV(uint32_t firstViewport, uint32_t viewportCount, const VkViewportWScalingNV* pViewportWScalings) const noexcept;

    void vkCmdSetDiscardRectangleEXT(uint32_t firstDiscardRectangle, uint32_t discardRectangleCount, const VkRect2D* pDiscardRectangles) const noexcept;

    void vkCmdBeginDebugUtilsLabelEXT(const VkDebugUtilsLabelEXT* pLabelInfo) const noexcept;

    void vkCmdEndDebugUtilsLabelEXT() const noexcept;

    void vkCmdInsertDebugUtilsLabelEXT(const VkDebugUtilsLabelEXT* pLabelInfo) const noexcept;

    void vkCmdSetSampleLocationsEXT(const VkSampleLocationsInfoEXT* pSampleLocationsInfo) const noexcept;

    void vkCmdBindShadingRateImageNV(VkImageView imageView, VkImageLayout imageLayout) const noexcept;

    void vkCmdSetViewportShadingRatePaletteNV(uint32_t firstViewport, uint32_t viewportCount, const VkShadingRatePaletteNV* pShadingRatePalettes) const noexcept;

    void vkCmdSetCoarseSampleOrderNV(VkCoarseSampleOrderTypeNV sampleOrderType, uint32_t customSampleOrderCount, const VkCoarseSampleOrderCustomNV* pCustomSampleOrders) const noexcept;

    void vkCmdBuildAccelerationStructureNV(const VkAccelerationStructureInfoNV* pInfo, VkBuffer instanceData, VkDeviceSize instanceOffset, VkBool32 update, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkBuffer scratch, VkDeviceSize scratchOffset) const noexcept;

    void vkCmdCopyAccelerationStructureNV(VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkCopyAccelerationStructureModeKHR mode) const noexcept;

    void vkCmdTraceRaysNV(VkBuffer raygenShaderBindingTableBuffer, VkDeviceSize raygenShaderBindingOffset, VkBuffer missShaderBindingTableBuffer, VkDeviceSize missShaderBindingOffset, VkDeviceSize missShaderBindingStride, VkBuffer hitShaderBindingTableBuffer, VkDeviceSize hitShaderBindingOffset, VkDeviceSize hitShaderBindingStride, VkBuffer callableShaderBindingTableBuffer, VkDeviceSize callableShaderBindingOffset, VkDeviceSize callableShaderBindingStride, uint32_t width, uint32_t height, uint32_t depth) const noexcept;

    void vkCmdWriteAccelerationStructuresPropertiesNV(uint32_t accelerationStructureCount, const VkAccelerationStructureNV* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) const noexcept;

    void vkCmdWriteBufferMarkerAMD(VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker) const noexcept;

    void vkCmdDrawMeshTasksNV(uint32_t taskCount, uint32_t firstTask) const noexcept;

    void vkCmdDrawMeshTasksIndirectNV(VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) const noexcept;

    void vkCmdDrawMeshTasksIndirectCountNV(VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const noexcept;

    void vkCmdSetExclusiveScissorNV(uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkRect2D* pExclusiveScissors) const noexcept;

    void vkCmdSetCheckpointNV(const void* pCheckpointMarker) const noexcept;

    VulkanResult vkCmdSetPerformanceMarkerINTEL(const VkPerformanceMarkerInfoINTEL* pMarkerInfo) const noexcept;

    VulkanResult vkCmdSetPerformanceStreamMarkerINTEL(const VkPerformanceStreamMarkerInfoINTEL* pMarkerInfo) const noexcept;

    VulkanResult vkCmdSetPerformanceOverrideINTEL(const VkPerformanceOverrideInfoINTEL* pOverrideInfo) const noexcept;

    void vkCmdSetLineStippleEXT(uint32_t lineStippleFactor, uint16_t lineStipplePattern) const noexcept;

    void vkCmdSetCullModeEXT(VkCullModeFlags cullMode) const noexcept;

    void vkCmdSetFrontFaceEXT(VkFrontFace frontFace) const noexcept;

    void vkCmdSetPrimitiveTopologyEXT(VkPrimitiveTopology primitiveTopology) const noexcept;

    void vkCmdSetViewportWithCountEXT(uint32_t viewportCount, const VkViewport* pViewports) const noexcept;

    void vkCmdSetScissorWithCountEXT(uint32_t scissorCount, const VkRect2D* pScissors) const noexcept;

    void vkCmdBindVertexBuffers2EXT(uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides) const noexcept;

    void vkCmdSetDepthTestEnableEXT(VkBool32 depthTestEnable) const noexcept;

    void vkCmdSetDepthWriteEnableEXT(VkBool32 depthWriteEnable) const noexcept;

    void vkCmdSetDepthCompareOpEXT(VkCompareOp depthCompareOp) const noexcept;

    void vkCmdSetDepthBoundsTestEnableEXT(VkBool32 depthBoundsTestEnable) const noexcept;

    void vkCmdSetStencilTestEnableEXT(VkBool32 stencilTestEnable) const noexcept;

    void vkCmdSetStencilOpEXT(VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp) const noexcept;

    void vkCmdPreprocessGeneratedCommandsNV(const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo) const noexcept;

    void vkCmdExecuteGeneratedCommandsNV(VkBool32 isPreprocessed, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo) const noexcept;

    void vkCmdBindPipelineShaderGroupNV(VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline, uint32_t groupIndex) const noexcept;

    void vkCmdSetFragmentShadingRateEnumNV(VkFragmentShadingRateNV shadingRate, const VkFragmentShadingRateCombinerOpKHR combinerOps[2]) const noexcept;

    void vkCmdSetVertexInputEXT(uint32_t vertexBindingDescriptionCount, const VkVertexInputBindingDescription2EXT* pVertexBindingDescriptions, uint32_t vertexAttributeDescriptionCount, const VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions) const noexcept;

    void vkCmdSubpassShadingHUAWEI() const noexcept;

    void vkCmdBindInvocationMaskHUAWEI(VkImageView imageView, VkImageLayout imageLayout) const noexcept;

    void vkCmdSetPatchControlPointsEXT(uint32_t patchControlPoints) const noexcept;

    void vkCmdSetRasterizerDiscardEnableEXT(VkBool32 rasterizerDiscardEnable) const noexcept;

    void vkCmdSetDepthBiasEnableEXT(VkBool32 depthBiasEnable) const noexcept;

    void vkCmdSetLogicOpEXT(VkLogicOp logicOp) const noexcept;

    void vkCmdSetPrimitiveRestartEnableEXT(VkBool32 primitiveRestartEnable) const noexcept;

    void vkCmdSetColorWriteEnableEXT(uint32_t attachmentCount, const VkBool32* pColorWriteEnables) const noexcept;

    void vkCmdDrawMultiEXT(uint32_t drawCount, const VkMultiDrawInfoEXT* pVertexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride) const noexcept;

    void vkCmdDrawMultiIndexedEXT(uint32_t drawCount, const VkMultiDrawIndexedInfoEXT* pIndexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride, const int32_t* pVertexOffset) const noexcept;

    void vkCmdBuildAccelerationStructuresKHR(uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos) const noexcept;

    void vkCmdBuildAccelerationStructuresIndirectKHR(uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkDeviceAddress* pIndirectDeviceAddresses, const uint32_t* pIndirectStrides, const uint32_t* const* ppMaxPrimitiveCounts) const noexcept;

    void vkCmdCopyAccelerationStructureKHR(const VkCopyAccelerationStructureInfoKHR* pInfo) const noexcept;

    void vkCmdCopyAccelerationStructureToMemoryKHR(const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo) const noexcept;

    void vkCmdCopyMemoryToAccelerationStructureKHR(const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo) const noexcept;

    void vkCmdWriteAccelerationStructuresPropertiesKHR(uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) const noexcept;

    void vkCmdTraceRaysKHR(const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, uint32_t width, uint32_t height, uint32_t depth) const noexcept;

    void vkCmdTraceRaysIndirectKHR(const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, VkDeviceAddress indirectDeviceAddress) const noexcept;

    void vkCmdSetRayTracingPipelineStackSizeKHR(uint32_t pipelineStackSize) const noexcept;

};

// clang-format on

} // namespace plex::vkapi

#endif // PLEX_GRAPHICS_VULKAN_API_H
