#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "libglew_static" for configuration "Release"
set_property(TARGET libglew_static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(libglew_static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C;RC"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/glew.lib"
  )

list(APPEND _cmake_import_check_targets libglew_static )
list(APPEND _cmake_import_check_files_for_libglew_static "${_IMPORT_PREFIX}/lib/glew.lib" )

# Import target "libglew_shared" for configuration "Release"
set_property(TARGET libglew_shared APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(libglew_shared PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/glew-shared.lib"
  )

list(APPEND _cmake_import_check_targets libglew_shared )
list(APPEND _cmake_import_check_files_for_libglew_shared "${_IMPORT_PREFIX}/lib/glew-shared.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
