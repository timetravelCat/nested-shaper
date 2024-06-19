# =============================================================================
#
# parse_git_tag
#
# This function set VERSION_* variables from git tag in parent scope Git tag is
# assumed as "vX.Y.Z" where X, Y and Z are integers
#
# Usage: include(parse_git_tag) # automatically parse_git_tag() and set
# VERSION_* variables
#
# Output: VERSION_MAJOR VERSION_MINOR VERSION_PATCH VERSION_TWEAK
#
function(parse_git_tag)
  execute_process(
    COMMAND git describe --exclude ext/* --always --tags
    OUTPUT_VARIABLE GIT_TAG
    OUTPUT_STRIP_TRAILING_WHITESPACE
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

  # If version is not vX.Y.Z, set v0.0.0
  if(NOT GIT_TAG MATCHES "^v[0-9]+\\.[0-9]+\\.[0-9]+")
    message(WARNING "Git tag is not vX.Y.Z")
    set(GIT_TAG "v0.0.0")
  endif()

  # git describe to X.Y.Z version
  string(REPLACE "." ";" VERSION_LIST ${GIT_TAG})

  # major version
  list(GET VERSION_LIST 0 VERSION_MAJOR)
  string(REPLACE "v" "" VERSION_MAJOR ${VERSION_MAJOR})
  set(VERSION_MAJOR
      ${VERSION_MAJOR}
      PARENT_SCOPE)
  # minor version
  list(GET VERSION_LIST 1 VERSION_MINOR)
  set(VERSION_MINOR
      ${VERSION_MINOR}
      PARENT_SCOPE)
  # patch version
  list(GET VERSION_LIST 2 VERSION_PATCH)
  string(REPLACE "-" ";" VERSION_PATCH ${VERSION_PATCH})
  list(GET VERSION_PATCH 0 VERSION_PATCH)
  set(VERSION_PATCH
      ${VERSION_PATCH}
      PARENT_SCOPE)
  # tweak version
  execute_process(
    COMMAND git rev-parse --short HEAD
    OUTPUT_VARIABLE VERSION_TWEAK
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  set(VERSION_TWEAK
      ${VERSION_TWEAK}
      PARENT_SCOPE)

  message(
    STATUS
      "version: v${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}-${VERSION_TWEAK}"
  )
endfunction()

parse_git_tag()
