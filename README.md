# Chat Application

This is a simple chat application that consists of a C++ server and a C++ client. The server handles multiple client connections, and clients can send and receive messages in real-time.

## Project Structure

```
Chat-Application/
    ├── chat_server.cpp
    ├── chat_client.cpp
    ├── CMakeLists.txt
    ├── Makefile
    ├── .gitignore
    ├── LICENSE
    └── README.md
```

## Requirements

- C++ compiler (GCC, Clang, or MSVC)
- CMake (for CMake build)

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
    ./chat_server
    ```

3. **Run the client**:

    ```bash
    ./chat_client
    ```

### Using Makefile

1. **Compile the project**:

    ```bash
    make
    ```

2. **Run the server**:

    ```bash
    ./chat_server
    ```

3. **Run the client**:

    ```bash
    ./chat_client
    ```

## Running the Application

1. Ensure the server is running as described above.
2. Run the client, enter a username, and start sending messages.

## File Descriptions

- **chat_server.cpp**: The C++ server code that handles multiple clients.
- **chat_client.cpp**: The C++ client code that connects to the server and sends/receives messages.
- **CMakeLists.txt**: CMake configuration file for building the project.
- **Makefile**: Makefile for building the project.

## License

This project is licensed under the [MIT License](LICENSE).