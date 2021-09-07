#pragma once

#include <optional>
#include <string>
#include <chrono>
#include <cpprest/http_listener.h>
#include <cpprest/uri.h>

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

class Server {
public:
    Server(const std::string& uri) : m_address{web::uri{uri}}, m_listener{m_address} {
        m_listener.support(web::http::methods::POST, handlePost);
        m_listener.support(web::http::methods::GET, handleGet);
    }
    void run();

    void run(std::chrono::duration<int> forNSeconds);
private:
    static void handlePost(web::http::http_request request);
    static void handleGet(web::http::http_request request);
    static Direction extractDirection(web::json::value body);
    void runImplementation(std::optional<std::chrono::time_point<std::chrono::system_clock>> startTime, std::optional<std::chrono::duration<int>> duration);

    bool hasRunDurationPassed(std::optional<std::chrono::time_point<std::chrono::system_clock>> startTime, std::optional<std::chrono::duration<int>> duration) const;
    bool isEndlesslyRunning(std::optional<std::chrono::time_point<std::chrono::system_clock>> startTime, std::optional<std::chrono::duration<int>> duration) const;

    web::uri m_address;
    web::http::experimental::listener::http_listener m_listener;
};
}