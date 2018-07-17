cmake_minimum_required(VERSION 3.0)

include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.22.32.tar.gz"
    SHA1 "37cc115ebce13ee279cc1f086d7203394e2e959a"
)


project(TheWeiSungEngine)
include(CMakeSources.cmake)

hunter_add_package(Box2D)
find_package(Box2D CONFIG REQUIRED)

hunter_add_package(cereal)
find_package(cereal CONFIG REQUIRED)

hunter_add_package(ZLIB)
find_package(ZLIB CONFIG REQUIRED)

include_directories(ext/SFML)

add_executable(runshootdrive src/main.cpp)

target_link_libraries(runshootdrive PUBLIC Box2D::Box2D SFML::system SFML::window SFML::graphics SFML::network SFML::audio)