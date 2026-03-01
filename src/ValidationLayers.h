#ifndef VALIDATION_LAYER_H
#define VALIDATION_LAYER_H

#include <vector>
#include <vulkan/vulkan.h>

class ValidationLayers {
public:
  static const std::vector<const char *> validationLayers;

#ifdef NDEBUG
  static constexpr bool enable = false;
#else
  static constexpr bool enable = true;
#endif

  static bool checkSupport();
  static void populateMessengerCreateInfo(
      VkDebugUtilsMessengerCreateInfoEXT &createInfo);

  void setup(VkInstance instance);
  void cleanup(VkInstance instance);

private:
  VkDebugUtilsMessengerEXT messenger = VK_NULL_HANDLE;
};

#endif
