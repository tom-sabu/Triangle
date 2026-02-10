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
};

#endif
