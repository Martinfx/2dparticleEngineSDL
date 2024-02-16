cmake_minimum_required(VERSION 3.5)

project(ParticleSystem2SDL LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#add_library(pugixml INTERFACE IMPORTED)
include_directories(${PUGI_XML_INCLUDE_DIRS})
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
find_package(SDL2_image)
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})

#target_link_libraries(pugixml INTERFACE pugixml::pugixml)

add_executable(ParticleSystem2SDL main.cpp
    engine.hpp engine.cpp
    module.hpp
    window.hpp
    window.cpp
    input.hpp input.cpp
    render.hpp render.cpp
    texture.hpp texture.cpp
    particlesystem.hpp particlesystem.cpp
    point.hpp
    emitter.hpp emitter.cpp
    perftimer.hpp perftimer.cpp
    particlepool.hpp particlepool.cpp
    particle.hpp particle.cpp
    scene.hpp scene.cpp
    timer.hpp timer.cpp )
target_link_libraries(ParticleSystem2SDL ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} -lpugixml )

include(GNUInstallDirs)
install(TARGETS ParticleSystem2SDL
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
