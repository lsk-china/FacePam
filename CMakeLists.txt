cmake_minimum_required(VERSION 3.22)
project(FacePam)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_COMPILER /usr/bin/gcc)
set(CMAKE_CXX_COMPILER /usr/bin/g++)
include_directories(.)
include(FindPkgConfig)
pkg_check_modules(PKGS_DEP REQUIRED
        opencv4
        seeta
        )
FOREACH(flag ${PKGS_DEP_CFLAGS})
    SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} ${flag}")
ENDFOREACH(flag)
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_CFLAGS}")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_CFLAGS}")

add_library(FacePam SHARED
        pam_facerec.cpp
        seeta_recognizer.cpp
        seeta_recognizer.hpp)
add_executable(FacePamTest test.cpp)
TARGET_LINK_LIBRARIES(FacePam ${PKGS_DEP_LDFLAGS})
target_link_libraries(FacePamTest ${PKGS_DEP_LDFLAGS})
