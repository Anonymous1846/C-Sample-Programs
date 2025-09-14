#include <iostream>
#include <map>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>

using namespace std;

#define PORT 8080
#define MAX_CLIENTS 10

int main() {
    int server_fd, new_socket, max_sd, activity, valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    char buffer[1025]; // message buffer
    fd_set readfds;    // file descriptor set

    // Create server socket (IPv4, TCP)
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Allow reuse of port
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));

    // Bind
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    cout << "Chat server started on port " << PORT << "...\n";

    // Map socket FD → client name
    map<int, string> clients;
    vector<string> chatHistory;

    while (true) {
        // Clear and set server_fd
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);
        max_sd = server_fd;

        // Add client sockets to set
        for (auto &p : clients) {
            int sd = p.first;
            FD_SET(sd, &readfds);
            if (sd > max_sd) max_sd = sd;
        }

        // Wait for activity
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR)) {
            perror("select error");
        }

        // New connection
        if (FD_ISSET(server_fd, &readfds)) {
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                                     (socklen_t *)&addrlen)) < 0) {
                perror("Accept failed");
                exit(EXIT_FAILURE);
            }

            string name = "Client_" + to_string(new_socket);
            clients[new_socket] = name;

            string welcome = "Welcome " + name + "!\n";
            send(new_socket, welcome.c_str(), welcome.size(), 0);

            chatHistory.push_back(name + " joined the chat");

            cout << name << " connected.\n";
        }

        // Client messages
        for (auto it = clients.begin(); it != clients.end();) {
            int sd = it->first;
            if (FD_ISSET(sd, &readfds)) {
                memset(buffer, 0, 1025);
                valread = read(sd, buffer, 1024);

                if (valread == 0) { // disconnect
                    cout << it->second << " disconnected.\n";
                    close(sd);
                    it = clients.erase(it);
                    continue;
                } else {
                    buffer[valread] = '\0';
                    string msg = it->second + ": " + string(buffer);
                    chatHistory.push_back(msg);

                    // Broadcast to all clients
                    for (auto &p : clients) {
                        if (p.first != sd) {
                            send(p.first, msg.c_str(), msg.size(), 0);
                        }
                    }

                    cout << msg; // server log
                }
            }
            ++it;
        }
    }

    close(server_fd);
    return 0;
}

