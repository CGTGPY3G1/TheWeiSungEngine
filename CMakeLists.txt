cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.22.32.tar.gz"
    SHA1 "37cc115ebce13ee279cc1f086d7203394e2e959a"
)
project(TheWeiSungEngine)
project(TheWeiSungEngine C CXX)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-std=c++14")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
IF(MSVC)
    	SET(CMAKE_CXX_FLAGS "/EHsc")
ENDIF(MSVC)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(include)
set (source "${CMAKE_SOURCE_DIR}/Assets")
set (destination "${CMAKE_CURRENT_BINARY_DIR}")

set(BUILD_SHARED_LIBS FALSE)
set(SFML_BUILD_NETWORK FALSE)
set(SFML_USE_STATIC_STD_LIBS TRUE)
add_subdirectory(ext/SFML)
include_directories(ext/SFML/include ${SFML_INCLUDE_DIR})

set(BOX2D_INSTALL_DOC OFF)
set(BOX2D_BUILD_EXAMPLES OFF)
set(BOX2D_INSTALL OFF CACHE BOOL "Install Box2D libs")
set(BOX2D_INSTALL_DOC OFF CACHE BOOL "Install Box2D documentation")
set(BOX2D_BUILD_SHARED OFF CACHE BOOL "Build Box2D shared libraries")
set(BOX2D_BUILD_EXAMPLES OFF CACHE BOOL "Build Box2D examples")
add_subdirectory(ext/Box2D/Box2D)

hunter_add_package(cereal)
find_package(cereal CONFIG REQUIRED)

hunter_add_package(ZLIB)
find_package(ZLIB CONFIG REQUIRED)

set(WEISUNG_HEADERS
	./include/AABB.h
	./include/AssetManager.h
	./include/AttackerIdentityScript.h
	./include/AttackTypes.h
	./include/AudioManagementSystem.h
	./include/AudioSource.h
	./include/BitmapFont.h
	./include/BitmapString.h
	./include/BloodSplatterScript.h
	./include/BoxCollider.h
	./include/BulletScript.h
	./include/CharacterScript.h
	./include/CircleCollider.h
	./include/CivWaypointScript.h
	./include/Collectable.h
	./include/Collider.h
	./include/CollisionData.h
	./include/CollisionHandler.h
	./include/CollisionMask.h
	./include/Component.h
	./include/ComponentData.h
	./include/ComponentHeaders.h
	./include/ComponentManager.h
	./include/DamageScript.h
	./include/DeathTimer.h
	./include/Debug.h
	./include/DebugDraw.h
	./include/Engine.h
	./include/EngineSettings.h
	./include/FileManager.h
	./include/Game.h
	./include/GameObject.h
	./include/GameObjectFactory.h
	./include/GameObjectManager.h
	./include/Graphics.h
	./include/GrenadeScript.h
	./include/HealthScript.h
	./include/Input.h
	./include/Joint.h
	./include/WSMath.h
	./include/Message.h
	./include/NameGenerator.h
	./include/ParsedTMX.h
	./include/Physics.h
	./include/PhysicsSystem.h
	./include/PivotPoint.h
	./include/PlayerScript.h
	./include/PolygonCollider.h
	./include/PopulationController.h
	./include/Profiler.h
	./include/Random.h
	./include/Rect.h
	./include/RenderLayer.h
	./include/resource.h
	./include/RigidBody2D.h
	./include/RunShootDrive.h
	./include/Scene.h
	./include/ScriptableComponent.h
	./include/ScriptManagementSystem.h
	./include/SelfDestructingAnimScript.h
	./include/SpriteAnimator.h
	./include/SpriteBatch.h
	./include/SpriteRenderer.h
	./include/SpriteRenderingSystem.h
	./include/System.h
	./include/TestScene.h
	./include/TileInfo.h
	./include/TileMapper.h
	./include/Timer.h
	./include/Transform2D.h
	./include/TypeConversion.h
	./include/TypeInfo.h
	./include/Vector2.h
	./include/VehicleController.h
	./include/WeaponCache.h
	./include/Weapons.h
	./include/WSSprite.h
)

set (WEISUNG_SOURCE
	./src/AssetManager.cpp
	./src/AttackerIdentityScript.cpp
	./src/AudioManagementSystem.cpp
	./src/AudioSource.cpp
	./src/BitmapFont.cpp
	./src/BitmapString.cpp
	./src/BloodSplatterScript.cpp
	./src/BoxCollider.cpp
	./src/BulletScript.cpp
    	./src/CharacterScript.cpp
    	./src/CircleCollider.cpp
	./src/CivWaypointScript.cpp
    	./src/Collectable.cpp
	./src/Collider.cpp
	./src/CollisionData.cpp
	./src/Component.cpp
    	./src/ComponentManager.cpp
    	./src/DamageScript.cpp
    	./src/DeathTimer.cpp
	./src/Debug.cpp
	./src/DebugDraw.cpp
	./src/Engine.cpp
	./src/EngineSettings.cpp
    	./src/Game.cpp
	./src/GameObject.cpp
	./src/GameObjectFactory.cpp
	./src/GameObjectManager.cpp
	./src/Graphics.cpp
	./src/GrenadeScript.cpp
	./src/HealthScript.cpp
	./src/Input.cpp
    	./src/Joint.cpp
	./src/WSMath.cpp
	./src/NameGenerator.cpp
	./src/ParsedTMX.cpp
	./src/Physics.cpp
	./src/PhysicsSystem.cpp
	./src/PlayerScript.cpp
	./src/PolygonCollider.cpp
	./src/PopulationController.cpp
	./src/Random.cpp
	./src/RigidBody2D.cpp
	./src/RunShootDrive.cpp
	./src/Scene.cpp
	./src/ScriptableComponent.cpp
	./src/ScriptManagementSystem.cpp
	./src/SelfDestructingAnimScript.cpp
	./src/SpriteAnimator.cpp
	./src/SpriteBatch.cpp
	./src/SpriteRenderer.cpp
    	./src/SpriteRenderingSystem.cpp
    	./src/TestScene.cpp
	./src/TileMapper.cpp
    	./src/Timer.cpp
	./src/Transform2D.cpp
	./src/Vector2.cpp
	./src/VehicleController.cpp
    	./src/WeaponCache.cpp
    	./src/Weapons.cpp
    	./src/WSSprite.cpp
)

source_group("Header Files" FILES ${WEISUNG_HEADERS})
source_group("Source Files" FILES ${WEISUNG_SOURCE})
include_directories(
	${WEISUNG_HEADERS}
	${WEISUNG_SOURCE}
)

add_executable(TheWeiSungEngine
	src/Main.cpp
	${WEISUNG_HEADERS}
	${WEISUNG_SOURCE}
)

add_custom_command(
 	TARGET ${PROJECT_NAME} PRE_LINK
 	COMMAND ${CMAKE_COMMAND} -E copy_directory ${source} ${destination} 
 	DEPENDS ${destination}
 	COMMENT "Copied assets from ${source} to ${destination}"
)
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ext/SFML/extlibs/bin/x64/openal32.dll ${destination}/openal32.dll COPYONLY)
else ()
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ext/SFML/extlibs/bin/x86/openal32.dll ${destination}/openal32.dll COPYONLY)
endif()

target_include_directories(TheWeiSungEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ext/Box2D/Box2D)
target_link_libraries(TheWeiSungEngine PUBLIC Box2D sfml-graphics sfml-window sfml-system sfml-audio sfml-main cereal::cereal)
