#ifndef VULKAN_PIPE_LINE_H
#define VULKAN_PIPE_LINE_H

#include "VulkanDevice.h"
#include <string>
#include <vector>

class VulkanPipeLine {
public:

  VulkanPipeLine(VulkanDevice &device, const std::string& vertFilepath, const std::string& fragFilepath); 

  void createGraphicsPipeline();

private:

  VkShaderModule createShaderModule(const std::vector<char>& code);

  VulkanDevice &device;
  std::string vertFilepath;
  std::string fragFilepath;
};

#endif

