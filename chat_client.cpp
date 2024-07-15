#include <iostream>
#include <thread>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>

void receive_messages(int socket) {
    char buffer[1024];
    while (true) {
        int bytes_received = recv(socket, buffer, sizeof(buffer), 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            std::cout << "Server: " << buffer << std::endl;
        }
    }
}

int main() {
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);

    connect(client_socket, (sockaddr*)&server_address, sizeof(server_address));
    std::cout << "Connected to server\n";

    std::string username;
    std::cout << "Enter your username: ";
    std::getline(std::cin, username);

    std::thread(receive_messages, client_socket).detach();

    std::string message;
    while (true) {
        std::getline(std::cin, message);
        std::string full_message = username + ": " + message;
        std::cout << "Sending: " << full_message << std::endl; // Debugging statement
        send(client_socket, full_message.c_str(), full_message.size(), 0);
    }

    close(client_socket);
    return 0;
}