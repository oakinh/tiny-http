#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sstream>
#include "../include/parse.h"


#define PORT 8080
#define BUFFER_LENGTH 1024

int main() {
    std::string response = "HTTP/1.1 200 OK\r\n"
"Content-Type: text/plain\r\n"
"Content-Length: 13\r\n"
"\r\n"
"Hello world\r\n";

    char buffer[BUFFER_LENGTH] = {0};
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (!sockfd) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    
    if (bind(sockfd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while (true) {
        int new_socket = accept(sockfd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if (new_socket < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
    
        int bytesRead = read(new_socket, buffer, BUFFER_LENGTH);
        if (bytesRead < 0) {
            perror("ERROR reading from socket");
            exit(EXIT_FAILURE);
        }
        buffer[bytesRead] = '\0';
        std::cout << "Message from client: " << buffer << std::endl;

        parseRequest(buffer, static_cast<size_t>(bytesRead));

        std::cout << "Writing response..." << std::endl;
        int bytesWritten = write(new_socket, response.c_str(), response.length());
        if (bytesWritten < 0) {
            perror("ERROR writing to socket");
            exit(EXIT_FAILURE);
        }
        close(new_socket);
    }
    

    close(sockfd);

    return 0;
}

