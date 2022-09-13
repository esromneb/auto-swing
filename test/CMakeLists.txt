cmake_minimum_required(VERSION 3.0)

project(auto_swing_test CXX)

cmake_policy(GET CMP0011 cmp)


add_subdirectory(../3rd/rapidcheck all)



# -O3 
set(CMAKE_CXX_FLAGS
"${CMAKE_CXX_FLAGS} \
-Werror=return-type -Wall -Wextra -fPIC -Wno-unused-function -Wno-error=attributes \
-std=gnu++11 -Wvla -Wcast-qual -Wdangling-else -Winit-self -Werror=uninitialized \
-Werror=array-bounds -Wshadow \
-Wduplicated-cond -Wnull-dereference -Wdangling-else -Waddress \
-Wint-in-bool-context -Winit-self \
-Wpointer-arith \
-Werror=uninitialized \
-Wlogical-op -Werror=logical-op -Werror=null-dereference \
-Werror=sequence-point \
-Werror=missing-braces -Werror=write-strings -Werror=address -Werror=array-bounds \
-Werror=char-subscripts -Werror=enum-compare \
-Werror=empty-body -Werror=main -Werror=nonnull -Werror=parentheses \
-Werror=ignored-qualifiers \
-Werror=unused-value \
-Wmissing-declarations \
-Wmissing-field-initializers \
-Wcast-align \
-Wduplicated-branches \
-Wformat \
-Wformat-security \
-Wformat-signedness \
-Woverlength-strings \
-Wredundant-decls \
-Wsynth \
-Wtrampolines \
-Wvariadic-macros \
-Wvirtual-inheritance \
-Wno-unused-parameter \
"
)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")


add_definitions(-DTEST_ENVIRONMENT)


#select the release build type by default to get optimization flags
if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE "Debug")
   message(STATUS "Build type not specified: defaulting to release.")
endif(NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "")


set(EMBEDDED_DIR "../src")

set(EMBEDDED_SOURCE_FILES
    ${EMBEDDED_DIR}/myfsm.hpp
    ${EMBEDDED_DIR}/myfsm.cpp
)

set(TEST_SOURCE_FILES
    test_fsm.cpp
    test_control_fsm.cpp
)


include_directories(${EMBEDDED_DIR})
include_directories(../3rd/rapidcheck/include)

add_executable(test ${EMBEDDED_SOURCE_FILES} ${TEST_SOURCE_FILES})

target_link_libraries(test rapidcheck)

add_custom_target(run COMMAND ./test)
