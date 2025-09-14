#include <iostream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include<cstring>
using namespace std;

void receiveMessages(int sock) {
    char buffer[1024];
    while (true) {
        memset(buffer, 0, 1024);
        int valread = read(sock, buffer, 1024);
        if (valread > 0) {
            cout << "\n" << buffer << endl;
            cout << "Enter message: ";
            cout.flush();
        }
    }
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    string message;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "Socket creation error\n";
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        cout << "Invalid address/Not supported\n";
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        cout << "Connection Failed\n";
        return -1;
    }

    // Start a receiving thread
    thread receiver(receiveMessages, sock);
    receiver.detach();

    while (true) {
        cout << "Enter message: ";
        getline(cin, message);
        send(sock, message.c_str(), message.size(), 0);
    }

    close(sock);
    return 0;
}

