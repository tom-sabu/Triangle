#include "VulkanRenderer.h"
#include "VulkanDevice.h"
#include <stdexcept>

VulkanRenderer::VulkanRenderer(VulkanDevice &device) : device(device) {}

void VulkanRenderer::createFramebuffers() {
  swapChainImageViews = device.getSwapChain().getSwapChainImageViews();
  swapChainFramebuffers.resize(swapChainImageViews.size());

  for (size_t i = 0; i < swapChainImageViews.size(); i++) {
    VkImageView attachments[] = {
      swapChainImageViews[i]
    };

    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = device.getPipeLine().getRenderPass();
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = attachments;
    framebufferInfo.width = device.getSwapChain().getSwapChainExtent().width;
    framebufferInfo.height = device.getSwapChain().getSwapChainExtent().height;
    framebufferInfo.layers = 1;

    if (vkCreateFramebuffer(device.getDevice(), &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
      throw std::runtime_error("failed to create framebuffer!");
    }
  }
}

void VulkanRenderer::cleanup() {
  for (auto framebuffer : swapChainFramebuffers) {
    vkDestroyFramebuffer(device.getDevice(), framebuffer, nullptr);
  }
  swapChainFramebuffers.clear();
}
