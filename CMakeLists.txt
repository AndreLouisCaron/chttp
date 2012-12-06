# Copyright (c) 2012, Andre Caron (andre.l.caron@gmail.com)
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#  * Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
#
#  * Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

cmake_minimum_required(VERSION 2.6)

project(chttp)

# Ignore Microsoft's warnings about standard functions.
if(MSVC)
  add_definitions(
    -D_CRT_SECURE_NO_WARNINGS
    -D_SCL_SECURE_NO_WARNINGS
    -D_CRT_NONSTDC_NO_DEPRECATE
  )
endif()

# Put all libraries and executables in the build folder root.
set(LIBRARY_OUTPUT_PATH    "${PROJECT_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}")

# Allow use of folders to group related targets.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Build the primary target.
add_subdirectory(code)

# Optional targets (skip when building as a dependency).
if("${PROJECT_NAME}" STREQUAL "${CMAKE_PROJECT_NAME}")

  # Locate the library & include paths just like our clients.
  find_package(chttp
    PATHS "${PROJECT_SOURCE_DIR}"
  )

  # Build demo projects.
  include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/code
  )
  add_subdirectory(demo)

  # Add a "help" target to build API documentation.
  find_package(Doxygen QUIET)
  if(PROJECT_VERSION AND NOT ("${PROJECT_VERSION}" STREQUAL "dev"))
    set(DOXYGEN_SHOW_INTERNALS NO)
    set(DOXYGEN_HIDE_INTERNALS YES)
  else()
    set(PROJECT_VERSION "dev")
    set(DOXYGEN_SHOW_INTERNALS YES)
    set(DOXYGEN_HIDE_INTERNALS NO)
  endif()
  configure_file(
    ${PROJECT_SOURCE_DIR}/Doxyfile
    ${PROJECT_BINARY_DIR}/help/Doxyfile @ONLY
  )
  if(DOXYGEN_EXECUTABLE)
    add_custom_target(
      help
      COMMAND
        ${DOXYGEN_EXECUTABLE}
      WORKING_DIRECTORY
        ${PROJECT_BINARY_DIR}/help
      COMMENT
        "Compiling documentation."
      VERBATIM
    )
  endif()

  # Register test suite.
  enable_testing()
  add_subdirectory(test)

endif()
