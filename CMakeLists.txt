# Add source files in this directory to the library
add_library(qdsp 
# src/q.cpp 
src/qfloat.cpp 
src/qmusic.cpp)

# Include directories
target_include_directories(qdsp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
