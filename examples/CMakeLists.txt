message(STATUS "Configuring examples")

include(FetchContent)

# disable ruckig's examples, cloud client, and tests
set(BUILD_EXAMPLES
    OFF
    CACHE INTERNAL "")
set(BUILD_CLOUD_CLIENT
    OFF
    CACHE INTERNAL "")
set(BUILD_TESTS
    OFF
    CACHE INTERNAL "")

FetchContent_Declare(
  ruckig
  GIT_REPOSITORY https://github.com/pantor/ruckig
  GIT_TAG v0.12.2)

FetchContent_MakeAvailable(ruckig)

find_package(Matplot++ REQUIRED)

file(GLOB_RECURSE EXAMPLES *.cpp)

foreach(example ${EXAMPLES})
  # Create a target for each test file
  get_filename_component(target ${example} NAME_WE)
  message(STATUS "Adding example: ${target}")
  add_executable(${target} ${example})
  target_link_libraries(${target} PRIVATE ${PROJECT_NAME} Matplot++::cimg
                                          Matplot++::matplot ruckig)
  target_compile_options(
    ${target} PRIVATE -Wno-double-promotion -Wno-unused-variable
                      -Wno-unused-but-set-variable)
endforeach()
