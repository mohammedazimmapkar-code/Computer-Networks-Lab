#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;

int main() {
    int serverSocket, clientSocket;
    char buffer[1024] = {0};

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (sockaddr*)&serverAddr,
         sizeof(serverAddr));

    listen(serverSocket, 1);
    cout << "Waiting for client..." << endl;

    clientSocket = accept(serverSocket, nullptr, nullptr);
    cout << "Client connected!" << endl;

    while (true) {
        memset(buffer, 0, sizeof(buffer));

        int bytes = recv(clientSocket, buffer,
                         sizeof(buffer) - 1, 0);

        if (bytes <= 0)
            break;

        cout << "Message: " << buffer << endl;
    }

    close(clientSocket);
    close(serverSocket);

    return 0;
}
