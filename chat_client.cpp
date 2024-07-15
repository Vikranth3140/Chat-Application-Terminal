#include <iostream>
#include <thread>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;

void receive_messages(int socket) {
    char buffer[1024];
    while (true) {
        int bytes_received = recv(socket, buffer, sizeof(buffer), 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            cout << "Server: " << buffer << endl;
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
    cout << "Connected to server\n";

    string username;
    cout << "Enter your username: ";
    getline(cin, username);

    thread(receive_messages, client_socket).detach();

    string message;
    while (true) {
        getline(cin, message);
        string full_message = username + ": " + message;
        cout << "Sending: " << full_message << endl; // Debugging statement
        send(client_socket, full_message.c_str(), full_message.size(), 0);
    }

    close(client_socket);
    return 0;
}