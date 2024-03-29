cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

# Project Name -----------
project(G4example02)

# Find Geant4 package, activating all available UI and Vis drivers
find_package(Geant4 REQUIRED ui_all vis_all)

# Setup Geant4 include directories and compile definitions
include(${Geant4_USE_FILE})


# Locate sources and headers for this project
include_directories(${PROJECT_SOURCE_DIR}/include
                    ${Geant4_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

# Copiar las macros y los datos a la carpeta build
file(GLOB MACRO_FILES
	"*.mac")
file(COPY ${MACRO_FILES} DESTINATION ${PROJECT_BINARY_DIR})

file(GLOB DATA_FILES
	"*.dat")
file(COPY ${DATA_FILES} DESTINATION ${PROJECT_BINARY_DIR})

# Add the executable, and link it to the Geant4 libraries
add_executable(exa02 G4example02.cc ${sources} ${headers})
target_link_libraries(exa02 ${Geant4_LIBRARIES})

add_custom_target(G4example02 DEPENDS exa02)

