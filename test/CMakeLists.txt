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

# Simple macro to generate consistent test cases.  Each test case is a
# standalone program based on "chttp" with the following contraints:
# - the program is a contained in a single source file;
# - the program requires no dependencies other than "chttp";
# - the program runs without command-line arguments;
# - the program returns a non-zero process status to indicate failure.
macro(add_test_program name)
  # Build the test program.
  add_executable(${name} ${name}.c)
  add_dependencies(${name} chttp)
  target_link_libraries(${name} ${chttp_libraries})

  # Group tests in a "test" folder.
  set_target_properties(${name} PROPERTIES FOLDER test)

  # Register the test.
  add_test(
    NAME    ${name}
    COMMAND ${name}
  )
endmacro()

# Make sure all demo programs successfully terminate.
add_test(NAME test-demo-c   COMMAND demo-c)
add_test(NAME test-demo-c++ COMMAND demo-c++)

# Register several test programs.
add_test_program(test-partial-push-success)
add_test_program(test-partial-push-field-overflow)
add_test_program(test-partial-push-value-overflow)
