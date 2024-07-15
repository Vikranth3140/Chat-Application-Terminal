cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(ChatServer VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find Boost libraries
find_package(Boost 1.70 REQUIRED COMPONENTS system thread)

# Add the executable
add_executable(chat_server chat_server.cpp)

# Link Boost libraries
target_link_libraries(chat_server Boost::system Boost::thread pthread)
