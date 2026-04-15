cmake_minimum_required(VERSION 3.16)

# Force compatibility with older CMake files for newer CMake 4.x versions
set(CMAKE_POLICY_VERSION_MINIMUM 3.10)

# Policy for Git submodules
cmake_policy(SET CMP0097 NEW)

project(CryptoLabSuite VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# 1. Fetch GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# 2. Fetch Dear ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG master
)
FetchContent_MakeAvailable(imgui)

# 3. Fetch Crypto++ (Official CMake Wrapper with correct Tag)
FetchContent_Declare(
    cryptopp_cmake 
    GIT_REPOSITORY https://github.com/abdes/cryptopp-cmake.git
    GIT_TAG CRYPTOPP_8_9_0  # <-- Sửa lại chính xác tên tag này
    GIT_SUBMODULES_RECURSE ON
)
set(CRYPTOPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(CRYPTOPP_BUILD_SHARED OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(cryptopp_cmake)

# 4. Fetch Portable File Dialogs (For the "Browse" buttons)
FetchContent_Declare(
    pfd
    GIT_REPOSITORY https://github.com/samhocevar/portable-file-dialogs.git
    GIT_TAG main
)
FetchContent_MakeAvailable(pfd)

# 5. Setup Project Sources
set(IMGUI_DIR ${imgui_SOURCE_DIR})
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

set(PROJECT_SOURCES
    src/main_imgui.cpp
    src/core/CryptoService.h
    src/core/CryptoService.cpp
    src/ui/ImGuiApp.h
    src/ui/ImGuiApp.cpp
    ${IMGUI_SOURCES}
)

# 6. Create Executable
add_executable(CryptoLabSuite ${PROJECT_SOURCES})

# 7. Include Directories
target_include_directories(CryptoLabSuite PRIVATE 
    src/
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${pfd_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}/_deps/cryptopp_cmake-build
)

# 8. Linking
find_package(OpenGL REQUIRED)

target_link_libraries(CryptoLabSuite PRIVATE 
    glfw 
    OpenGL::GL 
    cryptopp::cryptopp
)
