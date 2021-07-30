#pragma once

#include <string>

namespace web::json {
    class value;
}

namespace web::http {
    class http_request;
}

namespace HttpServer {
// action, like stop go?
// duration?

enum class Direction {Left, Right, Forward, Backward};

Direction extractDirection(web::json::value body);

void handle_post(web::http::http_request request);

void run(const std::string& uri);

}