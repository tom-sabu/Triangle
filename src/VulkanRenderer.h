#ifndef VULKAN_RENDERER_H
#define VULKAN_RENDERER_H

#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>

class VulkanDevice;

class VulkanRenderer {

public:
  VulkanRenderer(VulkanDevice &device);

  void createFramebuffers();
  void cleanup();

private:
  VulkanDevice &device;

  std::vector<VkImageView> swapChainImageViews;
  std::vector<VkFramebuffer> swapChainFramebuffers;
};

#endif
