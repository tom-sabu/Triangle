cmake_minimum_required(VERSION 4.2.1)
project(VulkanTriangle)

option(USE_AI_SRC "Build using AI sandbox sources" OFF)

if(USE_AI_SRC)
    message(STATUS "Building using AI sandbox sources (ai/src)")
    set(SRC_ROOT "${CMAKE_SOURCE_DIR}/ai/src")
else()
    message(STATUS "Building using real sources (src)")
    set(SRC_ROOT "${CMAKE_SOURCE_DIR}/src")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

set(SOURCE
    ${SRC_ROOT}/main.cpp
    ${SRC_ROOT}/Application.cpp
    ${SRC_ROOT}/Window.cpp
    ${SRC_ROOT}/ValidationLayers.cpp
    ${SRC_ROOT}/VulkanDevice.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE})

target_include_directories(${PROJECT_NAME} PRIVATE ${SRC_ROOT})
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan glfw)
