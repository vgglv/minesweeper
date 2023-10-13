cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
project(minesweeper)

set(CWD ${CMAKE_CURRENT_LIST_DIR})

macro(Log arg)
	message(STATUS "[Main] ${arg}")
endmacro()

# APPLICATION
file(GLOB_RECURSE application_sources
	"game/application/*.cpp"
	"game/application/*.h"
)
LOG("Added application sources: ${application_sources}")

# RANDOM
file(GLOB_RECURSE random_sources
	"game/random/*.cpp"
	"game/random/*.h"
)
LOG("Added random sources: ${random_sources}")

# BOARD
file(GLOB_RECURSE board_sources
	"game/board/*.cpp"
	"game/board/*.h"
)
LOG("Added board sources: ${board_sources}")

# PLATFORM

if (WIN32)
	file(
		GLOB_RECURSE windows_sources
		"game/platforms/win32/*.cpp"
	)
endif(WIN32)

if (MACOS)
	file(
		GLOB_RECURSE macos_sources
		"game/platforms/macos/*.cpp"
	)
endif(MACOS)

set(platform_sources
	${windows_sources}
	${macos_sources}
)
LOG("Added platform sources: ${platform_sources}")

file(
	GLOB_RECURSE loader_sources
	"game/loader/*.cpp"
	"game/loader/*.h"
)

# ALL_SOURCES
set(ALL_SOURCES
	${application_sources}
	${random_sources}
	${board_sources}
	${platform_sources}
	${loader_sources}
)

set(EXEC_VARIABLE)
if (WIN32)
	set(EXEC_VARIABLE WIN32)
endif(WIN32)
add_executable(${PROJECT_NAME} ${EXEC_VARIABLE} ${ALL_SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
)
target_include_directories(${PROJECT_NAME} PUBLIC ${CWD}/game)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/game/" FILES ${ALL_SOURCES})

add_subdirectory(third_party/SDL)
add_subdirectory(third_party/SDL_image)

target_link_libraries(${PROJECT_NAME}
	PRIVATE
	SDL2
	SDL2_image
)
target_include_directories(${PROJECT_NAME}
	PRIVATE
	${CMAKE_SOURCE_DIR}/game/include
)
if (WIN32 OR MACOS)
	set_target_properties(${PROJECT_NAME} PROPERTIES
	   RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
	   RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}
	   RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}
	)
endif(WIN32 OR MACOS)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")