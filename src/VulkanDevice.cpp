#include "VulkanDevice.h"
#include "ValidationLayers.h"
#include "Window.h"
#include <cstring>
#include <iostream>
#include <stdexcept>

static VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
              VkDebugUtilsMessageTypeFlagsEXT messageType,
              const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
              void *pUserData) {

  std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

  return VK_FALSE;
}

VkResult CreateDebugUtilsMessengerEXT(
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    VkAllocationCallbacks *pAllocator,
    VkDebugUtilsMessengerEXT *pDebugMessenger) {
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr) {
    return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
  } else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                   VkDebugUtilsMessengerEXT debugMessenger,
                                   const VkAllocationCallbacks *pAllocator) {
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkDestroyDebugUtilsMessengerEXT");
  if (func != nullptr) {
    func(instance, debugMessenger, pAllocator);
  }
}

VulkanDevice::VulkanDevice(Window &window)
    : window(window), instance(VK_NULL_HANDLE) {
  initVulkan();
}

VulkanDevice::~VulkanDevice() {

  if (ValidationLayers::enable) {
    DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
  }
  if (instance != VK_NULL_HANDLE) {
    vkDestroyInstance(instance, nullptr);
  }
}

void VulkanDevice::createInstance() {

  if (ValidationLayers::enable && !ValidationLayers::checkSupport()) {
    throw std::runtime_error("Validation layers requested, but not available!");
  }

  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Hello Triangle";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_3;

  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  // getting available extensions
  uint32_t extensionCount = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
  std::vector<VkExtensionProperties> extensions(extensionCount);
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount,
                                         extensions.data());

  std::cout << "Available extensions:\n";
  for (const auto &extension : extensions) {
    std::cout << '\t' << extension.extensionName << '\n';
  }

  // getting required extensions form GLFW
  auto requiredExtensions = getRequiredExtensions();

  std::cout << "\nRequired GLFW extensions:\n";
  for (const auto &ext : requiredExtensions) {
    std::cout << '\t' << ext << '\n';
  }

  if (!checkExtensionSupport(requiredExtensions.data(),
                             static_cast<uint32_t>(requiredExtensions.size()),
                             extensions)) {
    throw std::runtime_error("Required GLFW extensions are not suppoerted!");
  }
  std::cout << "\nAll required extensions are supported!\n" << std::endl;

  // creating instance
  createInfo.enabledExtensionCount =
      static_cast<uint32_t>(requiredExtensions.size());
  createInfo.ppEnabledExtensionNames = requiredExtensions.data();

  if (ValidationLayers::enable) {
    createInfo.enabledLayerCount =
        static_cast<uint32_t>(ValidationLayers::validationLayers.size());
    createInfo.ppEnabledLayerNames = ValidationLayers::validationLayers.data();
    std::cout << "Validation layers enabled\n" << std::endl;
  } else {
    createInfo.enabledLayerCount = 0;
    std::cout << "Validation layers disabled\n" << std::endl;
  }

  if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create Vulkan instance!");
  }
}

void VulkanDevice::initVulkan() {
  createInstance();
  setupDebugMessenger();
}

void VulkanDevice::populateDebugMessengerCreateInfo(
    VkDebugUtilsMessengerCreateInfoEXT &createInfo) {
  createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  createInfo.pfnUserCallback = debugCallback;
  createInfo.pUserData = nullptr;
}

void VulkanDevice::setupDebugMessenger() {
  if (!ValidationLayers::enable)
    return;

  VkDebugUtilsMessengerCreateInfoEXT createInfo;
  populateDebugMessengerCreateInfo(createInfo);

  if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr,
                                   &debugMessenger) != VK_SUCCESS) {
    throw std::runtime_error("failed to set up debug messenger!");
  }
}

// checking if all required extensions are available
bool VulkanDevice::checkExtensionSupport(
    const char **requiredExtensions, uint32_t requiredCount,
    const std::vector<VkExtensionProperties> &availableExtensions) {

  for (uint32_t i = 0; i < requiredCount; i++) {
    bool found = false;
    for (const auto &extension : availableExtensions) {
      if (strcmp(requiredExtensions[i], extension.extensionName) == 0) {
        found = true;
        break;
      }
    }
    if (!found) {
      std::cerr << "Missing required extension: " << requiredExtensions[i]
                << std::endl;
      return false;
    }
  }
  return true;
}

// getting the list of required extensions from GLFW
std::vector<const char *> VulkanDevice::getRequiredExtensions() {
  uint32_t glfwExtensionCount = 0;
  const char **glfwExtensions =
      glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  std::vector<const char *> extensions(glfwExtensions,
                                       glfwExtensions + glfwExtensionCount);

  if (ValidationLayers::enable) {
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  return extensions;
}
