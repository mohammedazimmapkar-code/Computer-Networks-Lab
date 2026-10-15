#include <iostream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);

    inet_pton(AF_INET, "127.0.0.1",
              &serverAddr.sin_addr);

    if (connect(sock, (sockaddr*)&serverAddr,
                sizeof(serverAddr)) < 0) {
        cout << "Connection failed!" << endl;
        close(sock);
        return 1;
    }

    cout << "Connected to server!" << endl;
    cout << "Type messages (exit to quit)." << endl;

    string message;

    while (getline(cin, message)) {
        if (message == "exit")
            break;

        message += '\n';

        if (send(sock, message.c_str(),
                 message.size(), 0) < 0) {
            cout << "Send failed!" << endl;
            break;
        }
    }

    close(sock);
    return 0;
}
