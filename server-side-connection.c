#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

int main(void)
{
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char *message = "message received by server\n";
    int opt = 1;
    
    //socket creation
    int socket_server_ret = socket(AF_INET, SOCK_STREAM, 0);
    if (!socket_server_ret)
    {
        perror("socket was not created\n");
        return 0;
    }

    // allow reuse of address and port
    if (setsockopt(socket_server_ret, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(int)))
    {
        perror("setsockopt returned an error");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    const int PORT = 8080;
    address.sin_port = htons(PORT);

    //memset((char *)&address, 0, addrlen);

    // bind socket to address and port
    int bind_ret = bind(socket_server_ret, (struct sockaddr *)&address, addrlen);
    if (bind_ret < 0)
    {
        perror("bind failed");
        return 0;
    }

    // wait for connection requests
    int listen_ret = listen(socket_server_ret, 3);
    if (listen_ret < 0)
    {
        perror("Error while listening");
        exit(EXIT_FAILURE);
    }

    while (1)
    {

        printf("\n----Waiting for new connection----\n");

        // accept connection from client
        int socket_client_ret = accept(socket_server_ret, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if ((socket_client_ret) < 0)
        {
            perror("error while accepting connection");
            exit(EXIT_FAILURE);
        }

        char buffer[1024] = {0};

        //recv(socket_client_ret, buffer, 25000, MSG_CMSG_CLOEXEC);
        int valread = read(socket_client_ret, buffer, 1024);
        printf("%s", buffer);

        send(socket_client_ret, message, strlen(message), 0);
        
        printf("confirmation sent !\n");

        close(socket_client_ret);
    }

    return 0;
}
