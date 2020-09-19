cmake_minimum_required(VERSION 3.7)

project(MineyCrafty)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
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
    set(SDL_PATH "C:/gamedev/sdl")
    set(GLEW_PATH "C:/gamedev/glew")
    list(APPEND CMAKE_PREFIX_PATH ${SDL_PATH})
    list(APPEND CMAKE_PREFIX_PATH ${GLEW_PATH})
    
    find_package(OpenGL REQUIRED)
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    find_package(SDL2_mixer REQUIRED)
    #find_package(SDL2_gfx REQUIRED)
    find_package(SDL2_net REQUIRED)

    set(GLEW_INCLUDE_DIRS ${GLEW_PATH}/include)


    include_directories(${CMAKE_SOURCE_DIR}/include ${GLEW_INCLUDE_DIRS}
        ${SDL2_INCLUDE_DIRS})
    
    if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
        link_directories("${SDL2_PATH}/lib/x64;${GLEW_PATH}/lib/x64")
        file(COPY "${SDL_PATH}/lib/x64/*.dll" 
            DESTINATION "./Debug"
            FILES_MATCHING
                PATTERN "*.dll")
        file(COPY "${GLEW_PATH}/bin/x64/glew32.dll" DESTINATION "./Debug")
        set(GLEW_LIBRARIES ${GLEW_PATH}/lib/x64/glew32.lib)
    else()
        link_directories("${SDL2_PATH}/lib/x86;${GLEW_PATH}/lib/x86")
        file(COPY "${SDL_PATH}/lib/x86/" 
        DESTINATION "./Debug"
        FILES_MATCHING
            PATTERN "*.dll")
        file(COPY "${GLEW_PATH}/bin/x86/glew32.dll" DESTINATION "./Debug")
        set(GLEW_LIBRARIES ${GLEW_PATH}/lib/x86/glew32.lib)
    endif()

	include_directories(${CMAKE_SOURCE_DIR}/include ${GLEW_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} 
        ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS} ${SDL2_GFX_INCLUDE_DIRS}
        ${SDL2_NET_INCLUDE_DIRS})# ${BULLET_INCLUDE_DIRS})
    
    add_executable(MineyCrafty WIN32 ${SOURCES} ${HEADERS})
    target_link_libraries(MineyCrafty ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES}
        ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY} 
        ${SDL2_MIXER_LIBRARY})
        #${BULLET_LBRARIES}) 
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
        ${SDL2_NET_INCLUDE_DIRS})# ${BULLET_INCLUDE_DIRS})

    add_executable(MineyCrafty ${SOURCES})
    target_link_libraries(MineyCrafty ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} 
        ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY} 
        ${SDL2_MIXER_LIBRARY} ${SDL2_GFX_LIBRARY} ${SDL2_NET_LIBRARY})
        #${BULLET_LBRARIES}) 
endif()

#find_package(Bullet REQUIRED)
file(COPY "resources" DESTINATION ".")