cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
project(minesweeper)

set(CWD ${CMAKE_CURRENT_LIST_DIR})

set(CXX_STANDARD 17)

# APPLICATION
file(GLOB_RECURSE GAME_SOURCES 
	"game/*.cpp"
	"game/*.h"
)

set(EXEC_VARIABLE)
if (WIN32)
	set(EXEC_VARIABLE WIN32)
endif(WIN32)
add_executable(${PROJECT_NAME} ${EXEC_VARIABLE} ${GAME_SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
)
target_include_directories(${PROJECT_NAME} PUBLIC ${CWD}/game)

if (MACOS)
	target_link_libraries(${PROJECT_NAME} PRIVATE
		"${CMAKE_SOURCE_DIR}/third_party/raylib-5.5_macos/lib/libraylib.a"
	)
	target_link_libraries(${PROJECT_NAME} PRIVATE 
		"-framework OpenGL"
		"-framework Cocoa"
		"-framework IOKit"
	)
	target_include_directories(${PROJECT_NAME} PRIVATE
		"third_party/raylib-5.5_macos/include"
	)
endif()

if (WIN32)
	target_link_libraries(${PROJECT_NAME} PRIVATE 
		"${CMAKE_SOURCE_DIR}/third_party/raylib-5.5_win64_msvc16/lib/raylib.lib"
	)
	target_include_directories(${PROJECT_NAME} PRIVATE 
		"third_party/raylib-5.5_win64_msvc16/include"
	)
endif()


set_target_properties(${PROJECT_NAME} PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}
	RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}
)

if (WIN32)
	target_compile_definitions(${PROJECT_NAME} PRIVATE WIN32)
endif(WIN32)
