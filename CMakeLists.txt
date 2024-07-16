cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(ChatApp VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the server executable
add_executable(chat_server chat_server.cpp)

# Add the client executable
add_executable(chat_client chat_client.cpp)