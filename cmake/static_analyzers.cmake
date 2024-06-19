# Cppcheck setup
find_program(CPPCHECK cppcheck)
if(CPPCHECK)
  message(STATUS "CMAKE_BINARY_DIR found: ${CMAKE_BINARY_DIR}")
  set(CMAKE_CXX_CPPCHECK
      ${CPPCHECK}
      --enable=all
      --force
      --inconclusive
      --suppress=missingIncludeSystem
      --suppress=noExplicitConstructor
      --std=c++11)
  message(STATUS "cppcheck found: ${CPPCHECK}")
else()
  message(WARNING "cppcheck not found")
endif()

# Clang-tidy setup
find_program(CLANGTIDY clang-tidy)
if(CLANGTIDY)
  set(CMAKE_CXX_CLANG_TIDY ${CLANGTIDY})
  message(STATUS "clang-tidy found: ${CLANGTIDY}")
else()
  message(WARNING "clang-tidy not found")
endif()
