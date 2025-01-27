cmake_minimum_required(VERSION 3.10)
project(ParallelMachineProblem)

set(CMAKE_CXX_STANDARD 17)

include_directories(headers)

add_executable(ParallelMachineProblem src/Parallel-machine.cc src/Machine.cc src/Problem.cc src/Algorithms/Greedy.cc src/Algorithms/GRASP.cc src/Algorithms/GRASP-min.cc src/Environment/ReInsertionIntra.cc src/Environment/SwapIntra.cc src/Environment/ReInsertionInter.cc src/Environment/SwapInter.cc src/Algorithms/MultiGVNS.cc)

target_link_libraries(ParallelMachineProblem)