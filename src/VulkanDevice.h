#ifndef VULKAN_DEVICE_H

#define VULKAN_DEVICE_H
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

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

  void createInstance();
  void initVulkan();
  void pickPhysicalDevice();
  int rateDeviceSuitability(VkPhysicalDevice device);
  void setupDebugMessenger();
  void populateDebugMessengerCreateInfo(
      VkDebugUtilsMessengerCreateInfoEXT &createInfo);
  bool checkExtensionSupport(
      const char **requiredExtensions, uint32_t requiredCount,
      const std::vector<VkExtensionProperties> &availableExtentions);
  std::vector<const char *> getRequiredExtensions();
};

#endif
