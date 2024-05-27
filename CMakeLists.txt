cmake_minimum_required(VERSION 3.28)
project(HELLOWORLD)

set(CMAKE_CXX_STANDARD 17)
find_package(sqlite3 REQUIRED)

add_executable(airplanmangment main.cpp sqlite/sqlite3.c)


#set(CMAKE_CXX_STANDARD 11)

# Find the SQLite3 library

# Add the executable
#add_executable(SQLiteExample main.cpp ../sqlite/sqlite3.c)

# Link against the SQLite3 library
#target_link_libraries(SQLiteExample SQLite::SQLite3)

