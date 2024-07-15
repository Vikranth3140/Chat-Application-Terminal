#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <mutex>
#include <unistd.h>
#include <arpa/inet.h>

std::vector<int> clients;
std::mutex clients_mutex;


void handle_client(int client_socket) {
    char buffer[1024];
    while (true) {
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            std::lock_guard<std::mutex> guard(clients_mutex);
            clients.erase(std::remove(clients.begin(), clients.end(), client_socket), clients.end());
            close(client_socket);
            break;
        }
        buffer[bytes_received] = '\0';
        std::string message(buffer);

        std::lock_guard<std::mutex> guard(clients_mutex);
        for (int client : clients) {
            if (client != client_socket) {
                send(client, message.c_str(), message.size(), 0);
            }
        }
    }
}



int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (sockaddr*)&server_address, sizeof(server_address));
    listen(server_socket, SOMAXCONN);

    std::cout << "Server started on port 8080\n";

    while (true) {
        sockaddr_in client_address;
        socklen_t client_size = sizeof(client_address);
        int client_socket = accept(server_socket, (sockaddr*)&client_address, &client_size);

        std::lock_guard<std::mutex> guard(clients_mutex);
        clients.push_back(client_socket);

        std::thread(handle_client, client_socket).detach();
    }

    return 0;
}
