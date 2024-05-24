#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <iostream>

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char *message = "Hello from client\n";

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << ("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9090);  // Replace with your server port

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cout << ("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << ("\nConnection Failed \n");
        return -1;
    }
    
    send(sock, message, strlen(message), 0);
    while (1) {
    }
    printf("Message sent\n");

    return 0;
}
