#ifndef VULKAN_DEVICE_H

#define VULKAN_DEVICE_H
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <string>

class Window;

class VulkanDevice {
public:
    VulkanDevice(Window& window);
    ~VulkanDevice();

    VulkanDevice(const VulkanDevice&) = delete;
    VulkanDevice& operator= (const VulkanDevice&) = delete;

    VkInstance getInstance() const { return instance; }

private:
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    Window& window;

    void createInstance();
    void initVulkan();
    void setupDebugMessenger();
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    bool checkExtensionSupport(
        const char** requiredExtensions,
        uint32_t requiredCount,
        const std::vector<VkExtensionProperties>& availableExtentions
    );
    std::vector<const char*> getRequiredExtensions();
};

#endif
