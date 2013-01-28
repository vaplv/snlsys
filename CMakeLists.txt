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
set(SYS_FILES_SRC
  src/clock_time.c
  src/mem_allocator.c)

set(SYS_FILES_INC
  src/clock_time.h
  src/list.h
  src/math.h
  src/mem_allocator.h
  src/ref_count.h
  src/sys.h)

add_library(sys SHARED ${SYS_FILES_SRC} ${SYS_FILES_INC})
set_target_properties(sys PROPERTIES DEFINE_SYMBOL SYS_SHARED_BUILD)

################################################################################
# Add tests
################################################################################
add_executable(test_list src/test_list.c)
target_link_libraries(test_list sys)
add_test(test_list test_list)

add_executable(test_mem_allocator src/test_mem_allocator.c)
target_link_libraries(test_mem_allocator sys)
add_test(test_mem_allocator test_mem_allocator)

################################################################################
# Define output & install directories
################################################################################
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${sys_BINARY_DIR}/lib)

install(TARGETS sys LIBRARY DESTINATION lib)
install(FILES ${SYS_FILES_INC} DESTINATION include/sys)
