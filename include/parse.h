#pragma once

#include <iostream>

enum class HttpMethod {
    GET,
    POST,
    PUT,
    PATCH,
    DELETE_,
    UNKNOWN,
};

struct HttpRequest {
    HttpMethod method = HttpMethod::UNKNOWN;
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

HttpMethod stringToMethod(const std::string &raw);
HttpRequest parseRequest(const char* request, size_t arrSize);
std::string methodToString(HttpMethod method);