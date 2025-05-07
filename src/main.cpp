#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sstream>
#include "../include/parse.h"
#include "../include/respond.h"
#include <format>
#include "router.h"

#define PORT 8080
#define BUFFER_LENGTH 1024

int main() {
    char buffer[BUFFER_LENGTH] = {0};
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (!sockfd) {
        std::cerr << "socket failed\n";
        exit(EXIT_FAILURE);
    }
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    
    if (bind(sockfd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "bind failed\n";
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, 3) < 0) {
        std::cerr << "listen\n";
        exit(EXIT_FAILURE);
    }

    while (true) {
        int newSocket = accept(sockfd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if (newSocket < 0) {
            std::cerr << "accept\n";
            exit(EXIT_FAILURE);
        }
    
        int bytesRead = read(newSocket, buffer, BUFFER_LENGTH);
        if (bytesRead < 0) {
            std::cerr << "ERROR reading from socket\n";
            exit(EXIT_FAILURE);
        }
        buffer[bytesRead] = '\0';
        std::cout << "Message from client: " << buffer << std::endl;

        HttpRequest parsedRequest = parseRequest(buffer, static_cast<size_t>(bytesRead));

        std::cout << "Writing response..." << std::endl;
        routeRequest(parsedRequest.method, parsedRequest.path, newSocket);

        close(newSocket);
    }
    

    close(sockfd);

    return 0;
}
