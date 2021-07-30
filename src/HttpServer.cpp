#include "HttpServer.h"

#include <cpprest/http_listener.h>
#include <cpprest/http_msg.h>
#include <cpprest/json.h>
#include <cpprest/uri.h>

#include <exception>

namespace HttpServer {
// action, like stop go?
// duration?

Direction extractDirection(web::json::value body) {
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

void handle_post(web::http::http_request request) {
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

void run(const std::string& uri) {
  web::uri address(uri);
  web::http::experimental::listener::http_listener listener(address);

  listener.support(web::http::methods::POST, handle_post);

  try {
    listener
      .open()
      .then([&listener] () { std::cout << "listening...\n"; })
      .wait();
      while(true);

  } catch (...) {
    std::cout << "some error happened, bye!\n";
  }
}

}