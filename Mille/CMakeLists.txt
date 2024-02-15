add_library(Mille SHARED Mille.cpp PedeResReader.cpp)
target_include_directories(Mille PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS "boost dir: ${Boost_INCLUDE_DIRS}")
target_link_libraries(Mille PRIVATE fmt::fmt range-v3::range-v3
                                    ${Boost_LIBRARIES})
