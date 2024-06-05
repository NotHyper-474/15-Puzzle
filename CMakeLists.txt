###################################################

cmake_minimum_required (VERSION 3.5)


project (15-Puzzle VERSION 0.1.0 LANGUAGES C CXX)

# Definitions
add_definitions(-c)

# Support for Hot Reload in MSVC
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	add_definitions(-Wextra -fdeclspec)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
else()
	add_definitions(-W4)
endif()
###########

file(GLOB_RECURSE HDRS ${CMAKE_SOURCE_DIR}/src/*.h  ${CMAKE_SOURCE_DIR}/src/*.hpp)
file(GLOB_RECURSE SRCS ${CMAKE_SOURCE_DIR}/src/*.c ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable (15-Puzzle ${HDRS} ${SRCS})

IF(CMAKE_SIZEOF_VOID_P STREQUAL "4")
	SET(ARCH_NUMBER "86")
ELSEIF(CMAKE_SIZEOF_VOID_P STREQUAL "8")
	SET(ARCH_NUMBER "64")
ENDIF()

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS serialization)

include(cmake/IncludeAll.cmake) # doesn't actually include, just provides paths in a variable

set(include_dirs "")
ADD_SUBDIRECTORIES(${CMAKE_SOURCE_DIR}/src include_dirs)

target_include_directories(15-Puzzle PRIVATE
	${CMAKE_SOURCE_DIR}/src
	${include_dirs}
)

target_link_libraries(15-Puzzle PRIVATE
	${SDL2_LIBRARIES}
	SDL2_image::SDL2_image
	SDL2_ttf::SDL2_ttf
	Boost::boost
	Boost::serialization
	fmt::fmt-header-only
)
set_property(TARGET 15-Puzzle PROPERTY CXX_STANDARD 20)


# Create link to/copy resources folder
IF(WIN32)
	string(REPLACE "/" "\\" source_path_windows "${CMAKE_SOURCE_DIR}/res")
	string(REPLACE "/" "\\" build_path_windows "${CMAKE_BINARY_DIR}/Resources")
	execute_process(COMMAND cmd.exe /c mklink /J "${build_path_windows}" "${source_path_windows}" RESULT_VARIABLE exitcode)
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
	execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}/Resources RESULT_VARIABLE exitcode)
else()
	set (exitcode 1) # Trigger copying
ENDIF()

if(NOT ${exitcode} EQUAL 0)
	MESSAGE("SYMLINKING FAILED: ${exitcode}")
	MESSAGE("FALLING BACK TO COPYING")

	file(GLOB_RECURSE RES RELATIVE ${CMAKE_SOURCE_DIR}/res/ ${CMAKE_SOURCE_DIR}/res/*.*)
	foreach(file IN LISTS RES)
		configure_file(${CMAKE_SOURCE_DIR}/res/${file} ${CMAKE_BINARY_DIR}/Resources/${file} COPYONLY)
	endforeach()

endif()
