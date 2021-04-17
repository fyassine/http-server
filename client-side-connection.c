
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
    char buffer[1024] = {0};
    
    //opening socket
    int socket_ret = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_ret < 0)
    {
        printf("socket was not created\n");
        return 0;
    }
    
    struct sockaddr_in server_addr;
    //memset(&serv_addr, '0', sizeof(serv_addr));
    server_addr.sin_family = AF_INET;
    const int PORT = 8080;
    server_addr.sin_port = htons(PORT);

    // convert ip address to binary from string
    int inet_ret = inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) ;
    if (inet_ret <= 0)
    {
        printf("invalid address\n");
        return 0;
    }

    // connect to server (local host)
    int connect_ret = connect(socket_ret, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (connect_ret < 0)
    {
        printf("connection failed, error code: %s\n", strerror(errno));
        return -1;
    }

    
    char *message = "message sent from client\n";
    send(socket_ret, message, strlen(message), 0);
    printf("\nmessage sent. waiting for confirmation ...\n");

    int valread = read(socket_ret, buffer, 1024);
    printf("%s\n", buffer);
    return 0;
}
