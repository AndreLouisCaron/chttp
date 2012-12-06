==============================================
  ``chttp``: Simple buffer for HTTP headers.
==============================================
:authors:
   André Caron
:contact: andre.l.caron@gmail.com


Description
===========

This library provides a simple utility for buffering HTTP headers.


Getting started
===============

The library is currently only distributed in source form.  However, it has no
external dependencies and will compile as-is with almost any C compiler.

The following presents the *supported* way to get up and running with
``chttp``.  Feel free to experiment with your toolchain of choice.

Requirements
------------

Dependencies are:

#. Git_
#. CMake_
#. Doxygen_
#. A C++ compiler toolchain:

   * Microsoft Visual Studio
   * ``g++`` and ``make``

.. _Git: http://git-scm.com/
.. _CMake: http://www.cmake.org/
.. _Doxygen: http://www.stack.nl/~dimitri/doxygen/

Standalone build
----------------

#. Get the source code.

   ::

      > git clone git://github.com/AndreLouisCaron/chttp.git
      > cd chttp
      > git submodule init
      > git submodule update

   Feel free to check out a specific version:

   ::

      > git tag
      v0.1

      > git checkout v0.1

#. Generate the build scripts.

   ::

      > mkdir work
      > cd work
      > cmake -G "NMake Makefiles" ..

#. Build the source code.

   ::

      > nmake

#. Run the test suite.

   ::

      > nmake /A test

#. Build the API documentation.

   ::

      > nmake help

   Open the HTML documentation in ``work/help/html/index.html``.

Embedded build
--------------

#. Register ``chttp`` as a Git sub-module.

   ::

      > cd myproject
      > git submodule add git://github.com/AndreLouisCaron/chttp.git libs/chttp

   Feel free to check out a specific version.

   ::

      > cd libs/chttp
      > git tag
      v0.1

      > git checkout v0.1
      > cd ../..
      > git add libs/chttp

#. Add ``chttp`` targets to your CMake project.

   ::

      set(chttp_DIR
        ${CMAKE_SOURCE_DIR}/libs/chttp
      )

#. Make sure your CMake project can ``#include <head.h>``.

   ::

      include_directories(
        ${chttp_include_dirs}
      )


#. Link against the ``http`` library.

   ::

      target_link_libraries(my-application ${chttp_libraries})
