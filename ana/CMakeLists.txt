# $Id: CMakeLists.txt 104871 2017-06-23 14:17:13Z gcosmo $

#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 3.16...3.21)
project(Ana)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -Wpedantic -Wno-shadow)

# #~~~~~~~~~~~~~~~~~~~~
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

# #---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS RIO)

# #---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

include_directories(${ROOT_INCLUDE_DIRS})
# #~~~~~~~~~~~~~~~~~~~~

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
# include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)
#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
# file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
# file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
# add_executable(Ana ${PROJECT_SOURCE_DIR}/ana.cc ${sources} ${headers})
add_executable(Ana ${PROJECT_SOURCE_DIR}/ana.cc ${sources} ${headers})
target_link_libraries(Ana ${ROOT_LIBRARIES})

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS Ana DESTINATION bin)