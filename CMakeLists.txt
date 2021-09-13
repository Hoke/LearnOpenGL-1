cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 3.0)

project(LearnOpenGL)

# 设置指定的C++编译器版本是必须的，如果不设置，或者为OFF，则指定版本不可用时，会使用上一版本
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 指定为C++11 版本
set(CMAKE_CXX_STANDARD 11)


message("${CMAKE_BUILD_TYPE}")

# release mode or using  cmake -DCMAKE_BUILD_TYPE=Debug/Release
# SET(CMAKE_BUILD_TYPE "Release")
IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

# cmake 模块路径
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

if(WIN32)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
ENDIF(WIN32)

macro(makeLink src dest target)
    add_custom_command(TARGET ${target} POST_BUILD COMMAND ${CMAKE_COMMAND} -E create_symlink ${src} ${dest}  DEPENDS  ${dest} COMMENT "mklink ${src} -> ${dest}")
endmacro()

if(WIN32)
    set(LIBS opengl32)
elseif(UNIX AND NOT APPLE)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
  find_package(OpenGL REQUIRED)
  add_definitions(${OPENGL_DEFINITIONS})
  find_package(X11 REQUIRED)
  # note that the order is important for setting the libs
  # use pkg-config --libs $(pkg-config --print-requires --print-requires-private glfw3) in a terminal to confirm
  set(LIBS ${GLFW3_LIBRARY} X11 Xrandr Xinerama Xi Xxf86vm Xcursor GL dl pthread freetype ${ASSIMP_LIBRARY})
  set (CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")
elseif(APPLE)
  INCLUDE_DIRECTORIES(/System/Library/Frameworks)
  FIND_LIBRARY(COCOA_LIBRARY Cocoa)
  FIND_LIBRARY(OpenGL_LIBRARY OpenGL)
  FIND_LIBRARY(IOKit_LIBRARY IOKit)
  FIND_LIBRARY(CoreVideo_LIBRARY CoreVideo)
  MARK_AS_ADVANCED(COCOA_LIBRARY OpenGL_LIBRARY)
  SET(APPLE_LIBS ${COCOA_LIBRARY} ${IOKit_LIBRARY} ${OpenGL_LIBRARY} ${CoreVideo_LIBRARY})
  SET(APPLE_LIBS ${APPLE_LIBS} ${GLFW3_LIBRARY} ${ASSIMP_LIBRARY})
  set(LIBS ${LIBS} ${APPLE_LIBS})
else()
  set(LIBS )
endif(WIN32)

# glfw
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(vendor/glfw)
set(LIBS ${LIBS} glfw)


# glad
set(GLAD_DOT_C vendor/glad/src/glad.c)
# add_library(GLAD ${GLAD_DOT_C})

# assimp
option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(vendor/assimp)
set(LIBS ${LIBS} assimp)




# GLM GLFW3 ASSIMP GLAD
set(CHAPTERS
    1.getting_started
    2.lighting
    3.model_loading
    4.advanced_opengl
    5.advanced_lighting
    6.pbr
    7.in_practice
)

set(1.getting_started
    1.0.test_compile
    1.1.hello_window
    2.1.hello_triangle
    2.2.hello_triangle_indexed
    2.3.hello_triangle_2
    2.4.get_opengl_info
    2.5.hello_triangle_exercise
    3.1.shader_uniform
    3.2.shaders_interpolation
    3.3.shader_class
    3.4_shaders_exercise1
    3.5_shaders_exercise2
    4.1.textures
)


configure_file(configuration/root_directory.h.in configuration/root_directory.h)

include_directories(
                    ${CMAKE_SOURCE_DIR}/includes
                    ${CMAKE_BINARY_DIR}/configuration
                    ${CMAKE_SOURCE_DIR}/vendor/assimp/include/
                    # ${CMAKE_SOURCE_DIR}/vendor/bullet/src/
                    # ${CMAKE_SOURCE_DIR}/vendor/glad/include/glad/glad.h
                    ${CMAKE_SOURCE_DIR}/vendor/glad/include/
                    ${CMAKE_SOURCE_DIR}/vendor/glfw/include/
                    ${CMAKE_SOURCE_DIR}/vendor/glm/
                    ${CMAKE_SOURCE_DIR}/vendor/stb/)

#  create a project for per tutorial
foreach(CHAPTER ${CHAPTERS})
    message("======start process chapter ${CHAPTER}======")
    foreach(DEMO ${${CHAPTER}})
        message("start generate project ${CHAPTER}/${DEMO}")
        file(GLOB SOURCE
            "src/${CHAPTER}/${DEMO}/*.h"
            "src/${CHAPTER}/${DEMO}/*.cpp"
        )

        set(NAME "${CHAPTER}_${DEMO}")
        # 可执行程序
        message("${NAME} source: ${SOURCE} ${GLAD_DOT_C}")
        add_executable(${NAME} ${GLAD_DOT_C} ${SOURCE} )
        # 链接
        message("${NAME} lib: ${LIBS}")
        target_link_libraries(${NAME} ${LIBS})

        if(WIN32)
            set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CHAPTER}/${DEMO}")
            set_target_properties(${NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CHAPTER}/${DEMO}/Debug")
        elseif(UNIX AND NOT APPLE)
            set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CHAPTER}")
        elseif(APPLE)
            set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CHAPTER}/${DEMO}")
            set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/${CHAPTER}/${DEMO}")
            set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/${CHAPTER}/${DEMO}")
        endif(WIN32)

        # copy shader files to build directory
        file(GLOB SHADERS
                 "src/${CHAPTER}/${DEMO}/*.vs"
                 # "src/${CHAPTER}/${DEMO}/*.frag"
                 "src/${CHAPTER}/${DEMO}/*.fs"
                 "src/${CHAPTER}/${DEMO}/*.gs"
        )
        foreach(SHADER ${SHADERS})
            if(WIN32)
                # configure_file(${SHADER} "test")
                add_custom_command(TARGET ${NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SHADER} $<TARGET_FILE_DIR:${NAME}>)
            elseif(UNIX AND NOT APPLE)
                file(COPY ${SHADER} DESTINATION ${CMAKE_SOURCE_DIR}/bin/${CHAPTER})
            elseif(APPLE)
                # create symbolic link for *.vs *.fs *.gs
                get_filename_component(SHADERNAME ${SHADER} NAME)
                makeLink(${SHADER} ${CMAKE_SOURCE_DIR}/bin/${CHAPTER}/${DEMO}/${SHADERNAME} ${NAME})
            endif(WIN32)
        endforeach(SHADER)
        # if compiling for visual studio, also use configure file for each project (specifically to set up working directory)
        if(MSVC)
            configure_file(${CMAKE_SOURCE_DIR}/configuration/visualstudio.vcxproj.user.in ${CMAKE_CURRENT_BINARY_DIR}/${NAME}.vcxproj.user @ONLY)
        endif(MSVC)
        message("end generate project ${CHAPTER}/${DEMO}\n")
    endforeach(DEMO)
    message("======end process chapter ${CHAPTER}======")
endforeach(CHAPTER)





