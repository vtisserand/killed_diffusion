cmake_minimum_required(VERSION 3.12)
project(killed_diffusion)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Add pybind11
add_subdirectory(pybind11)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Define the library
pybind11_add_module(killed_diffusion src/wrapper.cpp src/montecarlo.cpp src/closed_formula.cpp)

# Link the library
target_link_libraries(killed_diffusion PRIVATE pybind11::module)