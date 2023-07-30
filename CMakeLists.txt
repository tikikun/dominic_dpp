cmake_minimum_required(VERSION 3.5)
project(gcc_dominic_dpp CXX) # Replace "MyProject" with the actual name of your project

# Enforce C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set runtime output dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Define the sources of the project
file(GLOB_RECURSE MAIN_SRC src/*.cc)

# Add the main executable
add_executable(${PROJECT_NAME} ${MAIN_SRC})

# Specify the include directories for the dpp library (if needed)
# include_directories(path/to/dpp/includes)

# Specify the link directories for the dpp library (if needed)
# link_directories(path/to/dpp/libraries)

# Link the dpp library to the main executable
target_link_libraries(${PROJECT_NAME} PRIVATE dpp) # Replace "dpp" with the actual name of the dpp library (without the 'lib' prefix and file extension)