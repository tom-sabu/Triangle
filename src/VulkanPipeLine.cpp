#include "VulkanPipeLine.h"
#include <fstream>
#include <vector>

VulkanPipeLine::VulkanPipeLine(VulkanDevice &device,
                               const std::string &vertFilepath,
                               const std::string &fragFilepath)
    : device{device}, vertFilepath(vertFilepath), fragFilepath(fragFilepath) {}

static std::vector<char> readFile(const std::string &filename) {
  std::ifstream file(filename, std::ios::ate | std::ios::binary);

  if (!file.is_open()) {
    throw std::runtime_error("failed to open file!");
  }

  size_t fileSize = (size_t)file.tellg();
  std::vector<char> buffer(fileSize);

  file.seekg(0);
  file.read(buffer.data(), fileSize);

  file.close();

  return buffer;
}

void VulkanPipeLine::createGraphicsPipeline() {
  auto vertShaderCode = readFile("Shaders/vert.spv");
  auto fragShaderCode = readFile("Shaders/frag.spv");
}
