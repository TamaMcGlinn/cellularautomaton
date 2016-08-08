cmake_minimum_required(VERSION 2.8)

# Name your program!
set (App_Name "CA")
if (App_Name STREQUAL "")
  message (FATAL_ERROR "You must set the App_Name variable!")
endif ()

add_definitions(-std=c++0x)
#add_definitions(-g) #Debug with gdb

# Every project must have a name.
project (${App_Name})

################################################################################
# Ensure that we are not building in our source directories.

set (Build_Dir_OK "TRUE")
string (REGEX MATCH "^${CMAKE_SOURCE_DIR}" In_Sub_Dir ${CMAKE_BINARY_DIR})
if (In_Sub_Dir)
  string (REGEX MATCH "^${CMAKE_SOURCE_DIR}/build" In_Build_Dir ${CMAKE_BINARY_DIR})
  if (NOT In_Build_Dir)
    set (Build_Dir_OK "FALSE")
  endif ()
endif ()

if (NOT Build_Dir_OK)
  message (FATAL_ERROR "You must run cmake from a directory that is not in your source tree, or that is in a special subdirectory of the tree whose name begins with ‘build’.")
endif ()

################################################################################
# Set up the basic build environment

if (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This messes up
  # differentiation between debug and release builds.              
  set (CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()

################################################################################
# The core project files

file (GLOB SRCS src/*.c src/*.cpp)
file (GLOB HDRS include/*.h include/*.hpp)

link_directories (
  )

include_directories (
  ${CMAKE_SOURCE_DIR}/include
  )

if (WIN32)
  add_executable (${App_Name} WIN32
    ${SRCS}
    ${HDRS}
    )
else ()
  add_executable (${App_Name}
    ${SRCS}
    ${HDRS}
    )
endif ()

find_package( OpenCV REQUIRED )

target_link_libraries (${App_Name}
${OpenCV_LIBS}
)
