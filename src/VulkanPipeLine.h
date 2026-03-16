#ifndef VULKAN_PIPE_LINE_H
#define VULKAN_PIPE_LINE_H

class VulkanPipeLine {
public:
  void createGraphicsPipeline();

private:
  vec2 positions[3] = vec2[](
    vec2(0.0, -0.5),
    vec2(0.5, 0.5),
    vec2(-0.5, 0.5),
  )
};

#endif

