#include "HttpServer.h"
#include <chrono>

int main() {
  using namespace std::chrono_literals;
  HttpServer::Server server{"http://0.0.0.0:8080/drive"};
  server.run();
}
