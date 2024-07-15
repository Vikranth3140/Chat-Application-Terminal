#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <algorithm>
#include <mutex>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;

vector<int> clients;
mutex clients_mutex;

void handle_client(int client_socket) {
    char buffer[1024];
    while (true) {
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            lock_guard<mutex> guard(clients_mutex);
            clients.erase(remove(clients.begin(), clients.end(), client_socket), clients.end());
            close(client_socket);
            break;
        }
        buffer[bytes_received] = '\0';
        string message(buffer);

        cout << "Received message: " << message << endl; // Debugging statement

        lock_guard<mutex> guard(clients_mutex);
        for (int client : clients) {
            if (client != client_socket) {
                cout << "Sending to client: " << client << endl; // Debugging statement
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

    cout << "Server started on port 8080\n";

    while (true) {
        sockaddr_in client_address;
        socklen_t client_size = sizeof(client_address);
        int client_socket = accept(server_socket, (sockaddr*)&client_address, &client_size);

        lock_guard<mutex> guard(clients_mutex);
        clients.push_back(client_socket);

        thread(handle_client, client_socket).detach();
    }

    return 0;
}