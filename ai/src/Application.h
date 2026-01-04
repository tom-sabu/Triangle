#ifndef APPLICATION_H

#define APPLICATION_H
#include "Window.h"
#include "VulkanDevice.h"
#include <memory>

class Application {
public:
    Application();
    ~Application();

    void run();

private:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    std::unique_ptr<Window> window;
    std::unique_ptr<VulkanDevice> device;

    void mainLoop();
};

#endif
