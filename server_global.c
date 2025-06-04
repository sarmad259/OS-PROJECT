#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <sys/select.h>

#define PORT 8091
#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket, client_socket[MAX_CLIENTS], activity, valread, sd;
    int max_sd;
    struct sockaddr_in address;
    fd_set readfds;
    char buffer[BUFFER_SIZE];
    int addrlen = sizeof(address);

    // Initialize all client_socket[] to 0 for no clients connected initially
    for (int i = 0; i < MAX_CLIENTS; i++) {
        client_socket[i] = 0;
    }

    // Create server socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
   {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Set server address options
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the server to the address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for client connections
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d\n", PORT);

    while (1) {
        // Clear the socket set and add server_fd
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);
        max_sd = server_fd;

        // Add client sockets to set
        for (int i = 0; i < MAX_CLIENTS; i++) {
            sd = client_socket[i];
//check that client is connected or disconnected basically select () is working behind it
            if (sd > 0) {
                FD_SET(sd, &readfds);
            }
            if (sd > max_sd) {
                max_sd = sd;
            }
        }

        // Wait for an activity on one of the sockets
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR)) {
            perror("Select error");
        }

        // If something happened on the master socket, it's an incoming connection
        if (FD_ISSET(server_fd, &readfds)) {
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
                perror("Accept failed");
                exit(EXIT_FAILURE);
            }

            printf("New connection: socket fd is %d, IP is: %s, PORT: %d\n",
                   new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));

            // Add new socket to client_socket array
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (client_socket[i] == 0) {
                    client_socket[i] = new_socket;
                    printf("Adding to list of sockets as %d\n", i);
                    break;
                }
            }
        }

        // Handle I/O operations on client sockets
        for (int i = 0; i < MAX_CLIENTS; i++) {
            sd = client_socket[i];

            if (FD_ISSET(sd, &readfds)) {
                // Clear buffer
                memset(buffer, 0, BUFFER_SIZE);

                // Check if client disconnected
                if ((valread = read(sd, buffer, BUFFER_SIZE - 1)) == 0) {
                    getpeername(sd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
                    printf("Client disconnected: IP %s, PORT %d\n",
                           inet_ntoa(address.sin_addr), ntohs(address.sin_port));

                    close(sd);
                    client_socket[i] = 0;
                } else {
                    // Process the client's message
                    buffer[valread] = '\0';
                    printf("Client message: %s\n", buffer);

                    // Prompt server for a reply
                    char server_response[BUFFER_SIZE];
                    printf("Enter response for client: ");
                    fgets(server_response, BUFFER_SIZE, stdin);
                    server_response[strcspn(server_response, "\n")] = 0;  // Remove the newline from input

                    // Send the response back to the client
                    send(sd, server_response, strlen(server_response), 0);
                }
            }
        }
    }

    close(server_fd);
    return 0;
}
