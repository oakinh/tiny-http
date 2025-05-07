#include <iostream>
#include <sstream>
#include "parse.h"
#include <string>
#include <algorithm>

HttpMethod stringToMethod(const std::string &raw) {
    std::string s = raw;
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    if (s == "GET") return HttpMethod::GET;
    else if (s == "POST") return HttpMethod::POST;
    else if (s == "PUT") return HttpMethod::PUT;
    else if (s == "PATCH") return HttpMethod::PATCH;
    else if (s == "DELETE") return HttpMethod::DELETE_;
    else return HttpMethod::UNKNOWN; 
}

HttpRequest parseRequest(const char* request, size_t arrSize) {

    HttpRequest parsedRequest{};
    
    std::string line;

    bool lineComplete;
    bool inBody;
    int  bodySize = 0;
    std::cout << "arrSize: " << arrSize << std::endl;
    for (size_t i = 0; i < arrSize; ++i) {
        if (inBody) {
            parsedRequest.body[bodySize] = request[i];
            ++bodySize;
            continue;
        }
        if (lineComplete) {
            // Process line

            if (parsedRequest.method == HttpMethod::UNKNOWN) {
                std::cout << "Parsing request..." << std::endl;
                std::string methodStr;
                std::istringstream stream(line);
                stream 
                    >> methodStr 
                    >> parsedRequest.path 
                    >> parsedRequest.protocolVersion; 

                parsedRequest.method = stringToMethod(methodStr);               
            } else {
                // Handle other headers, order not guaranteed
                std::istringstream stream(line);
                std::string headerName, headerValue;

                std::getline(stream, headerName, ' ');
                std::getline(stream, headerValue);

            }
            lineComplete = false;
            line = "";
        }
        if (request[i] == '\r' && request[i+1] == '\n') {
            lineComplete = true;
            if (request[i+2] == '\r' && request[i+3] == '\n') {
                inBody = true;
            }
        }
        line += request[i];
    }

    return parsedRequest;
}

std::string methodToString(HttpMethod method) {
    switch (method) {
        case HttpMethod::GET: return "GET";
        case HttpMethod::POST: return "POST";
        case HttpMethod::PUT: return "PUT";
        case HttpMethod::PATCH: return "PATCH";
        case HttpMethod::DELETE_: return "DELETE";
        default: return "UNKNOWN"; 
    }
}