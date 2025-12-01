# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "C:/Users/nberk/Desktop/Projeler/QT/GStreamerGUI/build_release/_deps/gstreamer_lib-src")
  file(MAKE_DIRECTORY "C:/Users/nberk/Desktop/Projeler/QT/GStreamerGUI/build_release/_deps/gstreamer_lib-src")
endif()
file(MAKE_DIRECTORY
  "C:/Users/nberk/Desktop/Projeler/QT/GStreamerGUI/build_release/_deps/gstreamer_lib-build"
  "C:/Users/nberk/Desktop/Projeler/QT/GStreamerGUI/build_release/_deps/gstreamer_lib-subbuild/gstreamer_lib-populate-prefix"
  "C:/Users/nberk/Desktop/Projeler/QT/GStreamerGUI/build_release/_deps/gstreamer_lib-subbuild/gstreamer_lib-populate-prefix/tmp"
  "C:/Users/nberk/Desktop/Projeler/QT/GStreamerGUI/build_release/_deps/gstreamer_lib-subbuild/gstreamer_lib-populate-prefix/src/gstreamer_lib-populate-stamp"
  "C:/Users/nberk/Desktop/Projeler/QT/GStreamerGUI/build_release/_deps/gstreamer_lib-subbuild/gstreamer_lib-populate-prefix/src"
  "C:/Users/nberk/Desktop/Projeler/QT/GStreamerGUI/build_release/_deps/gstreamer_lib-subbuild/gstreamer_lib-populate-prefix/src/gstreamer_lib-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/nberk/Desktop/Projeler/QT/GStreamerGUI/build_release/_deps/gstreamer_lib-subbuild/gstreamer_lib-populate-prefix/src/gstreamer_lib-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/nberk/Desktop/Projeler/QT/GStreamerGUI/build_release/_deps/gstreamer_lib-subbuild/gstreamer_lib-populate-prefix/src/gstreamer_lib-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
