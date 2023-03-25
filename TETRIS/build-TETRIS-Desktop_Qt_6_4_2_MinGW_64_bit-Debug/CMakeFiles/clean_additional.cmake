# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\TETRIS_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\TETRIS_autogen.dir\\ParseCache.txt"
  "TETRIS_autogen"
  )
endif()
