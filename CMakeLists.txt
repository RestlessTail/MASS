cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)
project(MASS)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)

add_library(MASS SHARED Action.cpp RectangleObject.cpp Actor.cpp CPPInterface.cpp Retreat.cpp Attack.cpp glad.c Scene.cpp GLSLLoader.cpp Button.cpp Shadow.cpp ButtonBack.cpp Sound.cpp Line.cpp ButtonHideUI.cpp stb_image.cpp LogView.cpp TextureLoader.cpp ButtonLogMode.cpp Mask.cpp utils.cpp CharacterController.cpp MyArknightsScriptScript.cpp CInterface.cpp)
target_include_directories(MASS PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(MASS opengl32 ${PROJECT_SOURCE_DIR}/lib/irrKlang.lib ${PROJECT_SOURCE_DIR}/lib/glfw3.lib ${PROJECT_SOURCE_DIR}/lib/freetype.lib)
