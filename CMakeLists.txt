# CMakeList.txt : CMake project for DecoderVis, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("DecoderVis")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(${PROJECT_SOURCE_DIR}/decoder)
add_subdirectory(${PROJECT_SOURCE_DIR}/external/glew)
add_subdirectory(${PROJECT_SOURCE_DIR}/external/glfw)
add_subdirectory(${PROJECT_SOURCE_DIR}/external/lemon)
add_subdirectory(${PROJECT_SOURCE_DIR}/external/glm EXCLUDE_FROM_ALL)

target_include_directories(DecoderVis PRIVATE
    ${PROJECT_SOURCE_DIR}/decoder/logic/include
    ${PROJECT_SOURCE_DIR}/decoder/render/include
    ${PROJECT_SOURCE_DIR}/external/glm
    ${PROJECT_SOURCE_DIR}/external/imgui
    ${PROJECT_SOURCE_DIR}/external/stb
)

target_include_directories(DecoderVis PUBLIC
    ${PROJECT_SOURCE_DIR}/external/glew/include
    ${PROJECT_SOURCE_DIR}/external/glfw/include
    ${PROJECT_SOURCE_DIR}/external/lemon
    ${PROJECT_SOURCE_DIR}/out/build/x64-debug/external/lemon
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET DecoderVis PROPERTY CXX_STANDARD 20)
endif()