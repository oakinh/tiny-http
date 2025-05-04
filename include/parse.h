#pragma once

#include <iostream>

struct HttpRequest {
    std::string method;
    std::string path;
    std::string query;
    std::string protocolVersion;
    std::string host;
    std::string contentType;
    std::string contentLength;
    std::string userAgent;
    std::string accept;
    std::string connection;
    std::string body;
};

enum class HttpMethod {
    GET,
    POST,
    PUT,
    PATCH,
    DELETE,
};

HttpRequest parseRequest(const char* request, size_t arrSize);
std::string methodToString(HttpMethod method);