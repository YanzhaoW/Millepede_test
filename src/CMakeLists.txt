add_library(builder EventBuilder.cpp Detector.cpp)
target_include_directories(builder PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(builder PUBLIC ROOT::MathCore fmt::fmt Mille
                                     nlohmann_json::nlohmann_json)
