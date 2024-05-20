file(GLOB "INC" include/gui/*.h)
file(GLOB "SRC" src/*.cpp)

add_executable(tabletop-game ${SRC} ${INC})
target_include_directories(tabletop-game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(tabletop-game PUBLIC gui-helpers)
target_compile_features(tabletop-game PUBLIC cxx_std_20)

if(WIN32)
    add_custom_command(
        TARGET tabletop-game
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:tabletop-game>
        VERBATIM)
endif()

install(TARGETS tabletop-game)