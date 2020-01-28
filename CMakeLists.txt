#  Copyright 2019 U.C. Berkeley RISE Lab
# 
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
# 
#      http://www.apache.org/licenses/LICENSE-2.0
# 
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.

CMAKE_MINIMUM_REQUIRED(VERSION 3.6 FATAL_ERROR)
PROJECT(Anna-Cache)

SET(ANNA_CACHE_MAJOR_VERSION 0)
SET(ANNA_CACHE_MINOR_VERSION 1)
SET(ANNA_CACHE_PATCH_VERSION 0)

IF(NOT DEFINED BUILD_TEST)
  SET(BUILD_TEST OFF)
ENDIF()

IF(${BUILD_TEST})
  ENABLE_TESTING()
ENDIF()

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED on)

SET(VENDOR_DIR common/vendor)

IF(${CMAKE_CXX_COMPILER} STREQUAL "/usr/bin/clang++") 
  SET(CMAKE_CXX_FLAGS_COMMON
      "-std=c++11 \
      -stdlib=libc++ -pthread")
ENDIF()

IF(${CMAKE_CXX_COMPILER} STREQUAL "/usr/bin/g++") 
  SET(CMAKE_CXX_FLAGS_COMMON
      "-std=c++11 -pthread")
ENDIF()

SET(CMAKE_CXX_FLAGS_DEBUG
    "${CMAKE_CXX_FLAGS_DEBUG} \
    ${CMAKE_CXX_FLAGS_COMMON} \
    -g -O0 -fprofile-arcs -ftest-coverage")

SET(CMAKE_CXX_FLAGS_RELEASE
    "${CMAKE_CXX_FLAGS_RELEASE} \
    ${CMAKE_CXX_FLAGS_COMMON} \
    -O3")

ADD_SUBDIRECTORY(${VENDOR_DIR}/spdlog)
ADD_SUBDIRECTORY(${VENDOR_DIR}/yamlcpp)
ADD_SUBDIRECTORY(${VENDOR_DIR}/zeromq)
ADD_SUBDIRECTORY(${VENDOR_DIR}/zeromqcpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${SPDLOG_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${ZEROMQCPP_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${ZEROMQ_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${YAMLCPP_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(common/include)
INCLUDE_DIRECTORIES(include)

INCLUDE(FindProtobuf)
FIND_PACKAGE(Protobuf REQUIRED)
INCLUDE_DIRECTORIES(${PROTOBUF_INCLUDE_DIR})
PROTOBUF_GENERATE_CPP(PROTO_SRC PROTO_HEADER 
  ./common/proto/cloudburst.proto
  ./common/proto/anna.proto
  ./common/proto/causal.proto
  ./common/proto/shared.proto
)

# Need to build a target at this level or subdirs won't have the 
# protobuf files generated.
ADD_LIBRARY(cache-proto ${PROTO_HEADER} ${PROTO_SRC})

FILE(GLOB_RECURSE ZMQ_UTIL_SRC common/include/zmq/*.cpp)
FILE(GLOB_RECURSE ZMQ_UTIL_HEADER common/include/zmq/*.hpp)
ADD_LIBRARY(hydro-zmq STATIC ${ZMQ_UTIL_HEADER} ${ZMQ_UTIL_SRC})
ADD_DEPENDENCIES(hydro-zmq zeromq zeromqcpp spdlog) 

IF(${CMAKE_BUILD_TYPE} STREQUAL "Debug") 
  INCLUDE(common/cmake/clang-format.cmake)
  INCLUDE(common/cmake/CodeCoverage.cmake) 
ENDIF()


LINK_DIRECTORIES(${ZEROMQ_LINK_DIRS} ${YAMLCPP_LINK_DIRS})

ADD_SUBDIRECTORY(src)

# We only build the mock library and the tests if we are running in test mode.
IF(${BUILD_TEST})
  INCLUDE(common/cmake/DownloadProject.cmake)
  DOWNLOAD_PROJECT(PROJ                googletest
                   GIT_REPOSITORY      https://github.com/google/googletest.git
                   GIT_TAG             release-1.8.0
                   UPDATE_DISCONNECTED 1
  )

  ADD_SUBDIRECTORY(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})

  INCLUDE_DIRECTORIES(common/mock)
  INCLUDE_DIRECTORIES(tests)
  ADD_SUBDIRECTORY(common/mock)
  ADD_SUBDIRECTORY(tests)
ENDIF()
