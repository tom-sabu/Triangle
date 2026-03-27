#ifndef VULKAN_DEVICE_H

#define VULKAN_DEVICE_H
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <optional>
#include "ValidationLayers.h"
#include "VulkanSwapChain.h"
#include "VulkanPipeLine.h"
#include "VulkanRenderer.h"



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

  QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

  VulkanDevice(const VulkanDevice &) = delete;
  VulkanDevice &operator=(const VulkanDevice &) = delete;

  VkInstance getInstance() const { return instance; }
  VkPhysicalDevice getPhysicalDevice() const { return physicalDevice; }
  VkDevice getDevice() const { return device; }
  VkSurfaceKHR getSurface() const { return surface; }
  Window &getWindow() { return window; }
  VulkanSwapChain &getSwapChain() { return vulkanSwapChain; }
  VulkanPipeLine &getPipeLine() { return vulkanPipeLine; }

private:
  VkInstance instance;
  const std::vector<const char*> deviceExtensions = {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME
  };
  VkSurfaceKHR surface;
  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
  VkDevice device;
  VkQueue graphicsQueue;
  VkQueue presentQueue;

  ValidationLayers validationLayers;
  VulkanSwapChain vulkanSwapChain;
  VulkanPipeLine vulkanPipeLine;
  VulkanRenderer vulkanRenderer;

  Window &window;

  void initVulkan();
  void createInstance();

  void createSurface();

  void pickPhysicalDevice();
  int rateDeviceSuitability(VkPhysicalDevice device);
  bool checkDeviceExtensionSupport(VkPhysicalDevice device);

  void createLogicalDevice();

  std::vector<const char *> getRequiredExtensions();
  bool checkExtensionSupport(
      const char **requiredExtensions, uint32_t requiredCount,
      const std::vector<VkExtensionProperties> &availableExtentions);
};

#endif
