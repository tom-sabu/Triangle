#ifndef WINDOW_H

#define WINDOW_H
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

class Window {
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    bool shouldClose() const;
    void pollEvents();
    GLFWwindow* getGLFWWindow() const { return window; }
    int getWidth() const { return width; }
    int gitHeight() const { return height; }

private:
    GLFWwindow* window;
    int width;
    int height;
    std::string title;
    
    void initWindow();
};

#endif
