# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11

# Targets
SERVER_TARGET = chat_server
CLIENT_TARGET = chat_client

# Sources
SERVER_SRCS = chat_server.cpp
CLIENT_SRCS = chat_client.cpp

# Build server target
$(SERVER_TARGET): $(SERVER_SRCS)
	$(CXX) $(CXXFLAGS) -o $(SERVER_TARGET) $(SERVER_SRCS)

# Build client target
$(CLIENT_TARGET): $(CLIENT_SRCS)
	$(CXX) $(CXXFLAGS) -o $(CLIENT_TARGET) $(CLIENT_SRCS)

# Clean target
clean:
	rm -f $(SERVER_TARGET) $(CLIENT_TARGET)