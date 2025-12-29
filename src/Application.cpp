#include "Application.h"
#include <iostream>

Application::Application() {
    window = std::make_unique<Window>(WIDTH, HEIGHT, "Vulkan Triangle");
    device = std::make_unique<VulkanDevice>(*window);
}

Application::~Application() {}

void Application::run() {
    mainLoop();
}

void Application::mainLoop() {
    std::cout << "Window should be open now..." << std::endl;

    while (!window->shouldClose()) {
        window->pollEvents();
    }

    std::cout << "Window closed." << std::endl;
}
