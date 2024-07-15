#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <algorithm>
#include <mutex>
#include <map>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctime>

using namespace std;

map<int, string> clients;
mutex clients_mutex;

string get_current_time() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char time_str[9];
    strftime(time_str, sizeof(time_str), "%H:%M:%S", ltm);
    return string(time_str);
}

void handle_client(int client_socket) {
    char buffer[1024];
    while (true) {
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            lock_guard<mutex> guard(clients_mutex);
            cout << "Client disconnected: " << clients[client_socket] << endl;
            clients.erase(client_socket);
            close(client_socket);
            break;
        }
        buffer[bytes_received] = '\0';
        string message(buffer);

        cout << "Received message: " << message << endl;

        string timestamped_message = "[" + get_current_time() + "] " + message;

        lock_guard<mutex> guard(clients_mutex);
        for (const auto& client : clients) {
            if (client.first != client_socket) {
                cout << "Sending to client: " << client.first << endl;
                send(client.first, timestamped_message.c_str(), timestamped_message.size(), 0);
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

        char buffer[1024];
        recv(client_socket, buffer, sizeof(buffer), 0);
        string username(buffer);

        lock_guard<mutex> guard(clients_mutex);
        clients[client_socket] = username;
        cout << "Client connected: " << username << endl;

        thread(handle_client, client_socket).detach();
    }

    return 0;
}