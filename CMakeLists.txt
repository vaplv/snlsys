cmake_minimum_required(VERSION 2.6)
project(sys C)
enable_testing()

if(NOT CMAKE_COMPILER_IS_GNUCC)
  message(FATAL_ERROR "Unsupported compiler")
endif(NOT CMAKE_COMPILER_IS_GNUCC)

set(CMAKE_C_FLAGS "-pedantic -std=c99 -Wall -Wextra -Wcast-align -Wmissing-declarations -Wmissing-prototypes -fvisibility=hidden -fstrict-aliasing -Wl,-z,defs")
set(CMAKE_C_FLAGS_DEBUG "-g -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")

add_definitions(-D_POSIX_C_SOURCE=200809L)

################################################################################
# Define targets
################################################################################
file(GLOB SYS_FILES_SRC src/*.c)
file(GLOB SYS_FILES_INC src/*.h)

add_library(sys SHARED ${SYS_FILES_SRC} ${SYS_FILES_INC})
set_target_properties(sys PROPERTIES DEFINE_SYMBOL SYS_SHARED_BUILD)

################################################################################
# Define output & install directories
################################################################################
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${sys_BINARY_DIR}/lib)

install(TARGETS sys LIBRARY DESTINATION lib)
install(FILES ${SYS_FILES_INC} DESTINATION include/sys)
