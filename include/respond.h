#pragma once

#include <iostream>

std::string buildHttpResponse(std::string body);
void sendHtmlResponse(int clientSocket, const std::string& filePath);
