#include "Window.h"
#include <iostream>
#include <stdexcept>

Window::Window(int width, int height, const std::string &title)
    : width(width), height(height), title(title), window(nullptr) {
  initWindow();
}

Window::~Window() {
    if (window != nullptr) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}

void Window::initWindow() {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    
    if (window == nullptr) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(window);
}

void Window::pollEvents() {
    glfwPollEvents();
}
