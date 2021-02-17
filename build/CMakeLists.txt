cmake_minimum_required(VERSION 3.1)

project(Chess)
 
# Set C++11
set (CMAKE_CXX_STANDARD 11)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
 
# Link runTests with what we want to test and the GTest and pthread library
add_executable(runBitBoardTests ../src/test/chess/bitboard_test.cpp)
add_executable(runSquareTests ../src/test/chess/square_test.cpp)
add_executable(runCastlingTests ../src/test/chess/castling_test.cpp)
add_executable(runBoardTests ../src/test/chess/board_test.cpp)
add_executable(runPlyTests ../src/test/chess/ply_test.cpp)
add_executable(runPieceAttacksTests ../src/test/chess/piece_attacks_test.cpp)
add_executable(runNodeTests ../src/test/search/node_test.cpp)
add_executable(runSearcherTests ../src/test/search/searcher_test.cpp)
target_link_libraries(runBitBoardTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(runSquareTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(runCastlingTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(runBoardTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(runPlyTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(runPieceAttacksTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(runNodeTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(runSearcherTests ${GTEST_LIBRARIES} pthread)