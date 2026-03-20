#ifndef VULKAN_PIPE_LINE_H
#define VULKAN_PIPE_LINE_H

class VulkanDevice;
#include <string>
#include <vector>
#include <vulkan/vulkan.h>

class VulkanPipeLine {
public:

  VulkanPipeLine(VulkanDevice &device); 

  void createGraphicsPipeline();
  void cleanup();

private:

  VkShaderModule createShaderModule(const std::vector<char>& code);

  VulkanDevice &device;
  VkPipelineLayout pipelineLayout;
};

#endif

