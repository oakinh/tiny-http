#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <vector>
#include <format>

std::string buildHttpResponse(std::string body) {
    return std::format("HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Content-Length: {}\r\n"
    "\r\n"
    "{}",
    body.size(), body);
}

void sendHtmlResponse(int clientSocket, const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::string notFound = "HTTP/1.1 404 Not Found\r\nContent-Length: 13\r\n\r\n404 Not Found";
        write(clientSocket, notFound.c_str(), notFound.length());
        return;
    }

    file.seekg(0, std::ios::end);
    auto size = static_cast<std::size_t>(file.tellg());
    file.seekg(0, std::ios::beg);
    std::string body(size, '\0');
    file.read(&body[0], size);

    std::string response = buildHttpResponse(body);
    int bytesSent = write(clientSocket, response.data(), response.size());
    if (bytesSent < 0) {
        std::cerr << "Error writing to socket\n";
        return;
    }
}