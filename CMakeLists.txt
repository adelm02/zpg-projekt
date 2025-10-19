cmake_minimum_required(VERSION 3.29)
project(zpgproj)

set(CMAKE_CXX_STANDARD 20)

# Get the Homebrew prefix to locate installed packages
execute_process(
        COMMAND brew --prefix
        OUTPUT_VARIABLE HOMEBREW_PREFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

message(STATUS "Homebrew prefix: ${HOMEBREW_PREFIX}")

# Find the GLFW include directory
find_path(GLFW_INCLUDE_DIR GLFW/glfw3.h HINTS ${HOMEBREW_PREFIX}/include)
if(GLFW_INCLUDE_DIR)
    message(STATUS "Found GLFW include directory: ${GLFW_INCLUDE_DIR}")
else()
    message(FATAL_ERROR "Could not find GLFW include directory")
endif()

# Find the GLFW library
find_library(GLFW_LIB glfw3 HINTS ${HOMEBREW_PREFIX}/lib)
if(GLFW_LIB)
    message(STATUS "Found GLFW library: ${GLFW_LIB}")
else()
    message(FATAL_ERROR "Could not find GLFW library")
endif()

# Find the GLM include directory (GLM is header-only)
find_path(GLM_INCLUDE_DIR glm/glm.hpp HINTS ${HOMEBREW_PREFIX}/include)
if(GLM_INCLUDE_DIR)
    message(STATUS "Found GLM include directory: ${GLM_INCLUDE_DIR}")
else()
    message(FATAL_ERROR "Could not find GLM include directory")
endif()

# Find the GLEW include directory
find_path(GLEW_INCLUDE_DIR GL/glew.h HINTS ${HOMEBREW_PREFIX}/include)
if (GLEW_INCLUDE_DIR)
    message(STATUS "Found GLEW include directory: ${GLEW_INCLUDE_DIR}")
else()
    message(FATAL_ERROR "Could not find GLEW include directory")
endif()

# Find the GLEW library
find_library(GLEW_LIB NAMES glew glew32 HINTS ${HOMEBREW_PREFIX}/lib)
if (GLEW_LIB)
    message(STATUS "Found GLEW library: ${GLEW_LIB}")
else()
    message(FATAL_ERROR "Could not find GLEW library")
endif()

# file(GLOB SOIL_SOURCES
#     ${CMAKE_SOURCE_DIR}/lib/soil/src/image_dxt.c
#     ${CMAKE_SOURCE_DIR}/lib/soil/src/image_helper.c
#     ${CMAKE_SOURCE_DIR}/lib/soil/src/soil.c
#     ${CMAKE_SOURCE_DIR}/lib/soil/src/stb_image_aug.c
# )

# find_path(ASSIMP_INCLUDE_DIR assimp/Importer.hpp HINTS ${HOMEBREW_PREFIX}/include)
# find_library(ASSIMP_LIB NAMES assimp HINTS ${HOMEBREW_PREFIX}/lib)


# include_directories(./lib/soil/include)
# add_library(SOIL STATIC ${SOIL_SOURCES})


# Add the executable
add_executable(zpgproj 
    zkouska.cpp
        Shader.cpp
        Shader.h
        ShaderProgram.cpp
        ShaderProgram.h
        Application.cpp
        Application.h
        Modell.cpp
        Modell.h
        DrawableObject.cpp
        DrawableObject.h
        Scene.cpp
        Scene.h
        Transformation.cpp
        Transformation.h
        AbstractTrans.cpp
        AbstractTrans.h
        Tranform.cpp
        Tranform.h
        Scale.cpp
        Scale.h
        Rotate.cpp
        Rotate.h
        SceneManager.cpp
        SceneManager.h
        Camera.cpp
        Camera.h
        Observer.h
        Controller.cpp
        Controller.h
        Light.cpp
        Light.h
        triangle.h
)

# Include the GLFW, GLM, and GLEW directories
target_include_directories(zpgproj PRIVATE
        ${GLFW_INCLUDE_DIR}
        ${GLM_INCLUDE_DIR}
        ${GLEW_INCLUDE_DIR}
        "src"
)

# If you're linking against the static version of GLEW, define GLEW_STATIC
# Uncomment the following line if using the static library
# target_compile_definitions(zpgproj PRIVATE GLEW_STATIC)

# Link the GLFW, GLEW libraries and required macOS frameworks
target_link_libraries(zpgproj
        ${GLFW_LIB}
        ${GLEW_LIB}
        #SOIL
        #${ASSIMP_LIB}
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
        "-framework OpenGL"
        "-framework CoreFoundation"
        "-lobjc"
)

