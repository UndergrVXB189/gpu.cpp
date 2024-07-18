get_filename_component(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
get_filename_component(PROJECT_ROOT ${PROJECT_ROOT} DIRECTORY)

# Construct potential paths
set(FILEPATH_CURRENT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}")
set(FILEPATH_PROJECT_ROOT "${PROJECT_ROOT}/${FILENAME}")

# Check if the file exists in the current directory
if(EXISTS ${FILEPATH_CURRENT_DIR})
    set(TARGET_FILE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
elseif(EXISTS ${FILEPATH_PROJECT_ROOT})
    set(TARGET_FILE_PATH ${PROJECT_ROOT})
else()
    message(FATAL_ERROR "File ${FILENAME} not found in either ${CMAKE_CURRENT_SOURCE_DIR} or ${CMAKE_CURRENT_SOURCE_DIR}/../../")
endif()

# Ensure the build type is set
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build: Debug or Release" FORCE)
endif()

# Define architecture and build type directories or file names
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(ARCH "x64")
else()
  set(ARCH "x86")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(BUILD_TYPE "Debug")
else()
  set(BUILD_TYPE "Release")
endif()

if(NOT TARGET gpu)
  message(STATUS "GPU_LIB not found")
  include("${TARGET_FILE_PATH}/cmake/webgpu.cmake")
  include("${TARGET_FILE_PATH}/cmake/gpu.cmake")
endif()

add_executable(${PROJECT_NAME} run.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE gpu)
target_link_libraries(${PROJECT_NAME} PRIVATE wgpu)
target_link_libraries(${PROJECT_NAME} PRIVATE webgpu)

if(WIN32)
# Ensure DLL is copied if on Windows
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                   ${DLL_PATH}
                   $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif()