#
# CMake script for DOOOOM
#
cmake_minimum_required(VERSION 3.10)

project(doom C)

include(doom/DoomFiles.cmake)

add_executable(doom ${DOOM_FILES})

if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "ESP32")
	target_compile_options(doom PUBLIC ESP32 )
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	target_compile_definitions(doom PUBLIC NORMALUNIX LINUX)
	target_link_libraries(doom PUBLIC X11 Xext m c nsl)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32 -g")
endif()

install(TARGETS doom DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/game)