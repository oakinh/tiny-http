#pragma once

#include "parse.h"
#include <iostream>

void routeRequest(HttpMethod method, std::string path, int clientSocket);