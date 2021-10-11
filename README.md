# C-websocket-endpoint
An application which uses the C library socket.h to create a server to handle a HTTP GET request with response 200 and HTML data. Must be compiled and run in Unix systems.

From a terminal, compile using:
 - gcc server.c -o server.o

Run the application:
 - ./server.o

In a web browser, type any of the following addresses:
 - localhost:9002
 - 127.0.0.1:9002

Then, the browser should display the page "index.html". If another URL is specified, the server returns a default "Error 404" page.
