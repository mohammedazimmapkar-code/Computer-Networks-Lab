#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080
#define MAXLINE 1024

int main(void) {
    int sockfd;
    char buffer[MAXLINE];
    char reply[MAXLINE];

    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *)&servaddr,
             sizeof(servaddr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("UDP server listening on port %d...\n", PORT);

    while (true) {
        len = sizeof(cliaddr);

        int n = recvfrom(sockfd, buffer, MAXLINE - 1, 0,
                         (struct sockaddr *)&cliaddr, &len);

        if (n < 0) {
            perror("recvfrom failed");
            continue;
        }

        buffer[n] = '\0';
        printf("\nClient: %s\n", buffer);

        if (strcmp(buffer, "exit") == 0) {
            printf("Client disconnected.\n");
            break;
        }

        printf("Server reply: ");
        if (fgets(reply, sizeof(reply), stdin) == NULL) {
            break;
        }

        reply[strcspn(reply, "\n")] = '\0';

        if (sendto(sockfd, reply, strlen(reply), 0,
                   (struct sockaddr *)&cliaddr, len) < 0) {
            perror("sendto failed");
            continue;
        }

        printf("Reply sent.\n");

        if (strcmp(reply, "exit") == 0) {
            printf("Server closing connection.\n");
            break;
        }
    }

    close(sockfd);
    return 0;
}
