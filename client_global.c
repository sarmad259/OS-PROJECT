#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8090

int main()
{
    int sock;
    struct sockaddr_in server_address;
    char buffer[1024];
    char message[1024];

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket creation error");
        return -1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    // Convert IP address to binary form
    if (inet_pton(AF_INET, "192.168.1.19", &server_address.sin_addr) <= 0)
    {
        perror("Invalid address / Address not supported");
        return -1;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("Connection to server failed");
        return -1;
    }
    printf("Connected to the server\n");

    // Communication loop
    while (1)
    {
        printf("Enter message: ");
        fgets(message, sizeof(message), stdin);

        // Remove newline character from the message (if present)
        message[strcspn(message, "\n")] = 0;

        // Send message to server
        if (send(sock, message, strlen(message), 0) < 0)
        {
            perror("Send failed");
            break;
        }

        // Read server's response
        int valread = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (valread > 0)
        {
            buffer[valread] = '\0';
            printf("Server response: %s\n", buffer);
        }
        else if (valread == 0)
        {
            printf("Server closed the connection.\n");
            break;
        }
        else
        {
            perror("Recv failed");
            break;
        }
    }

    close(sock);
    return 0;
}
	
