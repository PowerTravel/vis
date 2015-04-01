cmake_minimum_required (VERSION 2.8)
project (VIS)

add_executable(vis 
	src/main.cpp
	src/MainWindow.cpp
	src/Timer.cpp

	src/Shader.cpp

	src/Node.cpp
	src/Geometry.cpp
	src/Group.cpp
	src/Transform.cpp
	src/Camera.cpp
	src/ParticleSystem.cpp
	src/State.cpp
	src/NodeVisitor.cpp
	src/RenderVisitor.cpp
	src/UpdateVisitor.cpp
	
	src/UpdateCallback.cpp
	src/CameraMovementCallback.cpp

	src/Light.cpp
	src/Material.cpp
)

find_package(OpenGL REQUIRED)

if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory !" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
	message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
	message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()



## Compile dependancies ## 
add_subdirectory(external)

## Include Directories @@
include_directories(
	external/glfw-3.1.1/include
	${GLEW_INCLUDE_LIBRARIES}
	external/assimp-3.1.1/include
	external/glm-0.9.6.3
	external/eigen-3.2.4
	include
)

## LINK TO LIBRARIES ##
set(GLFW_LIBS
	glfw
	${GLFW_LIBRARIES}
)
set(GLEW_LIBS
	GLEW
	${GLEW_LIBRARIES}
)

target_link_libraries(vis
	${OPENGL_LIBRARY}
	${GLFW_LIBS}
	${GLEW_LIBS}
	assimp
)



 #set compiler flags
list( APPEND CMAKE_CXX_FLAGS 
	"-std=c++0x ${CMAKE_CXX_FLAGS} -g")

install(TARGETS vis DESTINATION bin)

