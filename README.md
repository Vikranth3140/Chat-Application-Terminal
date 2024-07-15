# Chat Application

This is a simple chat application that consists of a C++ server using Boost.Beast for WebSocket communication and an HTML/JavaScript client for the frontend.

## Project Structure

```
Chat-Application/
├── CMakeLists.txt
├── Makefile
├── chat_client.cpp
├── chat_server.cpp
└── index.html
```

## Requirements

- C++ compiler (GCC, Clang, or MSVC)
- Boost libraries (system, thread)
- CMake (for CMake build)
- A web browser to run the client

## Building the Project

1. **Create a `CMakeLists.txt` file** in your project directory:

    ```cmake
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
    ```

2. **Create the build directory and compile the project**:

    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```

3. **Run the server**:

    ```bash
    ./chat_server 0.0.0.0 8080
    ```

## Running the Client

1. Ensure the server is running as described above.
2. Open the `index.html` file in a web browser.

## File Descriptions

- **chat_server.cpp**: The C++ server code using Boost.Beast for WebSocket communication.
- **index.html**: The HTML/JavaScript client code.
- **CMakeLists.txt**: CMake configuration file for building the project.
- **Makefile**: Makefile for building the project.

## Dependencies

- Boost (system, thread)
- CMake (for building with CMake)
- GCC, Clang, or MSVC (for compiling the C++ code)

## License

This project is licensed under the [MIT License](LICENSE).