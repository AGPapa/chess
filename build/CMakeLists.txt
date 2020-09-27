cmake_minimum_required(VERSION 3.1)
 
# Set C++11
set (CMAKE_CXX_STANDARD 11)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
 
# Link runTests with what we want to test and the GTest and pthread library
add_executable(runBitBoardTests ../bitboard_test.cpp)
add_executable(runSquareTests ../square_test.cpp)
add_executable(runBoard ../board.cpp)
target_link_libraries(runBitBoardTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(runSquareTests ${GTEST_LIBRARIES} pthread)