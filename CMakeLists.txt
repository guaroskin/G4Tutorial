#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
#----------------------------------------------------------------------------
# Project Name
#
project(G4example02)

# Find Geant4 package, activating all available UI and Vis drivers
#
find_package(Geant4 REQUIRED ui_all vis_all)

# Setup Geant4 include directories and compile definitions
include(${Geant4_USE_FILE})


#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
#include_directories(${PROJECT_SOURCE_DIR}/include
#                    ${Geant4_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)


# Add the executable, and link it to the Geant4 libraries
add_executable(exa02 G4example02.cc ${sources} ${headers})
target_link_libraries(exa02 ${Geant4_LIBRARIES})

add_custom_target(G4example02 DEPENDS exa02)

