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
#set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -Wno-int-in-bool-context")

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

find_library(GMP_LIBRARY NAMES gmp)
set(EIGEN_INCLUDE /usr/include/eigen3)

find_package(Boost COMPONENTS serialization REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include ${EIGEN_INCLUDE})
add_library(tensor-algebra SHARED src/Indices.cpp src/Rational.cpp src/Tensor.cpp src/TensorMonomial.cpp src/Scalar.cpp src/ScalarSum.cpp src/MonomialExpression.cpp src/Expression.cpp src/Generator.cpp src/MyPermutation.cpp src/NumericSimplify.cpp src/ApplyGaugeSymmetry.cpp)
add_executable(ansatz src/ansatz.cpp)
add_executable(area src/area.cpp)
add_executable(area_epsilon src/area_epsilon.cpp)
add_executable(3plus1_ab src/3plus1_ab.cpp)
add_executable(3plus1_abc src/3plus1_abc.cpp)
add_executable(3plus1_abcd src/3plus1_abcd.cpp)
add_executable(metric src/metric.cpp)
add_executable(test_all src/test/RationalTest.cpp src/test/ScalarTest.cpp src/test/ScalarSumTest.cpp src/test/IndicesTest.cpp src/test/TensorTest.cpp src/test/TensorMonomialTest.cpp src/test/MonomialExpressionTest.cpp src/test/ExpressionTest.cpp src/test/CalculationsTest.cpp)
add_test(NAME test_all COMMAND test_all)
target_link_libraries(test_all gtest_main tensor-algebra ${GMP_LIBRARY} ${Boost_LIBRARIES})
target_link_libraries(ansatz tensor-algebra ${GMP_LIBRARY} ${Boost_LIBRARIES})
target_link_libraries(area tensor-algebra ${GMP_LIBRARY} ${Boost_LIBRARIES})
target_link_libraries(area_epsilon tensor-algebra ${GMP_LIBRARY} ${Boost_LIBRARIES})
target_link_libraries(3plus1_ab tensor-algebra ${GMP_LIBRARY} ${Boost_LIBRARIES})
target_link_libraries(3plus1_abc tensor-algebra ${GMP_LIBRARY} ${Boost_LIBRARIES})
target_link_libraries(3plus1_abcd tensor-algebra ${GMP_LIBRARY} ${Boost_LIBRARIES})
target_link_libraries(metric tensor-algebra ${GMP_LIBRARY} ${Boost_LIBRARIES})
