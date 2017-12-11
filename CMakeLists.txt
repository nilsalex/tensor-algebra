cmake_minimum_required(VERSION 3.9)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

project(tensor-algebra)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

find_library(GMP_LIBRARY NAMES gmp)
set(EIGEN_INCLUDE /usr/include/eigen3)

include_directories(${CMAKE_SOURCE_DIR}/include ${EIGEN_INCLUDE})
add_library(tensor-algebra SHARED src/Indices.cpp src/Rational.cpp src/Tensor.cpp src/TensorMonomial.cpp src/Scalar.cpp src/ScalarSum.cpp src/MonomialExpression.cpp src/Expression.cpp src/Generator.cpp src/MyPermutation.cpp src/NumericSimplify.cpp)
add_executable(main src/main.cpp)
target_link_libraries(main tensor-algebra ${GMP_LIBRARY})
add_executable(rational_test src/test/RationalTest.cpp)
target_link_libraries(rational_test gtest_main tensor-algebra ${GMP_LIBRARY})
add_test(NAME rational_test COMMAND rational_test)
