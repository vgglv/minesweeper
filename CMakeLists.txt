cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
project(minesweeper)


set(CWD ${CMAKE_CURRENT_LIST_DIR})

macro(Log arg)
	message(STATUS "[Main] ${arg}")
endmacro()

# APPLICATION
file(GLOB_RECURSE application_sources
	"${CWD}/src/application/*.h"
	"${CWD}/src/application/*.cpp"
)
LOG("Added application sources: ${application_sources}")

# RANDOM
file(GLOB_RECURSE random_sources
	"${CWD}/src/random/*.h"
	"${CWD}/src/random/*.cpp"
)
LOG("Added random sources: ${random_sources}")

# PLATFORM

if (WIN32)
	file(
		GLOB_RECURSE windows_sources
		"src/platforms/win32/*.h"
		"src/platforms/win32/*.cpp"
	)
endif(WIN32)

set(platform_sources 
	${windows_sources}
)
LOG("Added platform sources: ${platform_sources}")

# ALL_SOURCES
set(ALL_SOURCES
	${application_sources}
	${random_sources}
	${platform_sources}
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
target_include_directories(${PROJECT_NAME} PUBLIC ${CWD}/src)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src/" FILES ${ALL_SOURCES})

message(STATUS ${CMAKE_TOOLCHAIN_FILE})

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} 
	PRIVATE
	$<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
	$<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)