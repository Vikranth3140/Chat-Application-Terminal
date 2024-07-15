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

### Using CMake

1. **Create the build directory and compile the project**:

    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```

2. **Run the server**:

    ```bash
    ./chat_server 0.0.0.0 8080
    ```

## Running the Client

1. Ensure the server is running as described above.
2. Open the `index.html` file in a web browser.

## File Descriptions

- **chat_server.cpp**: The C++ server code using Boost.Beast for WebSocket communication.
- **index.html**: The HTML/JavaScript client code with Bootstrap for styling.
- **CMakeLists.txt**: CMake configuration file for building the project.
- **Makefile**: Makefile for building the project.

## Dependencies

- Boost (system, thread)
- CMake (for building with CMake)
- GCC, Clang, or MSVC (for compiling the C++ code)

## License

This project is licensed under the [MIT License](LICENSE).