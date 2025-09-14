
cmake_minimum_required(VERSION 3.8)

project(NumericalIntegrate)

find_package(ROOT REQUIRED COMPONENTS RIO)

set(sources 
    integrators.cpp
    test_integration.cpp
)

set(include 
    integrators.hpp    
)

add_executable(test_integration ${sources} ${include})

target_link_libraries(test_integration PUBLIC ROOT::Core)

target_include_directories(test_integration PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")