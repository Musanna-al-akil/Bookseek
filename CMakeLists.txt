cmake_minimum_required(VERSION 3.10)
project(BookSeeks VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SQLite3 package
find_package(SQLite3 REQUIRED)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${SQLite3_INCLUDE_DIRS})

# Add executable
add_executable(bookseeks src/main.cpp  src/user.cpp src/database.cpp)

# Link libraries
target_link_libraries(bookseeks ${SQLite3_LIBRARIES}) 