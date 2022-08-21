
#ifndef PLEX_GRAPHICS_VULKAN_FUNCTION_TABLE_H
#define PLEX_GRAPHICS_VULKAN_FUNCTION_TABLE_H

#define VK_NO_PROTOTYPES

// Definitions for WIN32 to avoid including windows.h
using DWORD = unsigned long;
using LPCWSTR = const wchar_t*;
using HANDLE = void*;
using HINSTANCE = struct HINSTANCE__*; // NOLINT
using HWND = struct HWND__*; // NOLINT
using HMONITOR = struct HMONITOR__*; // NOLINT
using SECURITY_ATTRIBUTES = struct _SECURITY_ATTRIBUTES; // NOLINT

#include "vulkan/vulkan_core.h"
#include "vulkan/vulkan_win32.h"

#include <string>
#include <tuple>
#include <vector>

// clang-format off

namespace plex::graphics::vkapi::loader
{

struct FunctionTable;

///
/// Get the global function table.
///
FunctionTable& GetFunctionTable() noexcept;

using FunctionGroupStartIndex = uint32_t;
using FunctionGroupCount = uint32_t;

///
/// Finds the index and the count of the function group for the given group name.
/// 
/// @param group_name The name of the function group. 
/// @return The start index and the count of the function group for the given function group name.
std::pair<FunctionGroupStartIndex, FunctionGroupCount> GetFunctionGroupLoadInfo(const std::string& group_name) noexcept;

///
/// Return the function name for the given index.
///
/// @param index The index of the function name.
/// @return The function name for the given index or nullptr if the index is out of bounds.
const char* GetFunctionName(uint32_t index) noexcept;

///
/// Set the list of extensions to load.
///
/// @param extensions The list of extensions to load.
void SetExtensions(const std::vector<const char*>& extensions);

///
/// Get the list of extensions to load.
///
/// @return The list of extensions to load.
const std::vector<const char*>& GetExtensions() noexcept;

struct FunctionTable
{
  PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr { nullptr };
  PFN_vkEnumerateInstanceVersion vkEnumerateInstanceVersion { nullptr };
  PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties { nullptr };
  PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties { nullptr };
  PFN_vkCreateInstance vkCreateInstance { nullptr };
  PFN_vkDestroyInstance vkDestroyInstance { nullptr };
  PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices { nullptr };
  PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures { nullptr };
  PFN_vkGetPhysicalDeviceFormatProperties vkGetPhysicalDeviceFormatProperties { nullptr };
  PFN_vkGetPhysicalDeviceImageFormatProperties vkGetPhysicalDeviceImageFormatProperties { nullptr };
  PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties { nullptr };
  PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties { nullptr };
  PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties { nullptr };
  PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr { nullptr };
  PFN_vkCreateDevice vkCreateDevice { nullptr };
  PFN_vkDestroyDevice vkDestroyDevice { nullptr };
  PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties { nullptr };
  PFN_vkEnumerateDeviceLayerProperties vkEnumerateDeviceLayerProperties { nullptr };
  PFN_vkGetDeviceQueue vkGetDeviceQueue { nullptr };
  PFN_vkQueueSubmit vkQueueSubmit { nullptr };
  PFN_vkQueueWaitIdle vkQueueWaitIdle { nullptr };
  PFN_vkDeviceWaitIdle vkDeviceWaitIdle { nullptr };
  PFN_vkAllocateMemory vkAllocateMemory { nullptr };
  PFN_vkFreeMemory vkFreeMemory { nullptr };
  PFN_vkMapMemory vkMapMemory { nullptr };
  PFN_vkUnmapMemory vkUnmapMemory { nullptr };
  PFN_vkFlushMappedMemoryRanges vkFlushMappedMemoryRanges { nullptr };
  PFN_vkInvalidateMappedMemoryRanges vkInvalidateMappedMemoryRanges { nullptr };
  PFN_vkGetDeviceMemoryCommitment vkGetDeviceMemoryCommitment { nullptr };
  PFN_vkBindBufferMemory vkBindBufferMemory { nullptr };
  PFN_vkBindImageMemory vkBindImageMemory { nullptr };
  PFN_vkGetBufferMemoryRequirements vkGetBufferMemoryRequirements { nullptr };
  PFN_vkGetImageMemoryRequirements vkGetImageMemoryRequirements { nullptr };
  PFN_vkGetImageSparseMemoryRequirements vkGetImageSparseMemoryRequirements { nullptr };
  PFN_vkGetPhysicalDeviceSparseImageFormatProperties vkGetPhysicalDeviceSparseImageFormatProperties { nullptr };
  PFN_vkQueueBindSparse vkQueueBindSparse { nullptr };
  PFN_vkCreateFence vkCreateFence { nullptr };
  PFN_vkDestroyFence vkDestroyFence { nullptr };
  PFN_vkResetFences vkResetFences { nullptr };
  PFN_vkGetFenceStatus vkGetFenceStatus { nullptr };
  PFN_vkWaitForFences vkWaitForFences { nullptr };
  PFN_vkCreateSemaphore vkCreateSemaphore { nullptr };
  PFN_vkDestroySemaphore vkDestroySemaphore { nullptr };
  PFN_vkCreateEvent vkCreateEvent { nullptr };
  PFN_vkDestroyEvent vkDestroyEvent { nullptr };
  PFN_vkGetEventStatus vkGetEventStatus { nullptr };
  PFN_vkSetEvent vkSetEvent { nullptr };
  PFN_vkResetEvent vkResetEvent { nullptr };
  PFN_vkCreateQueryPool vkCreateQueryPool { nullptr };
  PFN_vkDestroyQueryPool vkDestroyQueryPool { nullptr };
  PFN_vkGetQueryPoolResults vkGetQueryPoolResults { nullptr };
  PFN_vkCreateBuffer vkCreateBuffer { nullptr };
  PFN_vkDestroyBuffer vkDestroyBuffer { nullptr };
  PFN_vkCreateBufferView vkCreateBufferView { nullptr };
  PFN_vkDestroyBufferView vkDestroyBufferView { nullptr };
  PFN_vkCreateImage vkCreateImage { nullptr };
  PFN_vkDestroyImage vkDestroyImage { nullptr };
  PFN_vkGetImageSubresourceLayout vkGetImageSubresourceLayout { nullptr };
  PFN_vkCreateImageView vkCreateImageView { nullptr };
  PFN_vkDestroyImageView vkDestroyImageView { nullptr };
  PFN_vkCreateShaderModule vkCreateShaderModule { nullptr };
  PFN_vkDestroyShaderModule vkDestroyShaderModule { nullptr };
  PFN_vkCreatePipelineCache vkCreatePipelineCache { nullptr };
  PFN_vkDestroyPipelineCache vkDestroyPipelineCache { nullptr };
  PFN_vkGetPipelineCacheData vkGetPipelineCacheData { nullptr };
  PFN_vkMergePipelineCaches vkMergePipelineCaches { nullptr };
  PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines { nullptr };
  PFN_vkCreateComputePipelines vkCreateComputePipelines { nullptr };
  PFN_vkDestroyPipeline vkDestroyPipeline { nullptr };
  PFN_vkCreatePipelineLayout vkCreatePipelineLayout { nullptr };
  PFN_vkDestroyPipelineLayout vkDestroyPipelineLayout { nullptr };
  PFN_vkCreateSampler vkCreateSampler { nullptr };
  PFN_vkDestroySampler vkDestroySampler { nullptr };
  PFN_vkCreateDescriptorSetLayout vkCreateDescriptorSetLayout { nullptr };
  PFN_vkDestroyDescriptorSetLayout vkDestroyDescriptorSetLayout { nullptr };
  PFN_vkCreateDescriptorPool vkCreateDescriptorPool { nullptr };
  PFN_vkDestroyDescriptorPool vkDestroyDescriptorPool { nullptr };
  PFN_vkResetDescriptorPool vkResetDescriptorPool { nullptr };
  PFN_vkAllocateDescriptorSets vkAllocateDescriptorSets { nullptr };
  PFN_vkFreeDescriptorSets vkFreeDescriptorSets { nullptr };
  PFN_vkUpdateDescriptorSets vkUpdateDescriptorSets { nullptr };
  PFN_vkCreateFramebuffer vkCreateFramebuffer { nullptr };
  PFN_vkDestroyFramebuffer vkDestroyFramebuffer { nullptr };
  PFN_vkCreateRenderPass vkCreateRenderPass { nullptr };
  PFN_vkDestroyRenderPass vkDestroyRenderPass { nullptr };
  PFN_vkGetRenderAreaGranularity vkGetRenderAreaGranularity { nullptr };
  PFN_vkCreateCommandPool vkCreateCommandPool { nullptr };
  PFN_vkDestroyCommandPool vkDestroyCommandPool { nullptr };
  PFN_vkResetCommandPool vkResetCommandPool { nullptr };
  PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers { nullptr };
  PFN_vkFreeCommandBuffers vkFreeCommandBuffers { nullptr };
  PFN_vkBeginCommandBuffer vkBeginCommandBuffer { nullptr };
  PFN_vkEndCommandBuffer vkEndCommandBuffer { nullptr };
  PFN_vkResetCommandBuffer vkResetCommandBuffer { nullptr };
  PFN_vkCmdBindPipeline vkCmdBindPipeline { nullptr };
  PFN_vkCmdSetViewport vkCmdSetViewport { nullptr };
  PFN_vkCmdSetScissor vkCmdSetScissor { nullptr };
  PFN_vkCmdSetLineWidth vkCmdSetLineWidth { nullptr };
  PFN_vkCmdSetDepthBias vkCmdSetDepthBias { nullptr };
  PFN_vkCmdSetBlendConstants vkCmdSetBlendConstants { nullptr };
  PFN_vkCmdSetDepthBounds vkCmdSetDepthBounds { nullptr };
  PFN_vkCmdSetStencilCompareMask vkCmdSetStencilCompareMask { nullptr };
  PFN_vkCmdSetStencilWriteMask vkCmdSetStencilWriteMask { nullptr };
  PFN_vkCmdSetStencilReference vkCmdSetStencilReference { nullptr };
  PFN_vkCmdBindDescriptorSets vkCmdBindDescriptorSets { nullptr };
  PFN_vkCmdBindIndexBuffer vkCmdBindIndexBuffer { nullptr };
  PFN_vkCmdBindVertexBuffers vkCmdBindVertexBuffers { nullptr };
  PFN_vkCmdDraw vkCmdDraw { nullptr };
  PFN_vkCmdDrawIndexed vkCmdDrawIndexed { nullptr };
  PFN_vkCmdDrawIndirect vkCmdDrawIndirect { nullptr };
  PFN_vkCmdDrawIndexedIndirect vkCmdDrawIndexedIndirect { nullptr };
  PFN_vkCmdDispatch vkCmdDispatch { nullptr };
  PFN_vkCmdDispatchIndirect vkCmdDispatchIndirect { nullptr };
  PFN_vkCmdCopyBuffer vkCmdCopyBuffer { nullptr };
  PFN_vkCmdCopyImage vkCmdCopyImage { nullptr };
  PFN_vkCmdBlitImage vkCmdBlitImage { nullptr };
  PFN_vkCmdCopyBufferToImage vkCmdCopyBufferToImage { nullptr };
  PFN_vkCmdCopyImageToBuffer vkCmdCopyImageToBuffer { nullptr };
  PFN_vkCmdUpdateBuffer vkCmdUpdateBuffer { nullptr };
  PFN_vkCmdFillBuffer vkCmdFillBuffer { nullptr };
  PFN_vkCmdClearColorImage vkCmdClearColorImage { nullptr };
  PFN_vkCmdClearDepthStencilImage vkCmdClearDepthStencilImage { nullptr };
  PFN_vkCmdClearAttachments vkCmdClearAttachments { nullptr };
  PFN_vkCmdResolveImage vkCmdResolveImage { nullptr };
  PFN_vkCmdSetEvent vkCmdSetEvent { nullptr };
  PFN_vkCmdResetEvent vkCmdResetEvent { nullptr };
  PFN_vkCmdWaitEvents vkCmdWaitEvents { nullptr };
  PFN_vkCmdPipelineBarrier vkCmdPipelineBarrier { nullptr };
  PFN_vkCmdBeginQuery vkCmdBeginQuery { nullptr };
  PFN_vkCmdEndQuery vkCmdEndQuery { nullptr };
  PFN_vkCmdResetQueryPool vkCmdResetQueryPool { nullptr };
  PFN_vkCmdWriteTimestamp vkCmdWriteTimestamp { nullptr };
  PFN_vkCmdCopyQueryPoolResults vkCmdCopyQueryPoolResults { nullptr };
  PFN_vkCmdPushConstants vkCmdPushConstants { nullptr };
  PFN_vkCmdBeginRenderPass vkCmdBeginRenderPass { nullptr };
  PFN_vkCmdNextSubpass vkCmdNextSubpass { nullptr };
  PFN_vkCmdEndRenderPass vkCmdEndRenderPass { nullptr };
  PFN_vkCmdExecuteCommands vkCmdExecuteCommands { nullptr };
  PFN_vkBindBufferMemory2 vkBindBufferMemory2 { nullptr };
  PFN_vkBindImageMemory2 vkBindImageMemory2 { nullptr };
  PFN_vkGetDeviceGroupPeerMemoryFeatures vkGetDeviceGroupPeerMemoryFeatures { nullptr };
  PFN_vkCmdSetDeviceMask vkCmdSetDeviceMask { nullptr };
  PFN_vkCmdDispatchBase vkCmdDispatchBase { nullptr };
  PFN_vkEnumeratePhysicalDeviceGroups vkEnumeratePhysicalDeviceGroups { nullptr };
  PFN_vkGetImageMemoryRequirements2 vkGetImageMemoryRequirements2 { nullptr };
  PFN_vkGetBufferMemoryRequirements2 vkGetBufferMemoryRequirements2 { nullptr };
  PFN_vkGetImageSparseMemoryRequirements2 vkGetImageSparseMemoryRequirements2 { nullptr };
  PFN_vkGetPhysicalDeviceFeatures2 vkGetPhysicalDeviceFeatures2 { nullptr };
  PFN_vkGetPhysicalDeviceProperties2 vkGetPhysicalDeviceProperties2 { nullptr };
  PFN_vkGetPhysicalDeviceFormatProperties2 vkGetPhysicalDeviceFormatProperties2 { nullptr };
  PFN_vkGetPhysicalDeviceImageFormatProperties2 vkGetPhysicalDeviceImageFormatProperties2 { nullptr };
  PFN_vkGetPhysicalDeviceQueueFamilyProperties2 vkGetPhysicalDeviceQueueFamilyProperties2 { nullptr };
  PFN_vkGetPhysicalDeviceMemoryProperties2 vkGetPhysicalDeviceMemoryProperties2 { nullptr };
  PFN_vkGetPhysicalDeviceSparseImageFormatProperties2 vkGetPhysicalDeviceSparseImageFormatProperties2 { nullptr };
  PFN_vkTrimCommandPool vkTrimCommandPool { nullptr };
  PFN_vkGetDeviceQueue2 vkGetDeviceQueue2 { nullptr };
  PFN_vkCreateSamplerYcbcrConversion vkCreateSamplerYcbcrConversion { nullptr };
  PFN_vkDestroySamplerYcbcrConversion vkDestroySamplerYcbcrConversion { nullptr };
  PFN_vkCreateDescriptorUpdateTemplate vkCreateDescriptorUpdateTemplate { nullptr };
  PFN_vkDestroyDescriptorUpdateTemplate vkDestroyDescriptorUpdateTemplate { nullptr };
  PFN_vkUpdateDescriptorSetWithTemplate vkUpdateDescriptorSetWithTemplate { nullptr };
  PFN_vkGetPhysicalDeviceExternalBufferProperties vkGetPhysicalDeviceExternalBufferProperties { nullptr };
  PFN_vkGetPhysicalDeviceExternalFenceProperties vkGetPhysicalDeviceExternalFenceProperties { nullptr };
  PFN_vkGetPhysicalDeviceExternalSemaphoreProperties vkGetPhysicalDeviceExternalSemaphoreProperties { nullptr };
  PFN_vkGetDescriptorSetLayoutSupport vkGetDescriptorSetLayoutSupport { nullptr };
  PFN_vkCmdDrawIndirectCount vkCmdDrawIndirectCount { nullptr };
  PFN_vkCmdDrawIndexedIndirectCount vkCmdDrawIndexedIndirectCount { nullptr };
  PFN_vkCreateRenderPass2 vkCreateRenderPass2 { nullptr };
  PFN_vkCmdBeginRenderPass2 vkCmdBeginRenderPass2 { nullptr };
  PFN_vkCmdNextSubpass2 vkCmdNextSubpass2 { nullptr };
  PFN_vkCmdEndRenderPass2 vkCmdEndRenderPass2 { nullptr };
  PFN_vkResetQueryPool vkResetQueryPool { nullptr };
  PFN_vkGetSemaphoreCounterValue vkGetSemaphoreCounterValue { nullptr };
  PFN_vkWaitSemaphores vkWaitSemaphores { nullptr };
  PFN_vkSignalSemaphore vkSignalSemaphore { nullptr };
  PFN_vkGetBufferDeviceAddress vkGetBufferDeviceAddress { nullptr };
  PFN_vkGetBufferOpaqueCaptureAddress vkGetBufferOpaqueCaptureAddress { nullptr };
  PFN_vkGetDeviceMemoryOpaqueCaptureAddress vkGetDeviceMemoryOpaqueCaptureAddress { nullptr };
  PFN_vkGetPhysicalDeviceToolProperties vkGetPhysicalDeviceToolProperties { nullptr };
  PFN_vkCreatePrivateDataSlot vkCreatePrivateDataSlot { nullptr };
  PFN_vkDestroyPrivateDataSlot vkDestroyPrivateDataSlot { nullptr };
  PFN_vkSetPrivateData vkSetPrivateData { nullptr };
  PFN_vkGetPrivateData vkGetPrivateData { nullptr };
  PFN_vkCmdSetEvent2 vkCmdSetEvent2 { nullptr };
  PFN_vkCmdResetEvent2 vkCmdResetEvent2 { nullptr };
  PFN_vkCmdWaitEvents2 vkCmdWaitEvents2 { nullptr };
  PFN_vkCmdPipelineBarrier2 vkCmdPipelineBarrier2 { nullptr };
  PFN_vkCmdWriteTimestamp2 vkCmdWriteTimestamp2 { nullptr };
  PFN_vkQueueSubmit2 vkQueueSubmit2 { nullptr };
  PFN_vkCmdCopyBuffer2 vkCmdCopyBuffer2 { nullptr };
  PFN_vkCmdCopyImage2 vkCmdCopyImage2 { nullptr };
  PFN_vkCmdCopyBufferToImage2 vkCmdCopyBufferToImage2 { nullptr };
  PFN_vkCmdCopyImageToBuffer2 vkCmdCopyImageToBuffer2 { nullptr };
  PFN_vkCmdBlitImage2 vkCmdBlitImage2 { nullptr };
  PFN_vkCmdResolveImage2 vkCmdResolveImage2 { nullptr };
  PFN_vkCmdBeginRendering vkCmdBeginRendering { nullptr };
  PFN_vkCmdEndRendering vkCmdEndRendering { nullptr };
  PFN_vkCmdSetCullMode vkCmdSetCullMode { nullptr };
  PFN_vkCmdSetFrontFace vkCmdSetFrontFace { nullptr };
  PFN_vkCmdSetPrimitiveTopology vkCmdSetPrimitiveTopology { nullptr };
  PFN_vkCmdSetViewportWithCount vkCmdSetViewportWithCount { nullptr };
  PFN_vkCmdSetScissorWithCount vkCmdSetScissorWithCount { nullptr };
  PFN_vkCmdBindVertexBuffers2 vkCmdBindVertexBuffers2 { nullptr };
  PFN_vkCmdSetDepthTestEnable vkCmdSetDepthTestEnable { nullptr };
  PFN_vkCmdSetDepthWriteEnable vkCmdSetDepthWriteEnable { nullptr };
  PFN_vkCmdSetDepthCompareOp vkCmdSetDepthCompareOp { nullptr };
  PFN_vkCmdSetDepthBoundsTestEnable vkCmdSetDepthBoundsTestEnable { nullptr };
  PFN_vkCmdSetStencilTestEnable vkCmdSetStencilTestEnable { nullptr };
  PFN_vkCmdSetStencilOp vkCmdSetStencilOp { nullptr };
  PFN_vkCmdSetRasterizerDiscardEnable vkCmdSetRasterizerDiscardEnable { nullptr };
  PFN_vkCmdSetDepthBiasEnable vkCmdSetDepthBiasEnable { nullptr };
  PFN_vkCmdSetPrimitiveRestartEnable vkCmdSetPrimitiveRestartEnable { nullptr };
  PFN_vkGetDeviceBufferMemoryRequirements vkGetDeviceBufferMemoryRequirements { nullptr };
  PFN_vkGetDeviceImageMemoryRequirements vkGetDeviceImageMemoryRequirements { nullptr };
  PFN_vkGetDeviceImageSparseMemoryRequirements vkGetDeviceImageSparseMemoryRequirements { nullptr };
  PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR { nullptr };
  PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR { nullptr };
  PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR { nullptr };
  PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR { nullptr };
  PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR { nullptr };
  PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR { nullptr };
  PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR { nullptr };
  PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR { nullptr };
  PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR { nullptr };
  PFN_vkQueuePresentKHR vkQueuePresentKHR { nullptr };
  PFN_vkGetDeviceGroupPresentCapabilitiesKHR vkGetDeviceGroupPresentCapabilitiesKHR { nullptr };
  PFN_vkGetDeviceGroupSurfacePresentModesKHR vkGetDeviceGroupSurfacePresentModesKHR { nullptr };
  PFN_vkGetPhysicalDevicePresentRectanglesKHR vkGetPhysicalDevicePresentRectanglesKHR { nullptr };
  PFN_vkAcquireNextImage2KHR vkAcquireNextImage2KHR { nullptr };
  PFN_vkGetPhysicalDeviceDisplayPropertiesKHR vkGetPhysicalDeviceDisplayPropertiesKHR { nullptr };
  PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR vkGetPhysicalDeviceDisplayPlanePropertiesKHR { nullptr };
  PFN_vkGetDisplayPlaneSupportedDisplaysKHR vkGetDisplayPlaneSupportedDisplaysKHR { nullptr };
  PFN_vkGetDisplayModePropertiesKHR vkGetDisplayModePropertiesKHR { nullptr };
  PFN_vkCreateDisplayModeKHR vkCreateDisplayModeKHR { nullptr };
  PFN_vkGetDisplayPlaneCapabilitiesKHR vkGetDisplayPlaneCapabilitiesKHR { nullptr };
  PFN_vkCreateDisplayPlaneSurfaceKHR vkCreateDisplayPlaneSurfaceKHR { nullptr };
  PFN_vkCreateSharedSwapchainsKHR vkCreateSharedSwapchainsKHR { nullptr };
  PFN_vkCmdBeginRenderingKHR vkCmdBeginRenderingKHR { nullptr };
  PFN_vkCmdEndRenderingKHR vkCmdEndRenderingKHR { nullptr };
  PFN_vkGetPhysicalDeviceFeatures2KHR vkGetPhysicalDeviceFeatures2KHR { nullptr };
  PFN_vkGetPhysicalDeviceProperties2KHR vkGetPhysicalDeviceProperties2KHR { nullptr };
  PFN_vkGetPhysicalDeviceFormatProperties2KHR vkGetPhysicalDeviceFormatProperties2KHR { nullptr };
  PFN_vkGetPhysicalDeviceImageFormatProperties2KHR vkGetPhysicalDeviceImageFormatProperties2KHR { nullptr };
  PFN_vkGetPhysicalDeviceQueueFamilyProperties2KHR vkGetPhysicalDeviceQueueFamilyProperties2KHR { nullptr };
  PFN_vkGetPhysicalDeviceMemoryProperties2KHR vkGetPhysicalDeviceMemoryProperties2KHR { nullptr };
  PFN_vkGetPhysicalDeviceSparseImageFormatProperties2KHR vkGetPhysicalDeviceSparseImageFormatProperties2KHR { nullptr };
  PFN_vkGetDeviceGroupPeerMemoryFeaturesKHR vkGetDeviceGroupPeerMemoryFeaturesKHR { nullptr };
  PFN_vkCmdSetDeviceMaskKHR vkCmdSetDeviceMaskKHR { nullptr };
  PFN_vkCmdDispatchBaseKHR vkCmdDispatchBaseKHR { nullptr };
  PFN_vkTrimCommandPoolKHR vkTrimCommandPoolKHR { nullptr };
  PFN_vkEnumeratePhysicalDeviceGroupsKHR vkEnumeratePhysicalDeviceGroupsKHR { nullptr };
  PFN_vkGetPhysicalDeviceExternalBufferPropertiesKHR vkGetPhysicalDeviceExternalBufferPropertiesKHR { nullptr };
  PFN_vkGetMemoryFdKHR vkGetMemoryFdKHR { nullptr };
  PFN_vkGetMemoryFdPropertiesKHR vkGetMemoryFdPropertiesKHR { nullptr };
  PFN_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR vkGetPhysicalDeviceExternalSemaphorePropertiesKHR { nullptr };
  PFN_vkImportSemaphoreFdKHR vkImportSemaphoreFdKHR { nullptr };
  PFN_vkGetSemaphoreFdKHR vkGetSemaphoreFdKHR { nullptr };
  PFN_vkCmdPushDescriptorSetKHR vkCmdPushDescriptorSetKHR { nullptr };
  PFN_vkCmdPushDescriptorSetWithTemplateKHR vkCmdPushDescriptorSetWithTemplateKHR { nullptr };
  PFN_vkCreateDescriptorUpdateTemplateKHR vkCreateDescriptorUpdateTemplateKHR { nullptr };
  PFN_vkDestroyDescriptorUpdateTemplateKHR vkDestroyDescriptorUpdateTemplateKHR { nullptr };
  PFN_vkUpdateDescriptorSetWithTemplateKHR vkUpdateDescriptorSetWithTemplateKHR { nullptr };
  PFN_vkCreateRenderPass2KHR vkCreateRenderPass2KHR { nullptr };
  PFN_vkCmdBeginRenderPass2KHR vkCmdBeginRenderPass2KHR { nullptr };
  PFN_vkCmdNextSubpass2KHR vkCmdNextSubpass2KHR { nullptr };
  PFN_vkCmdEndRenderPass2KHR vkCmdEndRenderPass2KHR { nullptr };
  PFN_vkGetSwapchainStatusKHR vkGetSwapchainStatusKHR { nullptr };
  PFN_vkGetPhysicalDeviceExternalFencePropertiesKHR vkGetPhysicalDeviceExternalFencePropertiesKHR { nullptr };
  PFN_vkImportFenceFdKHR vkImportFenceFdKHR { nullptr };
  PFN_vkGetFenceFdKHR vkGetFenceFdKHR { nullptr };
  PFN_vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR { nullptr };
  PFN_vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR { nullptr };
  PFN_vkAcquireProfilingLockKHR vkAcquireProfilingLockKHR { nullptr };
  PFN_vkReleaseProfilingLockKHR vkReleaseProfilingLockKHR { nullptr };
  PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR vkGetPhysicalDeviceSurfaceCapabilities2KHR { nullptr };
  PFN_vkGetPhysicalDeviceSurfaceFormats2KHR vkGetPhysicalDeviceSurfaceFormats2KHR { nullptr };
  PFN_vkGetPhysicalDeviceDisplayProperties2KHR vkGetPhysicalDeviceDisplayProperties2KHR { nullptr };
  PFN_vkGetPhysicalDeviceDisplayPlaneProperties2KHR vkGetPhysicalDeviceDisplayPlaneProperties2KHR { nullptr };
  PFN_vkGetDisplayModeProperties2KHR vkGetDisplayModeProperties2KHR { nullptr };
  PFN_vkGetDisplayPlaneCapabilities2KHR vkGetDisplayPlaneCapabilities2KHR { nullptr };
  PFN_vkGetImageMemoryRequirements2KHR vkGetImageMemoryRequirements2KHR { nullptr };
  PFN_vkGetBufferMemoryRequirements2KHR vkGetBufferMemoryRequirements2KHR { nullptr };
  PFN_vkGetImageSparseMemoryRequirements2KHR vkGetImageSparseMemoryRequirements2KHR { nullptr };
  PFN_vkCreateSamplerYcbcrConversionKHR vkCreateSamplerYcbcrConversionKHR { nullptr };
  PFN_vkDestroySamplerYcbcrConversionKHR vkDestroySamplerYcbcrConversionKHR { nullptr };
  PFN_vkBindBufferMemory2KHR vkBindBufferMemory2KHR { nullptr };
  PFN_vkBindImageMemory2KHR vkBindImageMemory2KHR { nullptr };
  PFN_vkGetDescriptorSetLayoutSupportKHR vkGetDescriptorSetLayoutSupportKHR { nullptr };
  PFN_vkCmdDrawIndirectCountKHR vkCmdDrawIndirectCountKHR { nullptr };
  PFN_vkCmdDrawIndexedIndirectCountKHR vkCmdDrawIndexedIndirectCountKHR { nullptr };
  PFN_vkGetSemaphoreCounterValueKHR vkGetSemaphoreCounterValueKHR { nullptr };
  PFN_vkWaitSemaphoresKHR vkWaitSemaphoresKHR { nullptr };
  PFN_vkSignalSemaphoreKHR vkSignalSemaphoreKHR { nullptr };
  PFN_vkGetPhysicalDeviceFragmentShadingRatesKHR vkGetPhysicalDeviceFragmentShadingRatesKHR { nullptr };
  PFN_vkCmdSetFragmentShadingRateKHR vkCmdSetFragmentShadingRateKHR { nullptr };
  PFN_vkWaitForPresentKHR vkWaitForPresentKHR { nullptr };
  PFN_vkGetBufferDeviceAddressKHR vkGetBufferDeviceAddressKHR { nullptr };
  PFN_vkGetBufferOpaqueCaptureAddressKHR vkGetBufferOpaqueCaptureAddressKHR { nullptr };
  PFN_vkGetDeviceMemoryOpaqueCaptureAddressKHR vkGetDeviceMemoryOpaqueCaptureAddressKHR { nullptr };
  PFN_vkCreateDeferredOperationKHR vkCreateDeferredOperationKHR { nullptr };
  PFN_vkDestroyDeferredOperationKHR vkDestroyDeferredOperationKHR { nullptr };
  PFN_vkGetDeferredOperationMaxConcurrencyKHR vkGetDeferredOperationMaxConcurrencyKHR { nullptr };
  PFN_vkGetDeferredOperationResultKHR vkGetDeferredOperationResultKHR { nullptr };
  PFN_vkDeferredOperationJoinKHR vkDeferredOperationJoinKHR { nullptr };
  PFN_vkGetPipelineExecutablePropertiesKHR vkGetPipelineExecutablePropertiesKHR { nullptr };
  PFN_vkGetPipelineExecutableStatisticsKHR vkGetPipelineExecutableStatisticsKHR { nullptr };
  PFN_vkGetPipelineExecutableInternalRepresentationsKHR vkGetPipelineExecutableInternalRepresentationsKHR { nullptr };
  PFN_vkCmdSetEvent2KHR vkCmdSetEvent2KHR { nullptr };
  PFN_vkCmdResetEvent2KHR vkCmdResetEvent2KHR { nullptr };
  PFN_vkCmdWaitEvents2KHR vkCmdWaitEvents2KHR { nullptr };
  PFN_vkCmdPipelineBarrier2KHR vkCmdPipelineBarrier2KHR { nullptr };
  PFN_vkCmdWriteTimestamp2KHR vkCmdWriteTimestamp2KHR { nullptr };
  PFN_vkQueueSubmit2KHR vkQueueSubmit2KHR { nullptr };
  PFN_vkCmdWriteBufferMarker2AMD vkCmdWriteBufferMarker2AMD { nullptr };
  PFN_vkGetQueueCheckpointData2NV vkGetQueueCheckpointData2NV { nullptr };
  PFN_vkCmdCopyBuffer2KHR vkCmdCopyBuffer2KHR { nullptr };
  PFN_vkCmdCopyImage2KHR vkCmdCopyImage2KHR { nullptr };
  PFN_vkCmdCopyBufferToImage2KHR vkCmdCopyBufferToImage2KHR { nullptr };
  PFN_vkCmdCopyImageToBuffer2KHR vkCmdCopyImageToBuffer2KHR { nullptr };
  PFN_vkCmdBlitImage2KHR vkCmdBlitImage2KHR { nullptr };
  PFN_vkCmdResolveImage2KHR vkCmdResolveImage2KHR { nullptr };
  PFN_vkGetDeviceBufferMemoryRequirementsKHR vkGetDeviceBufferMemoryRequirementsKHR { nullptr };
  PFN_vkGetDeviceImageMemoryRequirementsKHR vkGetDeviceImageMemoryRequirementsKHR { nullptr };
  PFN_vkGetDeviceImageSparseMemoryRequirementsKHR vkGetDeviceImageSparseMemoryRequirementsKHR { nullptr };
  PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT { nullptr };
  PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT { nullptr };
  PFN_vkDebugReportMessageEXT vkDebugReportMessageEXT { nullptr };
  PFN_vkDebugMarkerSetObjectTagEXT vkDebugMarkerSetObjectTagEXT { nullptr };
  PFN_vkDebugMarkerSetObjectNameEXT vkDebugMarkerSetObjectNameEXT { nullptr };
  PFN_vkCmdDebugMarkerBeginEXT vkCmdDebugMarkerBeginEXT { nullptr };
  PFN_vkCmdDebugMarkerEndEXT vkCmdDebugMarkerEndEXT { nullptr };
  PFN_vkCmdDebugMarkerInsertEXT vkCmdDebugMarkerInsertEXT { nullptr };
  PFN_vkCmdBindTransformFeedbackBuffersEXT vkCmdBindTransformFeedbackBuffersEXT { nullptr };
  PFN_vkCmdBeginTransformFeedbackEXT vkCmdBeginTransformFeedbackEXT { nullptr };
  PFN_vkCmdEndTransformFeedbackEXT vkCmdEndTransformFeedbackEXT { nullptr };
  PFN_vkCmdBeginQueryIndexedEXT vkCmdBeginQueryIndexedEXT { nullptr };
  PFN_vkCmdEndQueryIndexedEXT vkCmdEndQueryIndexedEXT { nullptr };
  PFN_vkCmdDrawIndirectByteCountEXT vkCmdDrawIndirectByteCountEXT { nullptr };
  PFN_vkCreateCuModuleNVX vkCreateCuModuleNVX { nullptr };
  PFN_vkCreateCuFunctionNVX vkCreateCuFunctionNVX { nullptr };
  PFN_vkDestroyCuModuleNVX vkDestroyCuModuleNVX { nullptr };
  PFN_vkDestroyCuFunctionNVX vkDestroyCuFunctionNVX { nullptr };
  PFN_vkCmdCuLaunchKernelNVX vkCmdCuLaunchKernelNVX { nullptr };
  PFN_vkGetImageViewHandleNVX vkGetImageViewHandleNVX { nullptr };
  PFN_vkGetImageViewAddressNVX vkGetImageViewAddressNVX { nullptr };
  PFN_vkCmdDrawIndirectCountAMD vkCmdDrawIndirectCountAMD { nullptr };
  PFN_vkCmdDrawIndexedIndirectCountAMD vkCmdDrawIndexedIndirectCountAMD { nullptr };
  PFN_vkGetShaderInfoAMD vkGetShaderInfoAMD { nullptr };
  PFN_vkGetPhysicalDeviceExternalImageFormatPropertiesNV vkGetPhysicalDeviceExternalImageFormatPropertiesNV { nullptr };
  PFN_vkCmdBeginConditionalRenderingEXT vkCmdBeginConditionalRenderingEXT { nullptr };
  PFN_vkCmdEndConditionalRenderingEXT vkCmdEndConditionalRenderingEXT { nullptr };
  PFN_vkCmdSetViewportWScalingNV vkCmdSetViewportWScalingNV { nullptr };
  PFN_vkReleaseDisplayEXT vkReleaseDisplayEXT { nullptr };
  PFN_vkGetPhysicalDeviceSurfaceCapabilities2EXT vkGetPhysicalDeviceSurfaceCapabilities2EXT { nullptr };
  PFN_vkDisplayPowerControlEXT vkDisplayPowerControlEXT { nullptr };
  PFN_vkRegisterDeviceEventEXT vkRegisterDeviceEventEXT { nullptr };
  PFN_vkRegisterDisplayEventEXT vkRegisterDisplayEventEXT { nullptr };
  PFN_vkGetSwapchainCounterEXT vkGetSwapchainCounterEXT { nullptr };
  PFN_vkGetRefreshCycleDurationGOOGLE vkGetRefreshCycleDurationGOOGLE { nullptr };
  PFN_vkGetPastPresentationTimingGOOGLE vkGetPastPresentationTimingGOOGLE { nullptr };
  PFN_vkCmdSetDiscardRectangleEXT vkCmdSetDiscardRectangleEXT { nullptr };
  PFN_vkSetHdrMetadataEXT vkSetHdrMetadataEXT { nullptr };
  PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT { nullptr };
  PFN_vkSetDebugUtilsObjectTagEXT vkSetDebugUtilsObjectTagEXT { nullptr };
  PFN_vkQueueBeginDebugUtilsLabelEXT vkQueueBeginDebugUtilsLabelEXT { nullptr };
  PFN_vkQueueEndDebugUtilsLabelEXT vkQueueEndDebugUtilsLabelEXT { nullptr };
  PFN_vkQueueInsertDebugUtilsLabelEXT vkQueueInsertDebugUtilsLabelEXT { nullptr };
  PFN_vkCmdBeginDebugUtilsLabelEXT vkCmdBeginDebugUtilsLabelEXT { nullptr };
  PFN_vkCmdEndDebugUtilsLabelEXT vkCmdEndDebugUtilsLabelEXT { nullptr };
  PFN_vkCmdInsertDebugUtilsLabelEXT vkCmdInsertDebugUtilsLabelEXT { nullptr };
  PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT { nullptr };
  PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT { nullptr };
  PFN_vkSubmitDebugUtilsMessageEXT vkSubmitDebugUtilsMessageEXT { nullptr };
  PFN_vkCmdSetSampleLocationsEXT vkCmdSetSampleLocationsEXT { nullptr };
  PFN_vkGetPhysicalDeviceMultisamplePropertiesEXT vkGetPhysicalDeviceMultisamplePropertiesEXT { nullptr };
  PFN_vkGetImageDrmFormatModifierPropertiesEXT vkGetImageDrmFormatModifierPropertiesEXT { nullptr };
  PFN_vkCreateValidationCacheEXT vkCreateValidationCacheEXT { nullptr };
  PFN_vkDestroyValidationCacheEXT vkDestroyValidationCacheEXT { nullptr };
  PFN_vkMergeValidationCachesEXT vkMergeValidationCachesEXT { nullptr };
  PFN_vkGetValidationCacheDataEXT vkGetValidationCacheDataEXT { nullptr };
  PFN_vkCmdBindShadingRateImageNV vkCmdBindShadingRateImageNV { nullptr };
  PFN_vkCmdSetViewportShadingRatePaletteNV vkCmdSetViewportShadingRatePaletteNV { nullptr };
  PFN_vkCmdSetCoarseSampleOrderNV vkCmdSetCoarseSampleOrderNV { nullptr };
  PFN_vkCreateAccelerationStructureNV vkCreateAccelerationStructureNV { nullptr };
  PFN_vkDestroyAccelerationStructureNV vkDestroyAccelerationStructureNV { nullptr };
  PFN_vkGetAccelerationStructureMemoryRequirementsNV vkGetAccelerationStructureMemoryRequirementsNV { nullptr };
  PFN_vkBindAccelerationStructureMemoryNV vkBindAccelerationStructureMemoryNV { nullptr };
  PFN_vkCmdBuildAccelerationStructureNV vkCmdBuildAccelerationStructureNV { nullptr };
  PFN_vkCmdCopyAccelerationStructureNV vkCmdCopyAccelerationStructureNV { nullptr };
  PFN_vkCmdTraceRaysNV vkCmdTraceRaysNV { nullptr };
  PFN_vkCreateRayTracingPipelinesNV vkCreateRayTracingPipelinesNV { nullptr };
  PFN_vkGetRayTracingShaderGroupHandlesKHR vkGetRayTracingShaderGroupHandlesKHR { nullptr };
  PFN_vkGetRayTracingShaderGroupHandlesNV vkGetRayTracingShaderGroupHandlesNV { nullptr };
  PFN_vkGetAccelerationStructureHandleNV vkGetAccelerationStructureHandleNV { nullptr };
  PFN_vkCmdWriteAccelerationStructuresPropertiesNV vkCmdWriteAccelerationStructuresPropertiesNV { nullptr };
  PFN_vkCompileDeferredNV vkCompileDeferredNV { nullptr };
  PFN_vkGetMemoryHostPointerPropertiesEXT vkGetMemoryHostPointerPropertiesEXT { nullptr };
  PFN_vkCmdWriteBufferMarkerAMD vkCmdWriteBufferMarkerAMD { nullptr };
  PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT vkGetPhysicalDeviceCalibrateableTimeDomainsEXT { nullptr };
  PFN_vkGetCalibratedTimestampsEXT vkGetCalibratedTimestampsEXT { nullptr };
  PFN_vkCmdDrawMeshTasksNV vkCmdDrawMeshTasksNV { nullptr };
  PFN_vkCmdDrawMeshTasksIndirectNV vkCmdDrawMeshTasksIndirectNV { nullptr };
  PFN_vkCmdDrawMeshTasksIndirectCountNV vkCmdDrawMeshTasksIndirectCountNV { nullptr };
  PFN_vkCmdSetExclusiveScissorNV vkCmdSetExclusiveScissorNV { nullptr };
  PFN_vkCmdSetCheckpointNV vkCmdSetCheckpointNV { nullptr };
  PFN_vkGetQueueCheckpointDataNV vkGetQueueCheckpointDataNV { nullptr };
  PFN_vkInitializePerformanceApiINTEL vkInitializePerformanceApiINTEL { nullptr };
  PFN_vkUninitializePerformanceApiINTEL vkUninitializePerformanceApiINTEL { nullptr };
  PFN_vkCmdSetPerformanceMarkerINTEL vkCmdSetPerformanceMarkerINTEL { nullptr };
  PFN_vkCmdSetPerformanceStreamMarkerINTEL vkCmdSetPerformanceStreamMarkerINTEL { nullptr };
  PFN_vkCmdSetPerformanceOverrideINTEL vkCmdSetPerformanceOverrideINTEL { nullptr };
  PFN_vkAcquirePerformanceConfigurationINTEL vkAcquirePerformanceConfigurationINTEL { nullptr };
  PFN_vkReleasePerformanceConfigurationINTEL vkReleasePerformanceConfigurationINTEL { nullptr };
  PFN_vkQueueSetPerformanceConfigurationINTEL vkQueueSetPerformanceConfigurationINTEL { nullptr };
  PFN_vkGetPerformanceParameterINTEL vkGetPerformanceParameterINTEL { nullptr };
  PFN_vkSetLocalDimmingAMD vkSetLocalDimmingAMD { nullptr };
  PFN_vkGetBufferDeviceAddressEXT vkGetBufferDeviceAddressEXT { nullptr };
  PFN_vkGetPhysicalDeviceToolPropertiesEXT vkGetPhysicalDeviceToolPropertiesEXT { nullptr };
  PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV vkGetPhysicalDeviceCooperativeMatrixPropertiesNV { nullptr };
  PFN_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV { nullptr };
  PFN_vkCreateHeadlessSurfaceEXT vkCreateHeadlessSurfaceEXT { nullptr };
  PFN_vkCmdSetLineStippleEXT vkCmdSetLineStippleEXT { nullptr };
  PFN_vkResetQueryPoolEXT vkResetQueryPoolEXT { nullptr };
  PFN_vkCmdSetCullModeEXT vkCmdSetCullModeEXT { nullptr };
  PFN_vkCmdSetFrontFaceEXT vkCmdSetFrontFaceEXT { nullptr };
  PFN_vkCmdSetPrimitiveTopologyEXT vkCmdSetPrimitiveTopologyEXT { nullptr };
  PFN_vkCmdSetViewportWithCountEXT vkCmdSetViewportWithCountEXT { nullptr };
  PFN_vkCmdSetScissorWithCountEXT vkCmdSetScissorWithCountEXT { nullptr };
  PFN_vkCmdBindVertexBuffers2EXT vkCmdBindVertexBuffers2EXT { nullptr };
  PFN_vkCmdSetDepthTestEnableEXT vkCmdSetDepthTestEnableEXT { nullptr };
  PFN_vkCmdSetDepthWriteEnableEXT vkCmdSetDepthWriteEnableEXT { nullptr };
  PFN_vkCmdSetDepthCompareOpEXT vkCmdSetDepthCompareOpEXT { nullptr };
  PFN_vkCmdSetDepthBoundsTestEnableEXT vkCmdSetDepthBoundsTestEnableEXT { nullptr };
  PFN_vkCmdSetStencilTestEnableEXT vkCmdSetStencilTestEnableEXT { nullptr };
  PFN_vkCmdSetStencilOpEXT vkCmdSetStencilOpEXT { nullptr };
  PFN_vkGetGeneratedCommandsMemoryRequirementsNV vkGetGeneratedCommandsMemoryRequirementsNV { nullptr };
  PFN_vkCmdPreprocessGeneratedCommandsNV vkCmdPreprocessGeneratedCommandsNV { nullptr };
  PFN_vkCmdExecuteGeneratedCommandsNV vkCmdExecuteGeneratedCommandsNV { nullptr };
  PFN_vkCmdBindPipelineShaderGroupNV vkCmdBindPipelineShaderGroupNV { nullptr };
  PFN_vkCreateIndirectCommandsLayoutNV vkCreateIndirectCommandsLayoutNV { nullptr };
  PFN_vkDestroyIndirectCommandsLayoutNV vkDestroyIndirectCommandsLayoutNV { nullptr };
  PFN_vkAcquireDrmDisplayEXT vkAcquireDrmDisplayEXT { nullptr };
  PFN_vkGetDrmDisplayEXT vkGetDrmDisplayEXT { nullptr };
  PFN_vkCreatePrivateDataSlotEXT vkCreatePrivateDataSlotEXT { nullptr };
  PFN_vkDestroyPrivateDataSlotEXT vkDestroyPrivateDataSlotEXT { nullptr };
  PFN_vkSetPrivateDataEXT vkSetPrivateDataEXT { nullptr };
  PFN_vkGetPrivateDataEXT vkGetPrivateDataEXT { nullptr };
  PFN_vkCmdSetFragmentShadingRateEnumNV vkCmdSetFragmentShadingRateEnumNV { nullptr };
  PFN_vkAcquireWinrtDisplayNV vkAcquireWinrtDisplayNV { nullptr };
  PFN_vkGetWinrtDisplayNV vkGetWinrtDisplayNV { nullptr };
  PFN_vkCmdSetVertexInputEXT vkCmdSetVertexInputEXT { nullptr };
  PFN_vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI { nullptr };
  PFN_vkCmdSubpassShadingHUAWEI vkCmdSubpassShadingHUAWEI { nullptr };
  PFN_vkCmdBindInvocationMaskHUAWEI vkCmdBindInvocationMaskHUAWEI { nullptr };
  PFN_vkGetMemoryRemoteAddressNV vkGetMemoryRemoteAddressNV { nullptr };
  PFN_vkCmdSetPatchControlPointsEXT vkCmdSetPatchControlPointsEXT { nullptr };
  PFN_vkCmdSetRasterizerDiscardEnableEXT vkCmdSetRasterizerDiscardEnableEXT { nullptr };
  PFN_vkCmdSetDepthBiasEnableEXT vkCmdSetDepthBiasEnableEXT { nullptr };
  PFN_vkCmdSetLogicOpEXT vkCmdSetLogicOpEXT { nullptr };
  PFN_vkCmdSetPrimitiveRestartEnableEXT vkCmdSetPrimitiveRestartEnableEXT { nullptr };
  PFN_vkCmdSetColorWriteEnableEXT vkCmdSetColorWriteEnableEXT { nullptr };
  PFN_vkCmdDrawMultiEXT vkCmdDrawMultiEXT { nullptr };
  PFN_vkCmdDrawMultiIndexedEXT vkCmdDrawMultiIndexedEXT { nullptr };
  PFN_vkSetDeviceMemoryPriorityEXT vkSetDeviceMemoryPriorityEXT { nullptr };
  PFN_vkGetDescriptorSetLayoutHostMappingInfoVALVE vkGetDescriptorSetLayoutHostMappingInfoVALVE { nullptr };
  PFN_vkGetDescriptorSetHostMappingVALVE vkGetDescriptorSetHostMappingVALVE { nullptr };
  PFN_vkCreateAccelerationStructureKHR vkCreateAccelerationStructureKHR { nullptr };
  PFN_vkDestroyAccelerationStructureKHR vkDestroyAccelerationStructureKHR { nullptr };
  PFN_vkCmdBuildAccelerationStructuresKHR vkCmdBuildAccelerationStructuresKHR { nullptr };
  PFN_vkCmdBuildAccelerationStructuresIndirectKHR vkCmdBuildAccelerationStructuresIndirectKHR { nullptr };
  PFN_vkBuildAccelerationStructuresKHR vkBuildAccelerationStructuresKHR { nullptr };
  PFN_vkCopyAccelerationStructureKHR vkCopyAccelerationStructureKHR { nullptr };
  PFN_vkCopyAccelerationStructureToMemoryKHR vkCopyAccelerationStructureToMemoryKHR { nullptr };
  PFN_vkCopyMemoryToAccelerationStructureKHR vkCopyMemoryToAccelerationStructureKHR { nullptr };
  PFN_vkWriteAccelerationStructuresPropertiesKHR vkWriteAccelerationStructuresPropertiesKHR { nullptr };
  PFN_vkCmdCopyAccelerationStructureKHR vkCmdCopyAccelerationStructureKHR { nullptr };
  PFN_vkCmdCopyAccelerationStructureToMemoryKHR vkCmdCopyAccelerationStructureToMemoryKHR { nullptr };
  PFN_vkCmdCopyMemoryToAccelerationStructureKHR vkCmdCopyMemoryToAccelerationStructureKHR { nullptr };
  PFN_vkGetAccelerationStructureDeviceAddressKHR vkGetAccelerationStructureDeviceAddressKHR { nullptr };
  PFN_vkCmdWriteAccelerationStructuresPropertiesKHR vkCmdWriteAccelerationStructuresPropertiesKHR { nullptr };
  PFN_vkGetDeviceAccelerationStructureCompatibilityKHR vkGetDeviceAccelerationStructureCompatibilityKHR { nullptr };
  PFN_vkGetAccelerationStructureBuildSizesKHR vkGetAccelerationStructureBuildSizesKHR { nullptr };
  PFN_vkCmdTraceRaysKHR vkCmdTraceRaysKHR { nullptr };
  PFN_vkCreateRayTracingPipelinesKHR vkCreateRayTracingPipelinesKHR { nullptr };
  PFN_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR vkGetRayTracingCaptureReplayShaderGroupHandlesKHR { nullptr };
  PFN_vkCmdTraceRaysIndirectKHR vkCmdTraceRaysIndirectKHR { nullptr };
  PFN_vkGetRayTracingShaderGroupStackSizeKHR vkGetRayTracingShaderGroupStackSizeKHR { nullptr };
  PFN_vkCmdSetRayTracingPipelineStackSizeKHR vkCmdSetRayTracingPipelineStackSizeKHR { nullptr };
  PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR { nullptr };
  PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR vkGetPhysicalDeviceWin32PresentationSupportKHR { nullptr };
  PFN_vkGetMemoryWin32HandleKHR vkGetMemoryWin32HandleKHR { nullptr };
  PFN_vkGetMemoryWin32HandlePropertiesKHR vkGetMemoryWin32HandlePropertiesKHR { nullptr };
  PFN_vkImportSemaphoreWin32HandleKHR vkImportSemaphoreWin32HandleKHR { nullptr };
  PFN_vkGetSemaphoreWin32HandleKHR vkGetSemaphoreWin32HandleKHR { nullptr };
  PFN_vkImportFenceWin32HandleKHR vkImportFenceWin32HandleKHR { nullptr };
  PFN_vkGetFenceWin32HandleKHR vkGetFenceWin32HandleKHR { nullptr };
  PFN_vkGetMemoryWin32HandleNV vkGetMemoryWin32HandleNV { nullptr };
  PFN_vkGetPhysicalDeviceSurfacePresentModes2EXT vkGetPhysicalDeviceSurfacePresentModes2EXT { nullptr };
  PFN_vkAcquireFullScreenExclusiveModeEXT vkAcquireFullScreenExclusiveModeEXT { nullptr };
  PFN_vkReleaseFullScreenExclusiveModeEXT vkReleaseFullScreenExclusiveModeEXT { nullptr };
  PFN_vkGetDeviceGroupSurfacePresentModes2EXT vkGetDeviceGroupSurfacePresentModes2EXT { nullptr };
};

} // namespace plex::graphics::vkapi::loader

// clang-format on

#endif // PLEX_GRAPHICS_VULKAN_FUNCTION_TABLE_H
