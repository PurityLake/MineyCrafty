cmake_minimum_required(VERSION 3.7)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "")
endif()

project(MineyCrafty)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADERS
    include/FastNoise.h
    include/util/input_manager.hpp
    include/util/mouse.hpp
    include/util/timer.hpp
	include/util/util.hpp
    include/camera.hpp
    include/chunk.hpp
    include/cube.hpp
    include/game.hpp
    include/shader.hpp
    include/texture_atlas.hpp)

set(SOURCES
    src/FastNoise.cpp
    src/util/input_manager.cpp
    src/util/mouse.cpp
    src/util/timer.cpp
    src/camera.cpp
    src/chunk.cpp
    src/game.cpp
    src/shader.cpp
    src/texture_atlas.cpp
    src/main.cpp)

if(WIN32)
    find_package(OpenGL REQUIRED)
    find_package(GLEW REQUIRED)
    find_package(SDL2 CONFIG REQUIRED)
    find_package(sdl2-image CONFIG REQUIRED)
    find_package(sdl2-ttf CONFIG REQUIRED)
    find_package(sdl2-mixer CONFIG REQUIRED)
    find_package(sdl2-net CONFIG REQUIRED)

    set(GLEW_INCLUDE_DIRS ${GLEW_PATH}/include)

    add_executable(mineycrafty WIN32 ${SOURCES})

    target_link_libraries(mineycrafty PRIVATE GLEW::GLEW)
    target_link_libraries(mineycrafty PRIVATE SDL2::SDL2 SDL2::SDL2main)
    target_link_libraries(mineycrafty PRIVATE SDL2::SDL2_image)
    target_link_libraries(mineycrafty PRIVATE SDL2::SDL2_ttf)
    target_link_libraries(mineycrafty PRIVATE SDL2::SDL2_mixer)
    target_link_libraries(mineycrafty PRIVATE SDL2::SDL2_net)
    
    include_directories(${GLEW_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/include)
else()
    find_package(OpenGL REQUIRED)
    find_package(GLEW REQUIRED)
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    find_package(SDL2_mixer REQUIRED)
    find_package(SDL2_net REQUIRED)

    include_directories(${CMAKE_SOURCE_DIR}/include ${GLEW_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} 
        ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS} ${SDL2_GFX_INCLUDE_DIRS}
        ${SDL2_NET_INCLUDE_DIRS})

    add_executable(MineyCrafty ${SOURCES})
    target_link_libraries(MineyCrafty ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} 
        ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY} 
        ${SDL2_MIXER_LIBRARY} ${SDL2_GFX_LIBRARY} ${SDL2_NET_LIBRARY})
endif()

file(COPY "resources" DESTINATION ".")