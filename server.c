#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // close socket
#include <errno.h> // error codes
#include <stdbool.h> // boolean types
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <string.h>
#include <signal.h> // for interrupt signal handler
#include <assert.h> // assert()

#define PORT 9002
#define BUFFER_SIZE 1024

int server_socket; // global variable in order to be handled after SIGINT

void handle_signal(int sig); // interrupt signal handler
char *readHTML(char *filename); // utility function to read HTML file

int main(int argc, char *argv[]) 
{
    // create the server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket < 0) {
        perror("Socket failed: ");
        printf("Error code: %d\n", errno);
        return EXIT_FAILURE;
    }

    // define the server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    //server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    // bind the socket to our specified IP and port
    if ((bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address))) < 0) {
        perror("Bind failed");
        printf("Error code: %d\n", errno);
        return EXIT_FAILURE;
    }
    puts("Binding done");

    signal(SIGINT, handle_signal);
    while (true) { // continuously listen for connections
        puts("Waiting for incoming requests... (press Ctrl+C to quit)\n");
        listen(server_socket, 5);

        // accept the connection
        int client_socket; // client socket
        struct sockaddr_in *client_address=NULL; // client address pointer initialized as null pointer
        if ((client_socket = accept(server_socket, (struct sockaddr *)client_address, (socklen_t*)sizeof(client_address))) < 0) {
            perror("Accept failed");
            printf("Error code: %d\n", errno);
            return EXIT_FAILURE;
        }
        puts("Request accepted!\n");

        /* --------------- Displays client data --------------- */

        // Display client IP address
        struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&client_address;
        struct in_addr ipAddr = pV4Addr->sin_addr;
        char client_ip_address[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &ipAddr, client_ip_address, INET_ADDRSTRLEN);
        printf("Client IP address: %s\n\n", client_ip_address);

        char client_message[BUFFER_SIZE];
        if ((recv(client_socket, &client_message, sizeof(client_message), 0)) < 0) {
            perror("Receive error:");
            printf("Error code: %d\n", errno);
            return EXIT_FAILURE;
        }

        // print the client request
        printf("Data sent by the client:\n\n%s", client_message);
        memset(client_message, 0, sizeof(client_message)); // sets client message to null pointer

        /* --------------- Sending data --------------- */

        // define response content (HTML)
        //char *content = "<html>\n<head>\n<title>Hello from the server!</title>\n</head>\n<body>\n<h1>Hello from the server!</h1>\n</body>\n</html>";
        char *content = readHTML("index.html");

        // define response date
        time_t t;
        time(&t);
        char *current_date;
        current_date = ctime(&t);
        current_date[strcspn(current_date, "\n")] = 0; // removes newline for correct parsing

        char server_message[BUFFER_SIZE];
        // define response headers
        sprintf(server_message, "HTTP/1.0 200 OK\nDate: %s\nContent-Type: text/html\nContent-Length: %ld\n\n%s", current_date, strlen(content), content);
        // sends the message
        send(client_socket, &server_message, sizeof(server_message), 0);
        memset(server_message, 0, sizeof(server_message)); // sets server data to null pointer
    }
    return 0;
}

void handle_signal(int sig) 
{
    printf("\nCaught interrupt signal %d\n", sig);
    // closes the socket
    puts("Closing socket ...");
    if (close(server_socket) == 0) {
        puts("Socket closed!");
        exit(0);
    }
    else {
        perror("An error occurred while closing the socket: ");
        printf("Error code: %d\n", errno);
        exit(1);
    }
}

char *readHTML(char *filename) {
    FILE *f = fopen(filename, "rt");
    assert(f);
    fseek(f, 0, SEEK_END);
    long length = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buffer = (char *) malloc(length + 1);
    buffer[length] = '\0';
    fread(buffer, 1, length, f);
    fclose(f);
    return buffer;
}
