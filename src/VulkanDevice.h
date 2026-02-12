#ifndef VULKAN_DEVICE_H

#define VULKAN_DEVICE_H
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <optional>



class Window;

class VulkanDevice {
public:
  struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
      return graphicsFamily.has_value() && presentFamily.has_value();
    }
  };

  VulkanDevice(Window &window);
  ~VulkanDevice();

  VulkanDevice(const VulkanDevice &) = delete;
  VulkanDevice &operator=(const VulkanDevice &) = delete;

  VkInstance getInstance() const { return instance; }

private:
  VkInstance instance;
  VkDebugUtilsMessengerEXT debugMessenger;
  VkSurfaceKHR surface;
  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
  VkDevice device;
  VkQueue graphicsQueue;
  VkQueue presentQueue;

  Window &window;

  void initVulkan();
  void createInstance();
  void setupDebugMessenger();

  void createSurface();

  void pickPhysicalDevice();
  int rateDeviceSuitability(VkPhysicalDevice device);
  QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

  void createLogicalDevice();

  void populateDebugMessengerCreateInfo(
      VkDebugUtilsMessengerCreateInfoEXT &createInfo);

  std::vector<const char *> getRequiredExtensions();
  bool checkExtensionSupport(
      const char **requiredExtensions, uint32_t requiredCount,
      const std::vector<VkExtensionProperties> &availableExtentions);
};

#endif
