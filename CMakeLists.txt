# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name
project(UltimateTrackerReader)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    # "C:/Users/kuloz/Documents/Codes_Windows/CppCodes_public/OpenVR/include"
    ${CMAKE_SOURCE_DIR}/../../OpenVR/headers
)

# Add source files
set(SOURCES
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/MainProgram.cpp
)

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCES})


# Link libraries
target_link_libraries(${PROJECT_NAME}
    "C:/Users/kuloz/Documents/Codes_Windows/CppCodes_public/OpenVR/lib/win64/openvr_api.lib"
)

# Copy the executable to a bin directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Post-build step: Copy dependencies (optional)
# You can add commands here to copy any necessary DLLs or assets to the bin directory.
