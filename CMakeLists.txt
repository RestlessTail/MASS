cmake_minimum_required(VERSION 3.0)

if(EXISTS "glad.cpp")
else()
	file(RENAME "glad.c" "glad.cpp")
endif()

file(MAKE_DIRECTORY "build")
file(MAKE_DIRECTORY "build/Debug")
file(MAKE_DIRECTORY "build/Release")

file(COPY "assets/" DESTINATION "build/Debug/assets/")
file(COPY "font/" DESTINATION "build/Debug/font/")
file(COPY "shader/" DESTINATION "build/Debug/shader/")
file(COPY "assets/" DESTINATION "build/Release/assets/")
file(COPY "font/" DESTINATION "build/Release/font/")
file(COPY "shader/" DESTINATION "build/Release/shader/")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "build/Debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "build/Release")

project(MASS
        VERSION 0.0.1
		DESCRIPTION "My Arknights Script Script"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE "Release")

include_directories("include/")

set(SOURCES
	glad.cpp
	Action.cpp
	Actor.cpp
	Attack.cpp
	Button.cpp
	ButtonBack.cpp
	ButtonHideUI.cpp
	ButtonLogMode.cpp
	CharacterController.cpp
	GLSLLoader.cpp
	Line.cpp
	LogView.cpp
	main.cpp
	Mask.cpp
	MyArknightsScriptScript.cpp
	RectangleObject.cpp
	Retreat.cpp
	Scene.cpp
	Shadow.cpp
	stb_image.cpp
	Symbol.cpp
	TextureLoader.cpp
	interpreter/lexer.cpp
	interpreter/syntax.cpp
)

link_directories(./lib)
add_executable(MASS ${SOURCES})
target_link_libraries(MASS 
	opengl32
	glfw3
	freetype
)

