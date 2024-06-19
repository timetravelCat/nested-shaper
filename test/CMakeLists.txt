message(STATUS "Configuring tests")

# include(FetchContent)

# set(CATCH_INSTALL_DOCS OFF CACHE INTERNAL "") set(CATCH_INSTALL_EXTRAS OFF
# CACHE INTERNAL "")

# FetchContent_Declare( Catch2 GIT_REPOSITORY
# https://github.com/catchorg/Catch2.git GIT_TAG v3.4.0 # or a later release )
# FetchContent_MakeAvailable(Catch2)

# list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras) include(Catch)

find_package(Catch2 3 REQUIRED)
include(Catch)

file(GLOB_RECURSE TESTS *.cpp)

foreach(test ${TESTS})
  # Create a target for each test file
  get_filename_component(target ${test} NAME_WE)
  message(STATUS "Adding test: ${target}")
  add_executable(${target} ${test})
  target_link_libraries(${target} PRIVATE ${PROJECT_NAME}
                                          Catch2::Catch2WithMain)
  target_compile_options(
    ${target} PRIVATE -Wno-double-promotion -Wno-unused-variable
                      -Wno-unused-but-set-variable)
  catch_discover_tests(${target})
endforeach()