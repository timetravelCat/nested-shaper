find_package(Matplot++ CONFIG REQUIRED)

add_executable(examples examples.cpp)
target_link_libraries(examples PRIVATE Matplot++::cimg  Matplot++::matplot nested-shaper::nested-shaper)