#if defined(_WIN32)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#else
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <error.h>
#include <unistd.h>

#endif

#if defined(_WIN32)
#define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET)
#define CLOSESOCKET(s) closesocket(s)
#define GETSOCKETERRNO() (WSAGetLastError())

#else
#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define GETSOCKETERRNO() (errno)
#define SOCKET int
#endif

#include <stdio.h>
#include <time.h>
#include <string.h>

int main()
{
#if defined(_WIN32)
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d))
    {
        fprintf(stderr, "Failed to initialize. \n");
        return 1;
    }
#endif

    printf("Configuring the local address... \n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo *bind_address;
    getaddrinfo(0, "8080", &hints, &bind_address);

    printf("Creating sockets... \n");
    SOCKET socket_listen;
    socket_listen = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);

    // check if call to socket was successful
    if (!ISVALIDSOCKET(socket_listen))
    {
        fprintf(stderr, "socket() failed. (%d) \n", GETSOCKETERRNO());
        return 1;
    }

    // bind the socket
    printf("Binding the socket to local address ...\n");
    if (bind(socket_listen, bind_address->ai_addr, bind_address->ai_addrlen))
    {
        fprintf(stderr, "bind() failed. (%d) \n", GETSOCKETERRNO());
        return 1;
    }
    // free memory
    freeaddrinfo(bind_address);

    // start listening
    printf("Listening ...\n");
    if (listen(socket_listen, 10) < 0)
    {
        fprintf(stderr, "listen() failed. (%d) \n", GETSOCKETERRNO());
        return 1;
    }

    // wait for connection
    printf("Waiting for connection ...\n");
    struct sockaddr_storage client_address;
    socklen_t client_len = sizeof(client_address);
    SOCKET socket_client = accept(socket_listen, (struct sockaddr *)&client_address, &client_len);
    if (!ISVALIDSOCKET(socket_client))
    {
        fprintf(stderr, "accept() failed. (%d) \n", GETSOCKETERRNO());
        return 1;
    }

    // print client address to the console
    printf("Client is connected ...\n");
    char address_buff[100];
    getnameinfo((struct sockaddr *)&client_address, client_len, address_buff, sizeof(address_buff), 0, 0, NI_NUMERICHOST);
    printf("%s\n", address_buff);

    // read HTTP request
    printf("Reading request ...\n");
    char request[1024];
    int bytes_received = recv(socket_client, request, 1024, 0);
    printf("Received %d bytes.\n", bytes_received);
    printf("%.*s", bytes_received, request);

    // sending response
    printf("Sending response ...\n");
    const char *response =
        "HTTP/1.1 200 OK\r\n"
        "Connection: close\r\n"
        "Content-type: text/plain\r\n\r\n" // or text/html for html
        "Local time is: ";
    int bytes_sent = send(socket_client, response, strlen(response), 0);
    printf("Sent %d of %d bytes\n", bytes_sent, (int)strlen(response));

    time_t timer;
    time(&timer);
    char *time_msg = ctime(&timer);
    bytes_sent = send(socket_client, time_msg, strlen(time_msg), 0);
    printf("Sent %d of %d bytes\n", bytes_sent, (int)strlen(time_msg));

    // Sending hello world!!
    // char *my_message = "<br> This is my message: <h1>Hello world</h1>";
    // bytes_sent = send(socket_client, my_message, strlen(my_message), 0);
    // printf("Sent %d of %d bytes\n", bytes_sent, (int)strlen(my_message));

    printf("Closing the connection...\n");
    CLOSESOCKET(socket_client);

    printf("Closing the listening socket...\n");
    CLOSESOCKET(socket_listen);
#if defined(_WIN32)
    WSACleanup();
#endif
    printf("Finished...");

    return 0;
}