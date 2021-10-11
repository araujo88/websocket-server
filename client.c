#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // close socket
#include <errno.h> // error codes
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 9002
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) 
{
    int network_socket; // create a socket
    network_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (network_socket < 0) {
        perror("Socket creation error");
        printf("Error code: %d\n", errno);
        return EXIT_FAILURE;
    }

    // specify an address for the socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET; // specifies protocol IPv4
    server_address.sin_port = htons(PORT); // specifies port
    //server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    // connection 
    int connection_status = connect(network_socket, (struct sockaddr *) &server_address, sizeof(server_address));

    // check for error in the connection
    if (connection_status < 0) {
        perror("Connection error: ");
        printf("Error code: %d\n", errno);
        return EXIT_FAILURE;
    }

    // sends message to the server
    char *client_message = "GET / HTTP/1.1\n";
    //char *client_message = "Hello from the client!\n\n";
    send(network_socket, client_message, strlen(client_message), 0);

    // Receive data from the server
    char buffer[BUFFER_SIZE]; // server response
    if ((recv(network_socket, &buffer, sizeof(buffer), 0)) < 0) {
        perror("Receive error:");
        printf("Error code: %d\n", errno);
        return EXIT_FAILURE;
    }

    // print the server's response
    printf("The server sent the data:\n%s\n", buffer);

    // close the socket
    close(network_socket);
    return 0;
}
