cmake_minimum_required(VERSION 3.20)
project(VulkanTriangle)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(VulkanTriangle ${SOURCES})

target_include_directories(VulkanTriangle PRIVATE src)
target_link_libraries(VulkanTriangle PRIVATE Vulkan::Vulkan glfw)
