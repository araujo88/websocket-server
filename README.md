# C-websocket-endpoint
An application which uses the C library socket.h to create a server to handle a HTTP GET request with response 200 and HTML data. Must be compiled and run in Unix systems.

From a terminal, compile using:
 - gcc server.c -o server.o

Run the application:
 - ./server.o

In any web browser, type any of the following addresses:
 - localhost:9002
 - 127.0.0.1:9002

Then, the browser should display a page with the string "Hello from the server!".
The full response from the server contains the following (example date shown):
```
HTTP/1.0 200 OK
Date: Sun Oct 10 00:17:35 2021 GMT-3
Content-Type: text/html
Content-Length: 114

<html>
<head>
<title>Hello from the server!</title>
</head>
<body>
<h1>Hello from the server!</h1>
</body>
</html>
```
