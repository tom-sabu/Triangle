#include "VulkanRenderer.h"
#include "VulkanDevice.h"
#include <stdexcept>

VulkanRenderer::VulkanRenderer(VulkanDevice &device) : device(device) {}

void VulkanRenderer::createFramebuffers() {
  swapChainImageViews = device.getSwapChain().getSwapChainImageViews();
  swapChainFramebuffers.resize(swapChainImageViews.size());

  for (size_t i = 0; i < swapChainImageViews.size(); i++) {
    VkImageView attachments[] = {swapChainImageViews[i]};

    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = device.getPipeLine().getRenderPass();
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = attachments;
    framebufferInfo.width = device.getSwapChain().getSwapChainExtent().width;
    framebufferInfo.height = device.getSwapChain().getSwapChainExtent().height;
    framebufferInfo.layers = 1;

    if (vkCreateFramebuffer(device.getDevice(), &framebufferInfo, nullptr,
                            &swapChainFramebuffers[i]) != VK_SUCCESS) {
      throw std::runtime_error("failed to create framebuffer!");
    }
  }
}

void VulkanRenderer::createCommandPool() {
  VulkanDevice::QueueFamilyIndices queueFamilyIndices =
      device.findQueueFamilies(device.getPhysicalDevice());
  VkCommandPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

  if (vkCreateCommandPool(device.getDevice(), &poolInfo, nullptr,
                          &commandPool) != VK_SUCCESS) {
    throw std::runtime_error("failed to create command pool!");
  }
}

void VulkanRenderer::createCommandBuffer() {
  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool = commandPool;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandBufferCount = 1;

  if (vkAllocateCommandBuffers(device.getDevice(), &allocInfo,
                               &commandBuffer) != VK_SUCCESS) {
    throw std::runtime_error("failed to allocate command buffers!");
  }
}

void VulkanRenderer::recordCommandBuffer(VkCommandBuffer commandBuffer,
                                         uint32_t imageIndex) {
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = 0;
  beginInfo.pInheritanceInfo = nullptr;

  if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
    throw std::runtime_error("failed to begin recording command buffer!");
  }

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = device.getPipeLine().getRenderPass();
  renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];
  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = device.getSwapChain().getSwapChainExtent();

  VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
  renderPassInfo.clearValueCount = 1;
  renderPassInfo.pClearValues = &clearColor;

  vkCmdBeginRenderPass(commandBuffer, &renderPassInfo,
                       VK_SUBPASS_CONTENTS_INLINE);
  vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                    device.getPipeLine().getGraphicsPipeline());

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width =
      static_cast<float>(device.getSwapChain().getSwapChainExtent().width);
  viewport.height =
      static_cast<float>(device.getSwapChain().getSwapChainExtent().height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = device.getSwapChain().getSwapChainExtent();
  vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

  vkCmdDraw(commandBuffer, 3, 1, 0, 0);
  vkCmdEndRendering(commandBuffer);

  if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
    throw std::runtime_error("failed to record command buffer!");
  }
}

void VulkanRenderer::createSyncObjects() {
  VkSemaphoreCreateInfo semaphoreInfo{};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkFenceCreateInfo fenceInfo{};
  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  if (vkCreateSemaphore(device.getDevice(), &semaphoreInfo, nullptr,
                        &imageAvailabeSemaphore) != VK_SUCCESS ||
      vkCreateSemaphore(device.getDevice(), &semaphoreInfo, nullptr,
                        &renderFinishedSemaphore) != VK_SUCCESS ||
      vkCreateFence(device.getDevice(), &fenceInfo, nullptr, &inFlightFence) !=
          VK_SUCCESS) {
    throw std::runtime_error("failed to create semaphore!");
  }
}

void VulkanRenderer::drawFrame() {
  vkWaitForFences(device.getDevice(), 1, &inFlightFence, VK_TRUE, UINT64_MAX);
  vkResetFences(device.getDevice(), 1, &inFlightFence);

  uint32_t imageIndex;
  vkAcquireNextImageKHR(device.getDevice(), device.getSwapChain().getSwapChain(), imageAvailabeSemaphore, VK_NULL_HANDLE, &imageIndex);
  vkResetCommandBuffer(commandBuffer, 0);
  recordCommandBuffer(commandBuffer, imageIndex);

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore waitSemaphores[] = {imageAvailabeSemaphore};
  VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = waitSemaphores;
  submitInfo.pWaitDstStageMask = waitStages;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffer;

  VkSemaphore signalSemaphores[] = {renderFinishedSemaphore};
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = signalSemaphores;

  if (vkQueueSubmit(device.getGraphicsQueue(), 1, &submitInfo, inFlightFence) != VK_SUCCESS) {
    throw std::runtime_error("failed to submit draw command buffer!");
  }
}

void VulkanRenderer::cleanup() {
  vkDestroySemaphore(device.getDevice(), imageAvailabeSemaphore, nullptr);
  vkDestroySemaphore(device.getDevice(), renderFinishedSemaphore, nullptr);
  vkDestroyFence(device.getDevice(), inFlightFence, nullptr);
  vkDestroyCommandPool(device.getDevice(), commandPool, nullptr);
  for (auto framebuffer : swapChainFramebuffers) {
    vkDestroyFramebuffer(device.getDevice(), framebuffer, nullptr);
  }
  swapChainFramebuffers.clear();
}
