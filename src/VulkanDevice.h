#ifndef VULKAN_DEVICE_H

#define VULKAN_DEVICE_H
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <optional>

struct QueueFamilyIndices {
  std::optional<uint32_t> graphicsFamily;

  bool isComplete() {
    return graphicsFamily.has_value();
  }
};

class Window;

class VulkanDevice {
public:
  VulkanDevice(Window &window);
  ~VulkanDevice();

  VulkanDevice(const VulkanDevice &) = delete;
  VulkanDevice &operator=(const VulkanDevice &) = delete;

  VkInstance getInstance() const { return instance; }

private:
  VkInstance instance;
  VkDebugUtilsMessengerEXT debugMessenger;
  Window &window;
  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
  VkDevice device;
    
  }

  void createInstance();
  void initVulkan();
  void pickPhysicalDevice();
  QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
  int rateDeviceSuitability(VkPhysicalDevice device);
  void createLogicalDevice();
  void setupDebugMessenger();
  void populateDebugMessengerCreateInfo(
      VkDebugUtilsMessengerCreateInfoEXT &createInfo);
  bool checkExtensionSupport(
      const char **requiredExtensions, uint32_t requiredCount,
      const std::vector<VkExtensionProperties> &availableExtentions);
  std::vector<const char *> getRequiredExtensions();
};

#endif
