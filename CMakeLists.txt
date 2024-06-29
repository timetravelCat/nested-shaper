cmake_minimum_required(VERSION 3.16.3)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(parse_git_tag)
include(add_compile_warnings)
include(static_analyzers)

project(
  nested-shaper
  VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
  DESCRIPTION
    "A flexible input shaping library using nested moving average filter."
  LANGUAGES CXX)
string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)
string(REPLACE "-" "_" PROJECT_NAME_UPPER ${PROJECT_NAME_UPPER})
configure_file(cmake/version.hpp.in
               ${CMAKE_SOURCE_DIR}/include/nested-shaper/version.hpp @ONLY)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Options
option(BUILD_EXAMPLE "Build examples, using Matplot++" OFF)

# Enforce C++11
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED On)

# Library setup
add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
add_compile_warnings(${PROJECT_NAME})
target_include_directories(
  ${PROJECT_NAME} INTERFACE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
                            $<INSTALL_INTERFACE:include>)

# Testing setup
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  # check c++ standard libary is included. if included,
  file(WRITE ${CMAKE_SOURCE_DIR}/dummy.cpp "")
  file(GLOB_RECURSE HEADERS include/nested-shaper/*.hpp
       include/nested-shaper/**/*.hpp)
  foreach(HEADER ${HEADERS})
    file(RELATIVE_PATH HEADER_RELATIVE ${CMAKE_SOURCE_DIR}/include ${HEADER})
    file(APPEND ${CMAKE_SOURCE_DIR}/dummy.cpp "#include <${HEADER_RELATIVE}>\n")
  endforeach()

  add_library(dummy ${CMAKE_SOURCE_DIR}/dummy.cpp)
  target_link_libraries(dummy ${PROJECT_NAME})
  add_stdlibcpp_exceptions(dummy)

  option(BUILD_TESTING "" OFF) # Set to OFF by default
  include(CTest)
  if(BUILD_TESTING)
    add_subdirectory(test)
  endif()

  if(BUILD_EXAMPLE)
    add_subdirectory(examples)
  endif()
endif()

# Installation setup
include(GNUInstallDirs)
install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES
  DESTINATION include
  PUBLIC_HEADER DESTINATION include)
install(
  EXPORT ${PROJECT_NAME}Targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
install(DIRECTORY include/${PROJECT_NAME} DESTINATION include)

# Packaging setup
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  ${PROJECT_NAME}ConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)
configure_package_config_file(
  ${CMAKE_CURRENT_LIST_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
