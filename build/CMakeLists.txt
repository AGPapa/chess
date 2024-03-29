cmake_minimum_required(VERSION 3.1)

project(Chess)
 
# Set C++11
set (CMAKE_CXX_STANDARD 11)

# Set vector
if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm64")
  set(CMAKE_CXX_FLAGS "-DUSE_NEON")
#  set(CMAKE_CXX_FLAGS "-mfpu=neon") # everything seems to work fine without this?
endif()

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
 
# Link runTests with what we want to test and the GTest and pthread library
add_executable(runBitBoardTests ../src/test/chess/bitboard_test.cpp)
add_executable(runSquareTests ../src/test/chess/square_test.cpp)
add_executable(runCastlingTests ../src/test/chess/castling_test.cpp)
add_executable(runBoardSignatureTests ../src/test/chess/board_signature_test.cpp)
add_executable(runBoardTests ../src/test/chess/board_test.cpp)
add_executable(runPlyTests ../src/test/chess/ply_test.cpp)
add_executable(runPieceAttacksTests ../src/test/chess/piece_attacks_test.cpp)
add_executable(runActionTests ../src/test/evaluation/action_test.cpp)
add_executable(runPolicyTests ../src/test/evaluation/policy_test.cpp)
add_executable(runEvaluatorTests ../src/test/evaluation/evaluator_test.cpp)
add_executable(runActivationLayerTests ../src/test/evaluation/activation_layer_test.cpp)
add_executable(runWeightsLayerTests ../src/test/evaluation/weights_layer_test.cpp)
add_executable(runConcatenationLayerTests ../src/test/evaluation/concatenation_layer_test.cpp)
add_executable(runTransformationLayerTests ../src/test/evaluation/transformation_layer_test.cpp)
add_executable(runOutputLayerTests ../src/test/evaluation/output_layer_test.cpp)
add_executable(runNeuralNetTests ../src/test/evaluation/neural_net_test.cpp)
add_executable(runCacheTests ../src/test/utility/cache_test.cpp)
add_executable(runRingbufferQueueTests ../src/test/utility/ringbuffer_queue_test.cpp)
add_executable(runUCITests ../src/test/interface/uci_test.cpp)
add_executable(runNodeTests ../src/test/search/node_test.cpp)
add_executable(runSearcherTests ../src/test/search/searcher_test.cpp)
add_executable(runExpandJobTests ../src/test/search/expand_job_test.cpp)
add_executable(run ../src/interface/main.cpp)
target_link_libraries(runBitBoardTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(runSquareTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(runCastlingTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(runBoardSignatureTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(runBoardTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(runPlyTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(runPieceAttacksTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(runActionTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(runPolicyTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(runEvaluatorTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(runActivationLayerTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(runWeightsLayerTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(runConcatenationLayerTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(runTransformationLayerTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(runCacheTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(runOutputLayerTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(runNeuralNetTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(runRingbufferQueueTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(runUCITests ${GTEST_LIBRARIES} pthread)
target_link_libraries(runNodeTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(runSearcherTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(runExpandJobTests ${GTEST_LIBRARIES} pthread)