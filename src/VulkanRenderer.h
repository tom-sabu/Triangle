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
  void createCommandPool();
  void createCommandBuffer();
  void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
  void cleanup();

private:
  VulkanDevice &device;

  VkCommandPool commandPool;
  VkCommandBuffer commandBuffer;
  std::vector<VkImageView> swapChainImageViews;
  std::vector<VkFramebuffer> swapChainFramebuffers;
};

#endif
