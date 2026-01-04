#include "ValidationLayers.h"
#include <iostream>
#include <cstring>

const std::vector<const char*> ValidationLayers::validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

bool ValidationLayers::checkSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    std::cout << "Available validation layers:\n";
    for (const auto& layer : availableLayers) {
        std::cout << '\t' << layer.layerName << '\n';
    }

    for (const auto& layerName: validationLayers) {
        bool layerFound = false;
        
        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }
        
        if (!layerFound) {
            std::cerr << "Missing validation layer: " << layerName << std::endl;
        }
    }

    return true;
}

