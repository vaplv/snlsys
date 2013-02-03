# Copyright (c) 2013 Vincent Forest
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
# 3. The name of the author may not be used to endorse or promote products
#    derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO
# EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
