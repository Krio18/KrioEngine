cmake_minimum_required(VERSION 3.21)
project(VoxelEngine VERSION 0.1.0 LANGUAGES CXX)

# =============================================================================
# Project Configuration
# =============================================================================

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# =============================================================================
# Build Options
# =============================================================================

option(VOXEL_BUILD_TESTS "Build unit tests" ON)
option(VOXEL_BUILD_BENCHMARKS "Build benchmarks" OFF)
option(VOXEL_BUILD_EDITOR "Build editor application" ON)
option(VOXEL_BUILD_SANDBOX "Build sandbox demo" ON)
option(VOXEL_ENABLE_PROFILING "Enable Tracy profiler" ON)
option(VOXEL_ENABLE_ASAN "Enable AddressSanitizer (Debug only)" OFF)

# =============================================================================
# vcpkg Integration
# =============================================================================

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")
    message(STATUS "Using vcpkg toolchain: ${CMAKE_TOOLCHAIN_FILE}")
endif()

# =============================================================================
# Custom CMake Modules
# =============================================================================

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(cmake/Compiler.cmake OPTIONAL)

# =============================================================================
# Engine Source Files
# =============================================================================

set(VOXEL_ENGINE_SOURCES
    src/main.cpp
    src/Platform/WindowSDL/WindowSDL.cpp
    src/Platform/InputManager/InputManager.cpp
    src/Core/Application/Application.cpp
    src/Core/ServiceLocator/ServiceLocator.cpp
    src/Core/TimeManager/TimeManager.cpp
    src/Core/PhysicsManager/PhysicsManager.cpp
    src/Core/SceneManager/SceneManager.cpp
    src/Core/RenderManager/RenderManager.cpp
    src/Core/Logger/Logger.cpp
    src/Renderer/Shader/Shader.cpp
    src/Renderer/BgfxContext/BgfxContext.cpp
    src/Renderer/ShaderManager/ShaderManager.cpp
    src/Renderer/Mesh/Mesh.cpp
    src/Renderer/Material/Material.cpp
    src/Renderer/MaterialManager/MaterialManager.cpp
    src/Renderer/MeshManager/MeshManager.cpp
    src/Renderer/RendererManager/RendererManager.cpp
    src/Renderer/Camera/Camera.cpp
    src/Renderer/CameraManager/CameraManager.cpp
    src/Renderer/Controllers/FollowCameraController/FollowCameraController.cpp
    src/Renderer/Controllers/FPSCameraController/FPSCameraController.cpp
    src/Renderer/Controllers/OrbitCameraController/OrbitCameraController.cpp
    src/Math/Transform.cpp
)

add_executable(VoxelEngine ${VOXEL_ENGINE_SOURCES})

# =============================================================================
# Compiler Warnings & Optimizations
# =============================================================================

if(MSVC)
    target_compile_options(VoxelEngine PRIVATE /W4 /WX /MP)
    target_compile_options(VoxelEngine PRIVATE $<$<CONFIG:Release>:/O2 /Oi /Ot /GL>)
    target_link_options(VoxelEngine PRIVATE $<$<CONFIG:Release>:/LTCG>)
    target_compile_options(VoxelEngine PRIVATE /wd4251)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(VoxelEngine PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(VoxelEngine PRIVATE $<$<CONFIG:Release>:-O3 -march=native>)  # LTO désactivé pour Linux Makefile
    target_compile_options(VoxelEngine PRIVATE $<$<CONFIG:Debug>:-g3 -O0>)
endif()

# Address Sanitizer
if(VOXEL_ENABLE_ASAN AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(VoxelEngine PRIVATE -fsanitize=address -fno-omit-frame-pointer)
        target_link_options(VoxelEngine PRIVATE -fsanitize=address)
        message(STATUS "AddressSanitizer enabled")
    endif()
endif()

# =============================================================================
# Global Definitions
# =============================================================================

if(WIN32)
    add_compile_definitions(VOXEL_PLATFORM_WINDOWS)
elseif(APPLE)
    add_compile_definitions(VOXEL_PLATFORM_MACOS)
elseif(UNIX)
    add_compile_definitions(VOXEL_PLATFORM_LINUX)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(VOXEL_DEBUG)
else()
    add_compile_definitions(VOXEL_RELEASE)
endif()

if(VOXEL_ENABLE_PROFILING)
    add_compile_definitions(VOXEL_ENABLE_PROFILING)
endif()

# =============================================================================
# External Dependencies
# =============================================================================

# BGFX via bgfx.cmake wrapper (contient bx, bimg)
add_subdirectory(extern/bgfx.cmake)

# =============================================================================
# Shader Compilation
# =============================================================================

include(${CMAKE_SOURCE_DIR}/extern/bgfx.cmake/cmake/bgfxToolUtils.cmake)

set(SHADER_SRC_DIR ${CMAKE_SOURCE_DIR}/shaders/src)
set(SHADER_OUT_DIR ${CMAKE_BINARY_DIR}/shaders)

bgfx_compile_shaders(
    TYPE VERTEX
    SHADERS ${SHADER_SRC_DIR}/v_simple.sc
    VARYING_DEF ${SHADER_SRC_DIR}/varying.def.sc
    OUTPUT_DIR ${SHADER_OUT_DIR}
    INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/extern/bgfx.cmake/bgfx/src
    OUT_FILES_VAR VERTEX_SHADER_OUTPUTS
)

bgfx_compile_shaders(
    TYPE FRAGMENT
    SHADERS ${SHADER_SRC_DIR}/f_simple.sc
    VARYING_DEF ${SHADER_SRC_DIR}/varying.def.sc
    OUTPUT_DIR ${SHADER_OUT_DIR}
    INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/extern/bgfx.cmake/bgfx/src
    OUT_FILES_VAR FRAGMENT_SHADER_OUTPUTS
)

bgfx_compile_shaders(
    TYPE VERTEX
    SHADERS ${SHADER_SRC_DIR}/v_error.sc
    VARYING_DEF ${SHADER_SRC_DIR}/varying.def.sc
    OUTPUT_DIR ${SHADER_OUT_DIR}
    INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/extern/bgfx.cmake/bgfx/src
    OUT_FILES_VAR ERROR_VERTEX_SHADER_OUTPUTS
)

bgfx_compile_shaders(
    TYPE FRAGMENT
    SHADERS ${SHADER_SRC_DIR}/f_error.sc
    VARYING_DEF ${SHADER_SRC_DIR}/varying.def.sc
    OUTPUT_DIR ${SHADER_OUT_DIR}
    INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/extern/bgfx.cmake/bgfx/src
    OUT_FILES_VAR ERROR_FRAGMENT_SHADER_OUTPUTS
)

add_custom_target(Shaders ALL DEPENDS ${VERTEX_SHADER_OUTPUTS} ${FRAGMENT_SHADER_OUTPUTS} ${ERROR_VERTEX_SHADER_OUTPUTS} ${ERROR_FRAGMENT_SHADER_OUTPUTS})
add_dependencies(VoxelEngine Shaders)

# =============================================================================
# Include Directories
# =============================================================================

target_include_directories(VoxelEngine PRIVATE
    src
    extern/bgfx.cmake/bgfx/include
    extern/bgfx.cmake/bx/include
    extern/bgfx.cmake/bimg/include
    extern/glm
    extern/entt/single_include
    extern/spdlog/include
    extern/SDL2/include
    extern/imgui
)

# =============================================================================
# Libraries
# =============================================================================

target_link_libraries(VoxelEngine PRIVATE
    bgfx
    bx
    bimg
)

# SDL2 (trying vcpkg first, fallback to manual)
find_package(SDL2 CONFIG QUIET)

if(SDL2_FOUND)
    target_link_libraries(VoxelEngine PRIVATE SDL2::SDL2 SDL2::SDL2main)
else()
    # Fallback for manual vcpkg installation
    set(SDL2_VCPKG_PATH "$ENV{HOME}/vcpkg/installed/x64-linux")
    if(EXISTS "${SDL2_VCPKG_PATH}")
        target_include_directories(VoxelEngine PRIVATE "${SDL2_VCPKG_PATH}/include")
        target_link_libraries(VoxelEngine PRIVATE
            "${SDL2_VCPKG_PATH}/lib/libSDL2.a"
            "${SDL2_VCPKG_PATH}/lib/libSDL2main.a"
            pthread dl m X11
        )
    else()
        message(FATAL_ERROR "SDL2 not found! Please install via vcpkg: vcpkg install sdl2")
    endif()
endif()

# =============================================================================
# Install Configuration
# =============================================================================

include(GNUInstallDirs)

install(TARGETS VoxelEngine
    EXPORT VoxelEngineTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY src/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VoxelEngine
    FILES_MATCHING PATTERN "*.hpp"
)

install(EXPORT VoxelEngineTargets
    FILE VoxelEngineTargets.cmake
    NAMESPACE Voxel::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/VoxelEngine
)

# =============================================================================
# Summary
# =============================================================================

message(STATUS "")
message(STATUS "=================================")
message(STATUS "VoxelEngine Configuration Summary")
message(STATUS "=================================")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "")
message(STATUS "Build options:")
message(STATUS "  Build Tests: ${VOXEL_BUILD_TESTS}")
message(STATUS "  Build Benchmarks: ${VOXEL_BUILD_BENCHMARKS}")
message(STATUS "  Build Editor: ${VOXEL_BUILD_EDITOR}")
message(STATUS "  Build Sandbox: ${VOXEL_BUILD_SANDBOX}")
message(STATUS "  Enable Profiling: ${VOXEL_ENABLE_PROFILING}")
message(STATUS "  Enable ASAN: ${VOXEL_ENABLE_ASAN}")
message(STATUS "=================================")
message(STATUS "")
