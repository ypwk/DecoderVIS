# CMAKE generated file: DO NOT EDIT!
# Generated by CMake Version 3.25
cmake_policy(SET CMP0009 NEW)

# SRC_FILES at CMakeLists.txt:18 (file)
file(GLOB_RECURSE NEW_GLOB LIST_DIRECTORIES false "C:/Users/kevin/Programming/DecoderVis/decoder/logic/src/*.cpp")
set(OLD_GLOB
  "C:/Users/kevin/Programming/DecoderVis/decoder/logic/src/RotatedPlanarCode.cpp"
  "C:/Users/kevin/Programming/DecoderVis/decoder/logic/src/decoder.cpp"
  "C:/Users/kevin/Programming/DecoderVis/decoder/logic/src/simulation.cpp"
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "C:/Users/kevin/Programming/DecoderVis/out/build/x64-debug/CMakeFiles/cmake.verify_globs")
endif()

# SRC_FILES at CMakeLists.txt:18 (file)
file(GLOB_RECURSE NEW_GLOB LIST_DIRECTORIES false "C:/Users/kevin/Programming/DecoderVis/decoder/render/src/*.cpp")
set(OLD_GLOB
  "C:/Users/kevin/Programming/DecoderVis/decoder/render/src/central_node_handler.cpp"
  "C:/Users/kevin/Programming/DecoderVis/decoder/render/src/engine.cpp"
  "C:/Users/kevin/Programming/DecoderVis/decoder/render/src/ig_contentwindow.cpp"
  "C:/Users/kevin/Programming/DecoderVis/decoder/render/src/ig_window.cpp"
  "C:/Users/kevin/Programming/DecoderVis/decoder/render/src/index_buffer.cpp"
  "C:/Users/kevin/Programming/DecoderVis/decoder/render/src/input.cpp"
  "C:/Users/kevin/Programming/DecoderVis/decoder/render/src/renderer.cpp"
  "C:/Users/kevin/Programming/DecoderVis/decoder/render/src/shader.cpp"
  "C:/Users/kevin/Programming/DecoderVis/decoder/render/src/texture.cpp"
  "C:/Users/kevin/Programming/DecoderVis/decoder/render/src/vertex_array.cpp"
  "C:/Users/kevin/Programming/DecoderVis/decoder/render/src/vertex_buffer.cpp"
  "C:/Users/kevin/Programming/DecoderVis/decoder/render/src/vertex_buffer_layout.cpp"
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "C:/Users/kevin/Programming/DecoderVis/out/build/x64-debug/CMakeFiles/cmake.verify_globs")
endif()
