#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8081

int main() {
    int client_fd;
    struct sockaddr_in server_address;
    char buffer[1024] = {0};

    // Create socket
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Specify server address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    // Convert IP to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) { // Change "127.0.0.1" to server's IP for remote
        perror("Invalid address");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(client_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Connection failed");
        close(client_fd);
        exit(EXIT_FAILURE);
    }
    printf("Connected to server\n");

    // Communicate with server
    while (1) {
        printf("Enter message: ");
        fgets(buffer, sizeof(buffer), stdin);
        send(client_fd, buffer, strlen(buffer), 0);

        memset(buffer, 0, sizeof(buffer));
        int valread = read(client_fd, buffer, sizeof(buffer));
        if (valread > 0) {
            printf("Server: %s\n", buffer);
        }
    }

    close(client_fd);
    return 0;
}

