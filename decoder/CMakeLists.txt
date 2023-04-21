

cmake_minimum_required(VERSION 3.8)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/decoder/render/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/decoder/logic/src/*.cpp"
	"${PROJECT_SOURCE_DIR}/external/imgui/imgui/*.cpp"
	"${PROJECT_SOURCE_DIR}/external/stb/*.cpp"
	"${PROJECT_SOURCE_DIR}/external/lemon/lemon/*.cpp"
	"${PROJECT_SOURCE_DIR}/external/lemon/lemon/bits/*.cpp"
)

add_executable(DecoderVis "app.cpp" ${SRC_FILES} "logic/include/RotatedPlanarDecoder.h")

target_link_libraries(DecoderVis PUBLIC
	glfw
	libglew_static
	glm
	lemon 
)