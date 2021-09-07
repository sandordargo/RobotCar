#include "HttpServer.h"

#include <cpprest/http_msg.h>
#include <cpprest/json.h>

#include <exception>

namespace HttpServer {
// action, like stop go?
// duration?

Direction Server::extractDirection(web::json::value body) {
  if (body.has_field("direction")) {
    auto directionInput = body.at("direction").as_string();
    std::cout << directionInput << '\n';
    
    if (directionInput == "left") {
      return Direction::Left;
    } else if (directionInput == "right") {
      return Direction::Right;
    } else if (directionInput == "forward") {
      return Direction::Forward;
    } else if (directionInput == "backward") {
      return Direction::Backward;
    }
    throw std::domain_error("bad direction");
  }
  throw std::domain_error("bad body");
}

void Server::handlePost(web::http::http_request request) {
  std::cout << "request received\n";
  std::cout << request.method() << '\n';
  web::json::value body;
  request.extract_json()       //extracts the request content into a json
    .then([&body](pplx::task<web::json::value> task)
    {
        body = task.get();
    })
    .wait();

  Direction direction = extractDirection(body);
  request.reply(web::http::status_codes::OK); //send the reply as a json
}

void Server::handleGet(web::http::http_request request) {
  std::cout << "request received\n";
  std::cout << request.method() << '\n';
  request.reply(web::http::status_codes::OK); //send the reply as a json
}


void Server::run() {
  runImplementation(std::nullopt, std::nullopt);
}

void Server::run(std::chrono::duration<int> forNSeconds) {
  auto startTime = std::chrono::system_clock::now();
  runImplementation(startTime, forNSeconds);
}

void Server::runImplementation(std::optional<std::chrono::time_point<std::chrono::system_clock>> startTime, std::optional<std::chrono::duration<int>> duration) {
  
try {
    m_listener
      .open()
      .then([this] () { std::cout << "listening...\n"; })
      .wait();
    while(hasRunDurationPassed(startTime, duration) || isEndlesslyRunning(startTime, duration));

  } catch (...) {
    std::cout << "some error happened, bye!\n";
  }

}

bool Server::hasRunDurationPassed(std::optional<std::chrono::time_point<std::chrono::system_clock>> startTime, std::optional<std::chrono::duration<int>> duration) const {
  return startTime && (std::chrono::duration_cast<std::chrono::seconds>( std::chrono::system_clock::now() - startTime.value()) <= duration.value());
}

bool Server::isEndlesslyRunning(std::optional<std::chrono::time_point<std::chrono::system_clock>> startTime, std::optional<std::chrono::duration<int>> duration) const {
  return !startTime && !duration;

}

}