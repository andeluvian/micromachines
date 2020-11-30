cmake_minimum_required(VERSION 3.1.0)

# Check that the build does not happen in the source tree
# i.e. first you have to create a build directory and navigate there
# then run cmake ..
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
   MESSAGE(FATAL_ERROR "Prevented in-tree built. This is bad practice.")
endif(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})


# Enable debug symbols by default
# must be done before project() statement
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()
# (you can also set it on the command line: -D CMAKE_BUILD_TYPE=Release)

# executable name
project(app)

#SET(GCC_COVERAGE_COMPILE_FLAGS "-g -Wall -Wno-missing-field-initializers")
#add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})


# specify c++ standard (-std=c++14)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set version information in a config.h file
set(app_VERSION_MAJOR 1)
set(app_VERSION_MINOR 0)
include_directories("${PROJECT_BINARY_DIR}" "${CMAKE_SOURCE_DIR}/include")

# Define sources and executable
set(EXECUTABLE_NAME "app")
file(GLOB SOURCES "src/*.cpp")
add_executable(app ${SOURCES})

# specify where FindSFML.cmake is located
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

if (WIN32)
    message("Windows detected.")
    set(SFML_ROOT "${CMAKE_SOURCE_DIR}/SFML")
else()
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Wall -Wno-missing-field-initializers")
endif()
    
# Detect and add SFML
#Find any version 2.X of SFML
#See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2 REQUIRED network audio graphics window system)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
    link_directories(${CMAKE_SOURCE_DIR}/SFML/dll)
else()
    message("SFML not found!")
endif()

# Install target
install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)

# CPack packaging
include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_VERSION_MAJOR "${app_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${app_VERSION_MINOR}")
include(CPack)

# execute the binary by typing make run
add_custom_target(run
    COMMAND app
    DEPENDS app
)

add_custom_target(docs
    COMMAND cd ../../doc/ && doxygen ../src/Doxyfile
    COMMENT "Generating Doxygen documentation"
)



