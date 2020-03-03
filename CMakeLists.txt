cmake_minimum_required(VERSION 3.7)

project(MineyCrafty)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(!WIN32)
    find_package(OpenGL REQUIRED)
    find_package(GLEW REQUIRED)
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    find_package(SDL2_mixer REQUIRED)
    find_package(SDL2_gfx REQUIRED)
    find_package(SDL2_net REQUIRED)
    include_directories(${CMAKE_SOURCE_DIR}/include ${GLEW_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} 
        ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS} ${SDL2_GFX_INCLUDE_DIRS}
        ${SDL2_NET_INCLUDE_DIRS})# ${BULLET_INCLUDE_DIRS})
else()
    list(APPEND CMAKE_PREFIX_PATH "C:\\gamedev\\sdl")
    list(APPEND CMAKE_PREFIX_PATH "C:\\gamedev\\glew")
    find_package(OpenGL REQUIRED)
    find_package(GLEW REQUIRED)
    find_package(SDL2 REQUIRED)
    include_directories(${CMAKE_SOURCE_DIR}/include ${GLEW_INCLUDE_DIRS}
        ${SDL2_INCLUDE_DIRS})
    if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
        link_directories("${SDL2_PATH}\\lib\\x64;${GLEW_PATH}\\lib\\x64")
    else()
        link_directories("${SDL2_PATH}\\lib\\x86;${GLEW_PATH}\\lib\\x86")
    endif()
endif()
#find_package(Bullet REQUIRED)

set(SOURCES
    src/game.cpp
    src/rectprim.cpp
    src/cube.cpp
    src/shader.cpp
    src/main.cpp)

add_executable(MineyCrafty ${SOURCES})
if (!WIN32)
    target_link_libraries(MineyCrafty ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} 
        ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY} 
        ${SDL2_MIXER_LIBRARY} ${SDL2_GFX_LIBRARY} ${SDL2_NET_LIBRARY})
        #${BULLET_LBRARIES})
else()
    target_link_libraries(MineyCrafty ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} 
        ${SDL2_LIBRARY} mingw32)
        #${BULLET_LBRARIES})
endif()