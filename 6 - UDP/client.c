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

    struct sockaddr_in servaddr;
    socklen_t len = sizeof(servaddr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1",
                  &servaddr.sin_addr) != 1) {
        fprintf(stderr, "Invalid server IP address\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    while (true) {
        printf("\nClient message: ");

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }

        buffer[strcspn(buffer, "\n")] = '\0';

        if (sendto(sockfd, buffer, strlen(buffer), 0,
                   (struct sockaddr *)&servaddr,
                   sizeof(servaddr)) < 0) {
            perror("sendto failed");
            break;
        }

        if (strcmp(buffer, "exit") == 0) {
            printf("Client exiting.\n");
            break;
        }

        int n = recvfrom(sockfd, buffer, MAXLINE - 1, 0,
                         (struct sockaddr *)&servaddr, &len);

        if (n < 0) {
            perror("recvfrom failed");
            break;
        }

        buffer[n] = '\0';
        printf("Server: %s\n", buffer);

        if (strcmp(buffer, "exit") == 0) {
            printf("Server ended the conversation.\n");
            break;
        }
    }

    close(sockfd);
    return 0;
}
