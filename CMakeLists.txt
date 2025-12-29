cmake_minimum_required(VERSION 4.2.1)
project(VulkanTriangle)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

set(SOURCE
    src/main.cpp
    src/Application.cpp
    src/Window.cpp
    src/VulkanDevice.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE})

target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan glfw)
