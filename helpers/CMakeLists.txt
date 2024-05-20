file(GLOB "INC" include/helpers/*.h)
file(GLOB "SRC" src/*.cpp)

add_library(gui-helpers ${SRC} ${INC})
target_include_directories(gui-helpers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(gui-helpers PUBLIC sfml-graphics)
target_compile_features(gui-helpers PUBLIC cxx_std_20)