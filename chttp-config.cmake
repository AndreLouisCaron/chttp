# Library configuration file used by dependent projects
# via find_package() built-in directive in "config" mode.

if(NOT DEFINED chttp_FOUND)

  # Locate library headers.
  find_path(chttp_include_dir
    NAMES head.h
    PATHS ${chttp_DIR}/code
  )

  # Export library targets.
  set(chttp_libraries
    chttp
    CACHE INTERNAL "chttp library"
  )

  # Usual "required" et. al. directive logic.
  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(
    chttp DEFAULT_MSG
    chttp_include_dir
    chttp_libraries
  )

  # Register library targets when found as part of a dependent project.
  # Since this project uses a find_package() directive to include this
  # file, don't recurse back into the CMakeLists file.
  if(NOT "${PROJECT_NAME}" STREQUAL "chttp")
    add_subdirectory(
      ${chttp_DIR}
      ${CMAKE_CURRENT_BINARY_DIR}/chttp
    )
  endif()
endif()
