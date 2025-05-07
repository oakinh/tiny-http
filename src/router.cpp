#include <iostream>
#include "parse.h"
#include "respond.h"

void routeRequest(HttpMethod method, std::string path, int clientSocket) {
    if (path == "/" || path == "/index.html") {
        sendHtmlResponse(clientSocket, "../static/index.html");
    } else if (path == "/cake.html") {
        sendHtmlResponse(clientSocket, "../static/cake.html");
    } else {
        sendHtmlResponse(clientSocket, "junk");
    }
    return;
}