//    const std::array<VkImageTiling, 2> tilings { VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_TILING_LINEAR };
//    const std::array<VkImageUsageFlags, 3> usages {
//      VK_IMAGE_USAGE_STORAGE_BIT,
//      VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
//      VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,
//    };
//
//    std::vector<std::tuple<VkImageUsageFlags, VkImageTiling, VkSurfaceFormatKHR>> supported_combinations;
//    for (const auto& u : usages)
//    {
//      for (const auto& tiling : tilings)
//      {
//        for (const auto& format : swapchain_support.formats)
//        {
//          VkImageFormatProperties image_format_properties_ {};
//          auto result = vkGetPhysicalDeviceImageFormatProperties(device_->GetPhysicalDeviceHandle(),
//            format.format,
//            VK_IMAGE_TYPE_2D,
//            tiling,
//            u,
//            0,
//            &image_format_properties_);
//          if (result != VK_ERROR_FORMAT_NOT_SUPPORTED)
//          {
//            supported_combinations.emplace_back(u, tiling, format);
//          }
//        }
//      }
//    }
//
//    const std::unordered_map<uint64_t, std::string> usage_to_string = {
//      { VK_IMAGE_USAGE_STORAGE_BIT, "VK_IMAGE_USAGE_STORAGE_BIT" },
//      { VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, "VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT" },
//      { VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT, "VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT" },
//    };
//
//    const std::unordered_map<uint64_t, std::string> tiling_to_string = {
//      { VK_IMAGE_TILING_OPTIMAL, "VK_IMAGE_TILING_OPTIMAL" },
//      { VK_IMAGE_TILING_LINEAR, "VK_IMAGE_TILING_LINEAR" },
//    };
//
//    const std::unordered_map<uint64_t, std::string> format_to_string = {
//      { VK_FORMAT_R8G8B8A8_UNORM, "VK_FORMAT_R8G8B8A8_UNORM" },
//      { VK_FORMAT_R8G8B8A8_SRGB, "VK_FORMAT_R8G8B8A8_SRGB" },
//      { VK_FORMAT_B8G8R8A8_UNORM, "VK_FORMAT_B8G8R8A8_UNORM" },
//      { VK_FORMAT_B8G8R8A8_SRGB, "VK_FORMAT_B8G8R8A8_SRGB" },
//      { VK_FORMAT_R8G8B8A8_SNORM, "VK_FORMAT_R8G8B8A8_SNORM" },
//      { VK_FORMAT_R8G8B8A8_USCALED, "VK_FORMAT_R8G8B8A8_USCALED" },
//      { VK_FORMAT_R8G8B8A8_SSCALED, "VK_FORMAT_R8G8B8A8_SSCALED" },
//      { VK_FORMAT_R8G8B8A8_UINT, "VK_FORMAT_R8G8B8A8_UINT" },
//      { VK_FORMAT_R8G8B8A8_SINT, "VK_FORMAT_R8G8B8A8_SINT" },
//      { VK_FORMAT_A2B10G10R10_UNORM_PACK32, "VK_FORMAT_A2B10G10R10_UNORM_PACK32" },
//      { VK_FORMAT_A2B10G10R10_SNORM_PACK32, "VK_FORMAT_A2B10G10R10_SNORM_PACK32" },
//      { VK_FORMAT_A2B10G10R10_USCALED_PACK32, "VK_FORMAT_A2B10G10R10_USCALED_PACK32" },
//      { VK_FORMAT_A2B10G10R10_SSCALED_PACK32, "VK_FORMAT_A2B10G10R10_SSCALED_PACK32" },
//      { VK_FORMAT_A2B10G10R10_UINT_PACK32, "VK_FORMAT_A2B10G10R10_UINT_PACK32" },
//      { VK_FORMAT_A2B10G10R10_SINT_PACK32, "VK_FORMAT_A2B10G10R10_SINT_PACK32" },
//      { VK_FORMAT_R16G16B16A16_UNORM, "VK_FORMAT_R16G16B16A16_UNORM" },
//      { VK_FORMAT_R16G16B16A16_SNORM, "VK_FORMAT_R16G16B16A16_SNORM" },
//      { VK_FORMAT_R16G16B16A16_USCALED, "VK_FORMAT_R16G16B16A16_USCALED" },
//      { VK_FORMAT_R16G16B16A16_SSCALED, "VK_FORMAT_R16G16B16A16_SSCALED" },
//      { VK_FORMAT_R16G16B16A16_UINT, "VK_FORMAT_R16G16B16A16_UINT" },
//      { VK_FORMAT_R16G16B16A16_SINT, "VK_FORMAT_R16G16B16A16_SINT" },
//    };
//    for (const auto& combination : supported_combinations)
//    {
//      const auto& [u, tiling, format] = combination;
//      std::cout << "Usage: " << usage_to_string.at(u) << '\n';
//      std::cout << "Tiling: " << tiling_to_string.at(tiling) << '\n';
//      std::cout << "Format: " << format_to_string.at(format.format) << "\n\n\n" << std::endl;
//    }