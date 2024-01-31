add_library(Mille SHARED Mille.cpp)
target_include_directories(Mille PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Mille PRIVATE fmt::fmt range-v3::range-v3)
